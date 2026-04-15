#pragma once
#include<QGraphicsItem>
#include<QGraphicsView>
#include<QGraphicsSceneMouseEvent>
#include<QDebug>
#include<QPainter>
#include<QPixmap>

#include"DataHelper.h"
class GraphicItem : public QGraphicsItem
{
public:
	GraphicItem(int type,int physId);
    GraphicItem();
    ~GraphicItem() {
        delete m_pix;
    }
  
	QRectF boundingRect() const override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    //void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    void setSelected(bool select);

    void changePropotion();

    int m_phyId() {
        return phyId;
    }
    void setBgImgItem(QGraphicsItem* img) {
        
        this->m_backgroundItem = img;
    }
protected:
    // 鼠标悬停进入 item 时触发
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

    // 鼠标在 item 上移动时触发
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;

    // 鼠标悬停离开 item 时触发
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
private:
    QPixmap* m_pix=nullptr;  // 存储图像
    bool m_isSelected;
    bool m_isHovered;
    QGraphicsItem* m_backgroundItem; // 存储背景项指针
    QGraphicsView* view;
    int phyId;
};

