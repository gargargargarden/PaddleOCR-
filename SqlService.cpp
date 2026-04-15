#include "SqlService.h"
#include<QMutex>

//SqlService* SqlService::self;
//void SqlService::InitInstance(QSqlDriver* sqldriver)
//{
//	if (self == nullptr) {
//		QMutex mutex;
//		QMutexLocker locker(&mutex);
//		if (self == nullptr) {
//			self = new SqlService(sqldriver);
//		}
//	}
//}
//SqlService* SqlService::Instance()
//{
//	return self;
//}

SqlService::~SqlService() {
    // 关闭数据库连接
    db.close();
}
bool SqlService::open(QString filename) {
    // 创建和打开数据库连接
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filename);
    if (!db.open()) {
        qDebug() << "Error: Unable to open database" << db.lastError().text();
        return false;
        isDbOpen = true;
    }
    isDbOpen = true;
    return true;
}
SqlService::QueryResult SqlService::execute(const QString& queryStr)
{
    QueryResult result;
    if (!isDbOpen) {
        result.success = false;
        result.error = "database not open";
        return result;
    }
    QSqlQuery query;
    if (!query.exec(queryStr)) {
        result.success = false;
        result.error = query.lastError().text();
        qCritical() << "Query failed:" << result.error << "\nQuery:" << queryStr;
        return result;
    }

    result.success = true;
    while (query.next()) {
        QVariantList recordlist;
        for (int i = 0; i < query.record().count(); ++i) {
            recordlist.append(query.value(i));
        }
        result.records.append(recordlist);
    }
    return result;
}

void SqlService::executeInsert()
{

}

void SqlService::updateExecute()
{
}

void SqlService::deleteExecute()
{
}

void SqlService::setDataBase(QString filename)
{
    this->dbName = filename;
}
