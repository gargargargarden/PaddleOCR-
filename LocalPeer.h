#ifndef LOCALPEER_H
#define LOCALPEER_H

#pragma once
// Qt
#include<QLocalServer>
#include<QLocalSocket>
#include<QDir>
#include<qobject.h>
#include<QString>
#include"LockedFile.h"
class LocalPeer:public QObject
{
    Q_OBJECT
public:
    LocalPeer(QObject* parent = 0, const QString& appId = QString());//QObject *parent = 0，目的是让SingleApplication* 对象为父对象
    ~LocalPeer();
    bool isClient();
    bool sendMessage(const QString& message, int timeout);
    QString applicationId() const {
        return id;
    }
signals:
    void messageReceived(const QString& message);
protected slots:
    void receiveConnection();


protected:
    QString id;
    QString socketName;
    QLocalServer* server;
    LockedFile lockFile;

private:
    static const char* ack;


};

#endif // LOCALPEER_H


