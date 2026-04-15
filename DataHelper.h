#pragma once
#include"SqlService.h"
#include"Building.h"
#include"Floor.h"
#include"Led.h"
#include"Area.h"

#include<QList>
#include<QPixmap>
class DataHelper
{
public:

	DataHelper();
	DataHelper& operator=(const DataHelper&) = delete;
	~DataHelper();

	static DataHelper* self;
	static void InitInstance();
	static DataHelper* Instance();
	QList<Building*> GetBuildingList() {
		return BuildingList;
	}
	QList<Floor*> GetFloorList() {
		return FloorList;
	}
	QList<Led*> GetLedList() {
		return LedList;
	}
	QList<Area*> GetAreaList() {
		return AreaList;
	}
	void loadBuildingList();
	void loadFloorList();
	void loadLedList();
	void loadAreaList();
	void updateLedPosById(int id,const QPointF& point);
	QList<Led*> GetLedlistByAreaId(int areaId);
	Led* GetLedById(QString ledId);
	void updateLedAreaId(int phyId, int floorId);
	void closeUpdate();
	void setFilename(QString filename);
private:
	QList<Building*> BuildingList;
	QList<Floor*> FloorList;
	QList<Led*> LedList;
	QList<Led*> areaLEDList;
	QList<Area*> AreaList;
	SqlService* sqlservice;
	QString filename;
};

