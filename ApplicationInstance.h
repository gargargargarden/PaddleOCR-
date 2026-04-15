#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H
#pragma once

#include<QApplication>
#include<QMetaType>
#include<QMutex>
#include<QString>
#include<QObject>
#include"LocalPeer.h"

class ApplicationInstance : public QApplication,QObject
{
    Q_OBJECT
public:

    ApplicationInstance(int& argc, char** argv, bool GUIenableId = true);
    ApplicationInstance(const QString& id, int& argc, char** argv, bool GUIenabled = true);
    ~ApplicationInstance();

    static ApplicationInstance* getInstance();

    bool isRunning();
    QString id() const;

    void setActivationWindow(QWidget* aw, bool activateOnMessage = true);
    QWidget* activationWindow() const;
    bool notify(QObject* receiver, QEvent* e);
public slots:
    void registerGlobalShortcut(int key, int modifiers) {
        globalShortcuts.append(QPair<int, int>(key, modifiers));
    }
    bool sendMessage(const QString& message, int timeout = 5000);
    void activateWindow();
signals:
    void messageReceived(const QString& message);
    void fileOpenRequestReceived(const QString& fileName);
    void globalShortcutPressed(int key, int modifier);

protected:
    virtual bool event(QEvent* e);
private:
    void sysInit(const QString& appId = QString());
    LocalPeer* peer;
    QWidget* actwin;
    QMutex mutexEvents;
    QThread* mutexEventLLocker;
    static ApplicationInstance* singleInstance;

    QList<QPair<int, int>> globalShortcuts;

};

#endif // SINGLEAPPLICATION_H