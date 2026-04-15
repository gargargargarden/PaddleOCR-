#pragma once
#include<QString>
#include<QObject>
class Building : public QObject{
	Q_OBJECT
public:
	Building(int id,QString name):id(id),Name(name) {

	}
	~Building(){}
	int Id()const {
		return id;
	}
	QString getName()const {
		return Name;
	}

private:
	int id;
	QString Name;
};