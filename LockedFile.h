#ifndef LOCKEDFILE_H
#define LOCKEDFILE_H


#pragma once
#include<QtCore/QFile>
#include<QDebug>

#ifdef Q_OS_WIN
#include<QtCore/QVector>
#endif


class LockedFile:public QFile
{
public:
    enum LockMode {
        NoLock = 0,
        ReadLock,
        WriteLock
    };
    LockedFile();
    LockedFile(const QString& name);
    ~LockedFile();

    bool open(OpenMode mode);
    bool lock(LockMode mode, bool block = true);
    bool unlock();
    bool isLocked() const;
    LockMode lockMode() const;
private:
    LockMode m_lock_mode;
#ifdef Q_OS_WIN
    Qt::HANDLE wmutex;
    Qt::HANDLE rmutex;
    QVector<Qt::HANDLE> rmutexes;
    QString mutexname;

    Qt::HANDLE getMutexHandle(int idx, bool doCreate);
    bool waitMutex(Qt::HANDLE mutex, bool doBlock);

#endif
};

#endif // LOCKEDFILE_H