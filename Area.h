#pragma once
#include<QString>
#include<QObject>
#include<QVector>
class Area :public QObject {
    Q_OBJECT
public:
    Area(int id,QString name,QVector<QPointF> PolygonPoints,int FloorId):m_id(id),m_name(name), m_PolygonPoints(PolygonPoints), m_FloorId(FloorId){}
    ~Area(){}
    // Getter º¯Êý
    int id() const { return m_id; }
    QString name() const { return m_name; }
    QVector<QPointF> polygonPoints() const { return m_PolygonPoints; }
    int floorId() const { return m_FloorId; }
private:
    int m_id;
    QString m_name;
    QVector<QPointF> m_PolygonPoints;
    int m_FloorId;
};