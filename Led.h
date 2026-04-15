#pragma once
#include<QList>
#include<QString>
#include<QObject>
class Item : public QObject{
    Q_OBJECT
};

class Led :public Item{
public:

    Led( const QString & physId, const QString& mapId, const QString & name, int TypeDetail, const QString& defaultStatus,
        const QString& EpsId,double x, double y, int Rotate, const QString& errorShield,const QString & areaId)
    : m_physId(physId), m_mapId(mapId), Name(name), m_typeDetail(TypeDetail),
        m_defaultStatus(defaultStatus), m_epsId(EpsId), m_x(x), m_y(y), m_rotate(Rotate),
        m_errorShield(errorShield), m_areaId(areaId) {}

    // Getter 函数（返回函数）
    QString physId() const { return m_physId; }
    QString mapId() const { return m_mapId; }
    QString name() const { return Name; }
    int typeDetail() const { return m_typeDetail; }
    QString defaultStatus() const { return m_defaultStatus; }
    QString epsId() const { return m_epsId; }
    double x() const { return m_x; }
    double y() const { return m_y; }
    int rotate() const { return m_rotate; }
    QString errorShield() const { return m_errorShield; }
    QString areaId() const { return m_areaId; }

private:

    QString m_physId;         // 存储 LED 的物理 ID
    QString m_mapId;          // 存储 LED 的映射 ID
    QString Name;           // 存储 LED 的名称
    int m_typeDetail;     // 存储 LED 的类型详情
    QString m_defaultStatus;  // 存储 LED 的默认状态
    QString m_epsId;          // 存储 LED 的 Eps ID
    double m_x = 0.0;         // 存储 LED 的 X 坐标
    double m_y = 0.0;         // 存储 LED 的 Y 坐标
    int m_rotate = 0;         // 存储 LED 的旋转角度
    QString m_errorShield;    // 存储 LED 的错误屏蔽信息
    QString m_areaId;         // 存储 LED 的区域 ID
};