#include "ApplicationInstance.h"
#include<QWidget>
#include<QFileOpenEvent>

ApplicationInstance* ApplicationInstance::singleInstance ;
ApplicationInstance::ApplicationInstance(int& argc, char** argv, bool GUIenableId )
    :QApplication(argc, argv, GUIenableId)// SingleApplication既能保持QApplication的所有功能，又能扩展出额外的特性
{                                       // 学术名称叫做初始化列表
    sysInit();
}

ApplicationInstance::ApplicationInstance(const QString& appId, int& argc, char** argv, bool GUIenabled )
    :QApplication(argc, argv, GUIenabled) {
    sysInit(appId);
}

ApplicationInstance::~ApplicationInstance()
{
    qDebug() << "delete ApplicationInstance";
}

ApplicationInstance* ApplicationInstance::getInstance()
{
    return singleInstance;
}

bool ApplicationInstance::isRunning()
{
    return peer->isClient();
}

QString ApplicationInstance::id() const
{
    return peer->applicationId();
}

void ApplicationInstance::setActivationWindow(QWidget* aw, bool activateOnMessage)
{
    actwin = aw;
    if (activateOnMessage) {
        QObject::connect(peer, SIGNAL(messageReceived(const QString&)), this, SLOT(activateWindow()));
    }
    else {
        QObject::disconnect(peer, SIGNAL(messageReceived(const QString&)), this, SLOT(activateWindow()));
    }
}

QWidget* ApplicationInstance::activationWindow() const
{
    return actwin;
}

bool ApplicationInstance::notify(QObject* receiver, QEvent* e)
{
    if (e->type() == QEvent::KeyPress) {
        QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
        for (int i = 0; i < globalShortcuts.length(); i++) {
            if (keyEvent->key() == globalShortcuts[i].first && keyEvent->modifiers() == globalShortcuts[i].second) {
                emit globalShortcutPressed(keyEvent->key(), keyEvent->modifiers());
                e->accept();
                return true;
            }
        }


    }
    return QApplication::notify(receiver, e);

}

bool ApplicationInstance::sendMessage(const QString& message, int timeout)
{
    return false;
}

void ApplicationInstance::activateWindow()
{
    if (actwin) {
        actwin->setWindowState(actwin->windowState() & ~Qt::WindowMinimized);
        actwin->raise();
        actwin->activateWindow();
    }
}

// 处理所有类型的事件
bool ApplicationInstance::event(QEvent* e) {
    QFileOpenEvent* foe = dynamic_cast<QFileOpenEvent*>(e);
    if (foe != NULL) {
        emit(fileOpenRequestReceived(foe->file()));
        e->accept();
        return true;
    }

    bool ret = QApplication::event(e);
    return ret;
}
void ApplicationInstance::sysInit(const QString& appId) {
    mutexEventLLocker = NULL;  //多线程同步互斥锁
    singleInstance = this;
    actwin = 0;
    peer = new LocalPeer(this, appId);
    QObject::connect(peer, SIGNAL(messageReceive(const QString&)), SIGNAL(messageReceive(const QString&)));
}