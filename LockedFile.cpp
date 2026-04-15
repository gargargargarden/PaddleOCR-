#include "LockedFile.h"
#include<qt_windows.h>

// Qt
#include<QFileInfo>
#define MUTEX_PREFIX "LockedFile mutex "
// Maximum number of concurrent read locks. Must not be greater than MAXIMUM_WAIT_OBJECTS
#define MAX_READERS MAXIMUM_WAIT_OBJECTS

#if QT_VERSION >= 0x050000
#define QT_WA(unicode, ansi) unicode
#endif
LockedFile::LockedFile()
{
}

LockedFile::LockedFile(const QString& name)
{
#ifdef Q_OS_WIN
    wmutex = 0;
    rmutex = 0;
#endif
    m_lock_mode = NoLock;
}

LockedFile::~LockedFile()
{
    if (isOpen()) {
        unlock();

    }    if (wmutex) {
        CloseHandle(wmutex);

    }
}

bool LockedFile::open(OpenMode mode)
{
    if (mode & QIODevice::Truncate) {
        qWarning("LockFile::open():Truncate is not allowed");
        return false;
    }
    return QFile::open(mode);
}

bool LockedFile::lock(LockMode mode, bool block)
{
    // 检查文件是否打开
    if (!isOpen()) {
        qWarning("LockedFile::lock(): file is not opened");
        return false;
    }
    // 处理不加锁的情况
    if (mode == NoLock) {
        return unlock();
    }
    // 处理重复加锁的情况
    if (mode == m_lock_mode) {
        return true;
    }
    // 解锁当前锁定状态
    if (m_lock_mode != NoLock) {
        unlock();
    }
    // 尝试获取写锁的互斥锁
    if (!wmutex && !(wmutex = getMutexHandle(-1, true))) {
        return false;
    }
    // 等待获取互斥锁
    if (!waitMutex(wmutex, block)) {
        return false;
    }
    // 处理读锁
    if (mode == ReadLock) {
        int idx = 0;
        for (; idx < MAX_READERS; idx++) {
            rmutex = getMutexHandle(idx, false);
            // rmutex为0,waitmutex返回1,结果为1
            // rmutex为1，waitmutex返回0，结果为1
            //          1,            1,结果为1
            if (!rmutex || waitMutex(rmutex, false))
            {
                break;
            }
            CloseHandle(rmutex);
        }
        bool ok = true;
        if (idx >= MAX_READERS) {
            qWarning("LockedFile::lock() :too many readers");
            rmutex = 0;
            ok = false;
        }
        else if (!rmutex) {
            rmutex = getMutexHandle(idx, true);
            if (!rmutex || !waitMutex(rmutex, false)) {
                ok = false;
            }
        }
        if (!ok && rmutex) {
            CloseHandle(rmutex);
            rmutex = 0;
        }
        ReleaseMutex(wmutex);
        if (!ok)
        {
            return false;
        }
    }
    // 处理写锁
    else {
        Q_ASSERT(rmutexes.isEmpty());
        for (int i = 0; i < MAX_READERS; i++) {
            Qt::HANDLE mutex = getMutexHandle(i, false);
            if (mutex) {
                rmutexes.append(mutex);
            }
        }
        // 如果有读锁存在，使用WaitForMultipleObjects等待所有读锁释放
        if (rmutexes.size()) {
            /*
             * @param:rmutexes.size()   等待的对象数量，即有多少个互斥锁需要等待
             * @param:rmutexes.constData()   互斥锁句柄数组
             * @param:True    表示等待的方式是“等待所有对象”，即只有当所有的互斥锁都被释放，该函数才会返回
             * @param:block ? INFINITE : 0    等待时间，如果block为true,则设置为永远等待，直到所有的互斥锁都被释放，
             * 如果为false，则不等待，如果没有获取到所有的互斥锁，就立即返回
             *
             * @return:WAIT_OBJECT_0：表示所有等待的对象（互斥锁）都成功被释放。
             * @return:WAIT_ABANDONED：表示等待的对象已经被放弃（不再使用）。
             * @return:WAIT_TIMEOUT：表示等待超时（没有获得所有对象）
             * */
            DWORD res = WaitForMultipleObjects(rmutexes.size(), rmutexes.constData(), TRUE, block ? INFINITE : 0);
            //
            if (res != WAIT_OBJECT_0 && res != WAIT_ABANDONED) {
                if (res != WAIT_TIMEOUT) {
                    qErrnoWarning("LockedFile::lock(): waitForMultipleObjects failed");

                }
                m_lock_mode = WriteLock;
                // 出现了其他错误，解锁当前状态，释放已经获取的资源
                unlock();
                return false;
            }
        }
    }
    // 设置当前锁模式为传入的模式，并返回true,表示锁定成功
    m_lock_mode = mode;
    return true;
}

bool LockedFile::unlock()
{
    if (!isOpen()) {
        qWarning("LockedFile::unlock():file is not opened");
        return false;
    }
    if (!isLocked()) {
        return true;
    }
    if (m_lock_mode == ReadLock) {
        ReleaseMutex(rmutex);
        CloseHandle(rmutex);
        rmutex = 0;
    }
    else {
        foreach(Qt::HANDLE mutex, rmutexes) {
            ReleaseMutex(mutex);
            CloseHandle(mutex);
        }
        rmutexes.clear();
        ReleaseMutex(wmutex);
    }

    m_lock_mode = LockedFile::NoLock;
    return true;
}
bool LockedFile::isLocked() const {
    return m_lock_mode != NoLock;
}

LockedFile::LockMode LockedFile::lockMode() const {
    return m_lock_mode;
}

Qt::HANDLE LockedFile::getMutexHandle(int idx, bool doCreate)
{
    if (mutexname.isEmpty()) {
        QFileInfo fi(*this);
        mutexname = QString::fromLatin1(MUTEX_PREFIX) + fi.absoluteFilePath().toLower();
    }
    QString mname(mutexname);
    if (idx >= 0) {
        mname += QString::number(idx);
    }
    Qt::HANDLE mutex;
    if (doCreate) {
        QT_WA({ mutex = CreateMutexW(NULL,FALSE,(TCHAR*)mname.utf16()); },
            { mutex = CreateMutexA(NULL,FALSE,mname.tolocal8Bit().constData()); });
        if (!mutex) {
            qErrnoWarning("LockedFile::lock():CreateMutex Failed");
            return 0;
        }
    }
    else {
        QT_WA({ mutex = OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE,FALSE,(TCHAR*)mname.utf16()); },
            { mutex = OpenMutexA(SYNCHRONIZE | MUTEX_MODIFY_STATE,FALSE,mname.tolocal8Bit().constData()); });
        if (!mutex) {
            if (GetLastError() != ERROR_FILE_NOT_FOUND)
            {
                qErrnoWarning("LockedFile::lock():OpenMutex Failed");
            }
            return 0;

        }
    }
    return mutex;
}
bool LockedFile::waitMutex(Qt::HANDLE mutex, bool doBlock)
    {
        Q_ASSERT(mutex);
        DWORD res = WaitForSingleObject(mutex, doBlock ? INFINITE : 0);
        switch (res) {
        case WAIT_OBJECT_0:
        case WAIT_ABANDONED:
            return true;
            break;
        case WAIT_TIMEOUT:
            break;
        default:
            qErrnoWarning("LockedFile::lock() WaitForSingleObject failed");
        }
        return false;

    
}
