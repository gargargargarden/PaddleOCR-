#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include<QSqlRecord>
#include <QVariant>
#include <QDebug>
#include <QVariantList>
#include <QVector>
class SqlService:public QSqlDatabase
{
public:
	SqlService() {

	}
	~SqlService();
	struct QueryResult {
		bool success;
		QString error;
		QVector<QVariantList> records; // 查询结果集
	};
	//static SqlService* self;
	//static void InitInstance(QSqlDriver *sqldriver);
	//SqlService* Instance();
	bool open(QString filename);

	// 纯粹的查询执行，返回一个结果，一个数，或者一个数组
	QueryResult execute(const QString& queryStr);
	// 带参数的查询执行
	//void execute();
	void executeInsert();
	void updateExecute();
	void deleteExecute();
	void setDataBase(QString dbName);
private:
	QSqlDatabase db;
	bool isDbOpen = false;
	QString dbName;
};

