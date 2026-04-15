#pragma once
#include <QFile>
#include <QTextStream>
#include <QString>
#include <QSettings>
#include<QCoreApplication>
class Setting :public QObject {
	Q_OBJECT
public:
	static QString dataBase;
    static QString License;
    static Setting* self;
    static void InitInstance();
    static Setting* Instance();


    // 设置全局变量到 QSettings
    static void setGlobalVariablesToSettings()
    {
        QSettings settings("MyApp", "Settings");
        // 保存全局变量
        settings.setValue("dataBase", "11111");
        settings.setValue("License", "22222");
    }

    // 将全局变量写入到文本文件
    static void writeVariablesToTxtFile()
    {
        QFile file("setting.txt");



    }

    static void writeToFileInAppDirectory() 
    {
        // 获取当前应用程序的目录路径
        QString appDir = QCoreApplication::applicationDirPath();

        // 构造目标文件的路径
        QString filePath = appDir + "/setting.txt";

        // 创建一个 QFile 对象，指定路径为 setting.txt
        QFile file(filePath);

        // 打开文件进行写操作
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            //QMessageBox::critical(nullptr, "Error", "Failed to open file for writing.");
            return;
        }

        // 创建 QTextStream 用于向文件中写入内容
        QTextStream out(&file);
        out << "Database Path: /path/to/database.db\n";
        out << "Username: admin\n";
        out << "Theme: dark\n";

        // 关闭文件
        file.close();

    }

private:
};