#pragma once
#include<QString>
#include<QObject>
class Floor :public QObject{
    Q_OBJECT
public:
    Floor(int id, const QString& name, const QByteArray& imageBytes, const QString& imageType,
        int originWidth, int originHeight, int imageWidth, int imageHeight,
        qreal viewScale, qreal viewOffsetX, qreal viewOffsetY, int buildingId)
        : m_id(id), m_name(name), m_imageBytes(imageBytes), m_imageType(imageType),
        m_originWidth(originWidth), m_originHeight(originHeight),
        m_imageWidth(imageWidth), m_imageHeight(imageHeight),
        m_viewScale(viewScale), m_viewOffsetX(viewOffsetX), m_viewOffsetY(viewOffsetY),
        m_buildingId(buildingId) {}
    ~Floor(){}

    // === Getter 函数 ===
   // 基本类型（int, qreal）
    int id() const { return m_id; }
    int originWidth() const { return m_originWidth; }
    int originHeight() const { return m_originHeight; }
    int imageWidth() const { return m_imageWidth; }
    int imageHeight() const { return m_imageHeight; }
    qreal viewScale() const { return m_viewScale; }
    qreal viewOffsetX() const { return m_viewOffsetX; }
    qreal viewOffsetY() const { return m_viewOffsetY; }
    int buildingId() const { return m_buildingId; }

    // Qt 类型（QString, QByteArray）
    QString getName() const { return m_name; }
    QByteArray imageBytes() const { return m_imageBytes; }
    QString imageType() const { return m_imageType; }
private:
    // 对应数据库字段
    int m_id = 0;                   // INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT
    QString m_name;                  // TEXT NOT NULL
    QByteArray m_imageBytes;         // BLOB NOT NULL
    QString m_imageType;             // TEXT NOT NULL
    int m_originWidth = 0;           // INTEGER NOT NULL
    int m_originHeight = 0;          // INTEGER NOT NULL
    int m_imageWidth = 0;            // INTEGER NOT NULL
    int m_imageHeight = 0;           // INTEGER NOT NULL
    qreal m_viewScale = 1.0;         // REAL NOT NULL
    qreal m_viewOffsetX = 0.0;       // REAL NOT NULL
    qreal m_viewOffsetY = 0.0;       // REAL NOT NULL
    int m_buildingId = 0;            // INTEGER NOT NULL

};