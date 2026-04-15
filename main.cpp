#include "OCRFastPoint.h"
#include <QtWidgets/QApplication>
#include"ApplicationInstance.h"
#include"SqlService.h"
#include<QSqlDriver>
#include<QTimer>
#include<QDateTime>
#include"DataHelper.h"
int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    ApplicationInstance *app = new ApplicationInstance(argc, argv);
    if (app->sendMessage(app->arguments().join("\n"), 30000)) {
        qWarning("Application already running,Aborting...");
            return 0;
    }

    QDateTime fixedExitTime = QDateTime::fromString("2025-12-01 00:00:00", "yyyy-MM-dd HH:mm:ss");

    QDateTime startTime = QDateTime::currentDateTime();
    QTimer* timer = new QTimer();
    /*QObject::connect(timer, &QTimer::timeout, [=]() {
        if (QDateTime::currentDateTime() > fixedExitTime) {
            qWarning("Time exceeded the threshold, exiting...");
            app->exit();
            delete app;
        }
        });*/
    timer->start(1000); 
    OCRFastPoint *window = new OCRFastPoint(app);
    window->show();
    int ret = app->exec();  // 启动事件循环
    DataHelper::Instance()->closeUpdate();
    // 在事件循环结束后删除动态分配的对象
    delete window;  // 删除窗口对象
    delete app;     // 删除 ApplicationInstance 对象
    
    return ret;  // 返回事件循环的返回值
}
