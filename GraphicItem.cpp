#include "GraphicItem.h"
#include<QFile>
GraphicItem::GraphicItem(int ledType,int physId):m_isSelected(false),m_isHovered(false)
{
    phyId = physId;
    if (ledType >= 3 && ledType <= 20) {
        m_pix = new QPixmap(":/new/prefix1/img/Exit.jpg");
        setScale(0.3);
    }
    else if (ledType >= 21 && ledType <= 25) {
        m_pix = new QPixmap(":/new/prefix1/img/left.jpg");
        setScale(0.3);
    }
    else if (ledType >= 26 && ledType <= 30) {
        m_pix = new QPixmap(":/new/prefix1/img/right.jpg");
        setScale(0.3);
    }
    else if (ledType >= 31 && ledType <= 35) {
        m_pix = new QPixmap(":/new/prefix1/img/twoway.jpg");
        setScale(0.3);
    }
    else if (ledType >= 36 && ledType <= 40) {
        m_pix = new QPixmap(":/new/prefix1/img/embedLeft.jpg");
        setScale(0.3);
    }
    else if (ledType >= 41 && ledType <= 45) {
        m_pix = new QPixmap(":/new/prefix1/img/embedRight.jpg");
        setScale(0.3);
    }
    else if (ledType >= 46 && ledType <= 50) {
        m_pix = new QPixmap(":/new/prefix1/img/embedTwoway.jpg");
        setScale(0.3);
    }
    else if (ledType >= 51 && ledType <= 100) {
        m_pix = new QPixmap(":/new/prefix1/img/light2.jpg");
        setScale(0.3);
    }
    else if (ledType >= 101 && ledType <= 110) {
        m_pix = new QPixmap(":/new/prefix1/img/light2.jpg");
        setScale(0.2);
    }
    else if (ledType >= 111 && ledType <= 120) {
        m_pix = new QPixmap(":/new/prefix1/img/light2.jpg");
        setScale(0.2);
    }
    else {
        m_pix = new QPixmap(":/new/prefix1/img/Exit.jpg");
        setScale(0.3);
    }

    if (m_pix->isNull()) {
        qDebug() << "Failed to load the image from the resource.";
    }
    else {
        //qDebug() << "Image loaded successfully from the resource.";
    }

    QPixmap scaledPix = m_pix->scaled(m_pix->width() * 2, m_pix->height() * 2, Qt::KeepAspectRatio);
    setFlag(ItemIsMovable);
    prepareGeometryChange();
    setAcceptHoverEvents(true);
}

GraphicItem::GraphicItem() :m_isSelected(false), m_isHovered(false) {

}
void GraphicItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setBrush(Qt::blue); // 设置颜色
    painter->drawRect(boundingRect()); // 绘制矩形，使用图形项的边界矩形
    painter->drawPixmap(0,0, *m_pix);  // 将图片绘制到图形项的原点

   // 如果被选中，绘制选中框
    if (m_isSelected ) {
        QPen pen(Qt::DashLine);
        pen.setColor(Qt::red);
        pen.setWidth(5);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(1, 1, -1, -1));
    }
    if (m_isHovered) {

        painter->setBrush(QColor(255, 0, 255, 100)); // 半透明红色 (alpha=100)
        painter->setPen(Qt::NoPen);
        painter->drawRect(boundingRect());
    }
    update();
}
void GraphicItem::setSelected(bool select)
{
    m_isSelected = select;
}
void GraphicItem::changePropotion()
{

}
QRectF GraphicItem::boundingRect() const
{
    const qreal margin = 5.0;
    if (m_pix) {
        return QRectF(QPointF(0, 0), m_pix->size()).adjusted(-margin, -margin, margin, margin);
    }
    return QRectF(-margin, -margin, 10 + 2 * margin, 10 + 2 * margin);
}

void GraphicItem:: mousePressEvent(QGraphicsSceneMouseEvent* event) {

    if (event->buttons() & Qt::LeftButton) {

        // 禁用 QGraphicsView 的拖拽
        view = this->scene()->views().first(); 
        if (view) {
            view->setDragMode(QGraphicsView::NoDrag);  // 禁用视图拖拽
        }
    }
    m_isSelected = true;
    //qDebug() << "Item pressed at:" << event->scenePos();
    QGraphicsItem::mousePressEvent(event);  
}

void GraphicItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)  {

    if (event->buttons() & Qt::LeftButton) {
        
        QGraphicsItem::mouseMoveEvent(event);
        QPointF scenePos = event->scenePos();
        m_backgroundItem = this->parentItem();
        QPointF localPos = m_backgroundItem->mapFromScene(scenePos);
        
        QVariant ledId = this->data(0);
        DataHelper::Instance()->updateLedPosById(ledId.toInt(), localPos);
        event->accept(); 
        this->update();
    }
    else {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void GraphicItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    m_isSelected = false;
    view = this->scene()->views().first(); 
    if (view) {
        view->setDragMode(QGraphicsView::ScrollHandDrag); 
    }
    QGraphicsItem::mouseReleaseEvent(event); 
}

//void GraphicItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
//{
//    qDebug() << "mouse scene double click" << event->pos();
//}

// 鼠标悬停进入 item 时触发
void GraphicItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)  {
    m_isHovered = true;
    update(); // 强制重绘
    QGraphicsItem::hoverEnterEvent(event);
}

// 鼠标在 item 上移动时触发
void GraphicItem::hoverMoveEvent(QGraphicsSceneHoverEvent* event)  {
    QGraphicsItem::hoverMoveEvent(event);
}

// 鼠标悬停离开 item 时触发
void GraphicItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)  {
    m_isHovered = false;
    update(); // 强制重绘
    QGraphicsItem::hoverLeaveEvent(event);
}