#include "DataHelper.h"

#include<QMutex>
DataHelper* DataHelper::self;
void DataHelper::InitInstance()
{
	if (self == nullptr) {
		QMutex mutex;
		QMutexLocker locker(&mutex);
		if (self == nullptr) {
			// 懒汉单例模式
			self = new DataHelper();
		}
	}
}
DataHelper* DataHelper::Instance() {

	return self;
}

DataHelper::DataHelper():sqlservice(new SqlService())
{

}

DataHelper::~DataHelper()
{
    closeUpdate();
    delete sqlservice;  // 在析构函数中释放内存
}


void DataHelper::loadBuildingList()
{
    if (sqlservice == nullptr) {
        return;
    }
    SqlService::QueryResult result = sqlservice->execute("select * from Building");
	// 打印查询结果
    // 如果查询成功，打印结果
    if (result.success) {
        for (const QVariantList& row : result.records) {
            
            int id = row.at(0).toInt();
            QString name = row.at(1).toString();
            Building* building = new Building(id, name);
            BuildingList.append(building);
            // 打印查询的每一行
            /*qDebug() << "Building ID:" << id
                << ", Name:" << name;*/
        }
    }
    else {
        // 如果查询失败，打印错误信息
        qDebug() << "Error executing query:" << result.error;
    }
}

void DataHelper::loadFloorList()
{
    if (sqlservice == nullptr) {
        return;
    }
    SqlService::QueryResult result = sqlservice->execute("select * from Floor");
    // 打印查询结果
    // 如果查询成功，打印结果
    if (result.success) {
        for (const QVariantList& row : result.records) {

            // 获取每一列的值，转换为适当的类型
            int id = row.at(0).toInt();
            QString name = row.at(1).toString();
            QByteArray imageBytes = row.at(2).toByteArray();
            QString imageType = row.at(3).toString();
            int originWidth = row.at(4).toInt();
            int originHeight = row.at(5).toInt();
            int imageWidth = row.at(6).toInt();
            int imageHeight = row.at(7).toInt();
            qreal viewScale = row.at(8).toReal();
            qreal viewOffsetX = row.at(9).toReal();
            qreal viewOffsetY = row.at(10).toReal();
            int buildingId = row.at(11).toInt();

            // 创建 Floor 对象
            Floor* floor = new Floor(id, name, imageBytes, imageType, originWidth, originHeight,
                imageWidth, imageHeight, viewScale, viewOffsetX, viewOffsetY, buildingId);

            // 将 Floor 对象添加到 FloorList 中
            FloorList.append(floor);

            // 打印查询的每一行数据（可以选择性打印）
            qDebug() << "Floor ID:" << id
                << ", Name:" << name<< "imageBytes" << imageBytes
                << ", Building ID:" << buildingId;
        }
    }
    else {
        // 如果查询失败，打印错误信息
        qDebug() << "Error executing query:" << result.error;
    }
}

void DataHelper::loadLedList()
{
    if (sqlservice == nullptr) {
        return;
    }

    // 执行查询
    SqlService::QueryResult result = sqlservice->execute("SELECT * FROM Led");

    // 创建一个 QList<LED*> 来存储 LED 对象
    QList<Led*> LEDList;

    // 如果查询成功，处理查询结果
    if (result.success) {
        // 遍历查询结果集中的所有记录
        for (const QVariantList& row : result.records) {
            // 提取各字段值
            QString physId = row.at(0).toString();
            QString mapId = row.at(1).toString();
            QString name = row.at(2).toString();
            int typeDetail = row.at(3).toInt();
            QString defaultStatus = row.at(4).toString();
            QString epsId = row.at(5).toString();
            double x = row.at(7).toDouble();
            double y = row.at(8).toDouble();
            int rotate = row.at(9).toInt();
            QString errorShield = row.at(10).toString();
            QString areaId = row.at(11).toString();

            // 创建LED对象并添加到列表
            Led* led = new Led(physId, mapId, name, typeDetail, defaultStatus,
                epsId, x, y, rotate, errorShield, areaId);
            LedList.append(led);
        }
    }
    else {
        qDebug() << "Error executing query:" << result.error;
    }
}

