#include "LocalPeer.h"

// Qt
#include<QCoreApplication>
#include<QDataStream>
#include<QTime>
#include <QRegularExpression>

#if defined (Q_OS_WIN)
#include<QLibrary>
#include<qt_windows.h>
typedef BOOL(WINAPI* PProcessIdToSessionId)(DWORD, DWORD*);//PProcessIdToSessionId 指向特定函数的指针类型，BOOL为函数的返回类型，(DWORD,DWORD*)是该函数指针类型所指向的函数的参数类型
static PProcessIdToSessionId pProcessIdToSessionId = 0;//声明了一个静态变量，他是一个PProcessIdToSessionId类型的指针
#endif
#if defined(Q_OS_UNIX)
#include <unistd.h>
#include <time.h>
#endif
// 单实例应用
const char* LocalPeer::ack = "ack";
LocalPeer::~LocalPeer()
{
}
LocalPeer::LocalPeer(QObject* parent, const QString& appId) :QObject(parent), id(appId) {
    QString prefix = id;
    if (id.isEmpty()) {
        id = QCoreApplication::applicationFilePath();
#ifdef Q_OS_WIN
        id = id.toLower();
#endif
        prefix = id.section(QLatin1Char('/'), -1);
    }
    prefix.remove(QRegularExpression("[^a-zA-Z]"));
    prefix.truncate(6);

    QByteArray idc = id.toUtf8();
    // 校验数据传输是否错误
    quint16 idNum = qChecksum(idc.constData(), idc.size());
    // 套接字的构成，由qtsingleapp-应用程序ID的前缀-校验和构成
    // qtsingleapp-app1234-1-1234

    socketName = QLatin1String("qtsingleapp-") + prefix + QLatin1Char('-') + QString::number(idNum, 16);
#if defined (Q_OS_WIN)
    if (!pProcessIdToSessionId) {
        QLibrary lib("kernel32");
        pProcessIdToSessionId = (PProcessIdToSessionId)lib.resolve("ProcessIdToSessionId");
    }
    if (pProcessIdToSessionId) {
        DWORD sessionId = 0;
        pProcessIdToSessionId(GetCurrentProcessId(), &sessionId);// ①32位的无符号整数，用于表示进程ID②指向DWORD类型的指针，用来存储会话ID
        socketName += QLatin1Char('-') + QString::number(sessionId, 16);
    }
#else
    socketName += QLatin1Char('-') + QString::number(::getuid(), 16);// socket变量追加一个基于当前用户唯一标识符的字符串
#endif
    server = new QLocalServer(this);
    QString lockName = QDir(QDir::tempPath()).absolutePath() + QLatin1Char('/') + socketName + QLatin1String("-lockfile");
    lockFile.setFileName(lockName);
    lockFile.open(QIODevice::ReadWrite);
}
bool LocalPeer::isClient()
{
    if (lockFile.isLocked()) {
        // 返回false，就是没有实例在运行

        return false;
    }
    // 文件已经被其他进程或线程锁定，不能锁定
    if (!lockFile.lock(LockedFile::WriteLock, false)) {
        return true;
    }
    bool res = server->listen(socketName);
#if defined(Q_OS_UNIX) && (QT_VERSION >= QT_VERSION_CHECK(4,5,0))
    // ### Workaround
    if (!res && server->serverError() == QAbstractSocket::AddressInUseError) {
        QFile::remove(QDir::cleanPath(QDir::tempPath()) + QLatin1Char('/') + socketName);
        res = server->listen(socketName);
    }
#endif

    if (!res) {
        qWarning("QtSingleCoreApplication: listen on local socket failed, %s", qPrintable(server->errorString()));
    }
    QObject::connect(server, SIGNAL(newConnection()), SLOT(receiveConnection()));
    return false;
}

bool LocalPeer::sendMessage(const QString& message, int timeout)
{
    if (!isClient()) {
        return false;
    }
    QLocalSocket socket;
    bool connOk = false;
    // 重发
    for (int i = 0; i < 2; i++) {
        socket.connectToServer(socketName);
        connOk = socket.waitForConnected(timeout / 2);
        if (connOk || i) {
            break;
        }
        int ms = 250;
#if defined(Q_OS_WIN)
        Sleep(DWORD(ms));
#else
        struct timespec ts = { ms / 1000, (ms % 1000) * 1000 * 1000 };
        nanosleep(&ts, NULL);
#endif
    }
    // 序列化
    QByteArray uMsg(message.toUtf8());
    QDataStream ds(&socket);
    ds.writeBytes(uMsg.constData(), uMsg.size());
    bool res = socket.waitForBytesWritten(timeout);
    res &= socket.waitForReadyRead(timeout);
    res &= (socket.read(qstrlen(ack)) == ack);
    return res;
}




// 实现进程间的通信(IPC)
void LocalPeer::receiveConnection() {
    // 等待数据，获取一个等待中的连接
    QLocalSocket* socket = server->nextPendingConnection();
    if (!socket) {
        return;
    }
    // 阻塞当前进程，直到套接字中有足够的数据可以供读取。确保套接字中有足够的数据，至少4字节，代表数据包的大小
    while (socket->bytesAvailable() < (int)sizeof(quint32))
    {
        socket->waitForReadyRead();
    }
    // 读取数据
    QDataStream ds(socket);
    QByteArray uMsg;
    quint32 remaining=0;
    int got = 0;
    char* uMsgBuf = uMsg.data();
    //    循环读取数据，remaining是剩余需要读取的数据量，
    do {
        got = ds.readRawData(uMsgBuf, remaining);
        remaining -= got;
        uMsgBuf += got;
    } while (remaining && got >= 0 && socket->waitForReadyRead(2000));
    // 错误处理
    if (got < 0) {
        qWarning() << "RLocalPeer: Message reception failed" << socket->errorString();
        delete socket;
        return;
    }
    //处理接收到的数据
    QString message(QString::fromUtf8(uMsg));
    socket->write(ack, qstrlen(ack));
    socket->waitForBytesWritten(1000);
    delete socket;
    emit messageReceived(message);

}