#pragma once
#include<QGraphicsView>
#include<QPixmap>
#include<QPainter>
#include <QMouseEvent>

class GraphicView : public QGraphicsView
{
public:
	explicit GraphicView(QWidget *parent=nullptr);
	~GraphicView(){

    }
	void setBackgroundImage(const QString& imagePath);

    // 常用需要重载的函数
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void wheelEvent(QWheelEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void dragEvent(QMouseEvent* ev);
    void translate(QPointF delta);
private:
	QPixmap m_background;
    QPoint m_dragStartPosition;
    QPointF m_pressedPos;   // 平移时鼠标点下的位置
    float zscale = 0.1;           //用于缩放

    QPointF centerAnchor;
};