QList<Led*> DataHelper::GetLedlistByAreaId(int areaId)
{
    areaLEDList.clear();
    //if (sqlservice == nullptr) {
    //    return ;
    //}
    QString queryStr = QString("SELECT * FROM Led WHERE areaId = '%1'").arg(areaId);
    // 执行查询
    SqlService::QueryResult result = sqlservice->execute(queryStr);

    // 创建一个 QList<LED*> 来存储 LED 对象
    

    // 如果查询成功，处理查询结果
    if (result.success) {
        // 遍历查询结果集中的所有记录
        for (const QVariantList& row : result.records) {
            // 提取各字段值
            QString physId = row.at(0).toString();
            QString mapId = row.at(1).toString();
            QString name = row.at(2).toString();
            int typeDetail = row.at(3).toInt();
            QString defaultStatus = row.at(4).toString();
            QString epsId = row.at(5).toString();
            double x = row.at(7).toDouble();
            double y = row.at(8).toDouble();
            int rotate = row.at(9).toInt();
            QString errorShield = row.at(10).toString();
            QString areaId = row.at(11).toString();

            // 创建LED对象并添加到列表
            Led* led = new Led(physId, mapId, name, typeDetail, defaultStatus,
                epsId, x, y, rotate, errorShield, areaId);
            areaLEDList.append(led);

            // 打印当前LED信息（类似FloorList的格式）
            //qDebug() << "LED - PhysID:" << physId
            //    << ", Name:" << name
            //    << ", Type:" << typeDetail
            //    << ", Status:" << defaultStatus
            //    << ", Position: (" << x << "," << y << ")"
            //    << ", Area:" << areaId;
        }
        //qDebug() << "Loaded" << LedList.size() << "LEDs for area"
        //    << (areaId.isEmpty() ? "[ALL]" : areaId);
        //}
    //}
}
    else {
        qDebug() << "Error executing query:" << result.error;
    }
    return areaLEDList;
}

void DataHelper::loadAreaList()
{
    if (sqlservice == nullptr) {
        return;
    }
    SqlService::QueryResult result = sqlservice->execute("select * from Area");
    // 打印查询结果
    // 如果查询成功，打印结果
    if (result.success) {
        for (const QVariantList& row : result.records) {

            int id = row.at(0).toInt();
            QString name = row.at(1).toString();
            // 假设多边形点数据（poly）存储在 row 中的第三列
            QVariantList polyData = row.at(2).toList();  // 假设多边形点在第三列
            QVector<QPointF> poly;
            for (const QVariant& point : polyData) {
                QVariantList pointData = point.toList();  // 每个点是一个 QVariantList，包含 x 和 y
                qreal x = pointData.at(0).toDouble();    // 获取 x 坐标
                qreal y = pointData.at(1).toDouble();    // 获取 y 坐标
                poly.append(QPointF(x, y));              // 将点加入 QVector<QPointF>
            }
            int FloorId = row.at(3).toInt();
            Area* area = new Area(id, name,poly, FloorId);
            AreaList.append(area);
            // 打印查询的每一行
            /*qDebug() << "Building ID:" << id
                << ", Name:" << name;*/
        }
    }
    else {
        // 如果查询失败，打印错误信息
        qDebug() << "Error executing query:" << result.error;
    }
}

void DataHelper::closeUpdate()
{
    sqlservice->execute("PRAGMA wal_checkpoint");
    delete sqlservice;
}

void DataHelper::updateLedAreaId(int phyId , int floorId)
{
    if (sqlservice == nullptr) {
        return;
    }
    QString updateQuery = QString("update Led set AreaId = %1 where PhysicalId = %2").arg(floorId).arg(phyId);
    SqlService::QueryResult result = sqlservice->execute(updateQuery);
    if (result.success) {
        qDebug() << "update led FloorId  success!"<< phyId<< floorId;
        return;
    }
    else {
        qDebug() << "update led FloorId postion failed!";
    }
}

Led* DataHelper::GetLedById(QString ledId)
{
    for (int i = 0; i < LedList.length(); i++) {
        if (LedList[i]->physId() == ledId) {  // 直接比较字符串
            return LedList[i];  // 找到匹配的 LED，返回 true
        }
    }
    return nullptr;
}

void DataHelper::updateLedPosById(int id,const QPointF& point)
{
    if (sqlservice == nullptr) {
        return;
    }
    QString updateQuery = QString("update Led set X = %1 , Y=%2 where PhysicalId = %3").arg(point.x()).arg(point.y()).arg(id);
    SqlService::QueryResult result = sqlservice->execute(updateQuery);
    if (result.success) {
        qDebug() << "update led postion success!"<<id<< point.x()<< point.y();
        return;
    }
    else {
        qDebug() << "update led postion failed!";
    }
}

void DataHelper::setFilename(QString filename) {
    this->filename = filename;
    FloorList.clear();
    BuildingList.clear();
    LedList.clear();
    AreaList.clear();
    sqlservice->open(filename);
}
