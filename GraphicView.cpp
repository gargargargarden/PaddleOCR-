#include "GraphicView.h"

#include<QDebug>
GraphicView::GraphicView(QWidget* parent):QGraphicsView(parent)
{
	setSceneRect(INT_MIN / 2, INT_MIN / 2, INT_MAX, INT_MAX);

	// 去掉滚动条
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	setResizeAnchor(QGraphicsView::AnchorUnderMouse);
	
	setInteractive(true);
	//setDragMode(QGraphicsView::NoDrag);
	setDragMode(QGraphicsView::ScrollHandDrag);  // 可选：启用拖拽
}

void GraphicView::setBackgroundImage(const QString& imagePath)
{
	m_background = QPixmap(imagePath);
	if (!imagePath.isNull()) {
		scene()->setSceneRect(m_background.rect());
		viewport()->update();
	}
}

void GraphicView::drawBackground(QPainter* painter, const QRectF& rect)
{
	// 先调用基类实现以保留默认行为
	QGraphicsView::drawBackground(painter, rect);
}

void GraphicView::paintEvent(QPaintEvent* event)
{
	QGraphicsView::paintEvent(event);
	QPainter painter(viewport());
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing | QPainter::SmoothPixmapTransform);
}

void GraphicView::dragEvent(QMouseEvent* ev)
{
	//QGraphicsView::setDragMode(QGraphicsView::ScrollHandDrag);
}

void GraphicView::translate(QPointF delta)
{
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	QPoint newCenter(viewport()->rect().width() / 2 - delta.x(), viewport()->rect().height() / 2 - delta.y());
	centerOn(mapToScene(newCenter));

	// scene 在 view 的中心点做为锚点
	setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}

void GraphicView::wheelEvent(QWheelEvent* event)
{
	// 滚轮的滚动量
	QPoint scrollAmount = event->angleDelta();
	// 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
	double zoomFactor = 0;
	zoomFactor = scrollAmount.y() > 0 ? (1 + zscale) : (1 - zscale);
	// 防止太小或过大
	qreal factor = transform().scale(zoomFactor, zoomFactor).mapRect(QRectF(0, 0, 1, 1)).width();
	if (factor < 0.07 || factor > 100)
	{
		return;
	}
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	scale(zoomFactor, zoomFactor);
	event->accept();
	
}

void GraphicView::mousePressEvent(QMouseEvent* event)
{
	// 如果按下鼠标左键，记录当前的鼠标位置
	if (event->button() == Qt::LeftButton) {
		// 将鼠标按下的位置保存，准备拖动
		m_dragStartPosition = event->pos();
		setCursor(Qt::ClosedHandCursor);
	}
	QGraphicsView::mousePressEvent(event);  // 调用基类的处理方法
}

void GraphicView::mouseMoveEvent(QMouseEvent* event)
{
	// 如果鼠标左键按下并且在拖动，更新视图的偏移量
	if (event->buttons() & Qt::LeftButton) {
		// 计算当前鼠标位置的偏移量
		//QPoint offset = event->pos() - m_dragStartPosition;
	//	// 计算鼠标当前的位置与鼠标按下时的位置偏移量
		//qreal deltaX = event->pos().x() - m_dragStartPosition.x();
		//qreal deltaY = event->pos().y() - m_dragStartPosition.y();
		//QPointF point(deltaX, deltaY);
	//	// 更新场景的位置
	//	// 使用 translate 来调整视图的场景位置
		//translate(point); // 向相反方向移动视图

	//	// 更新起始位置，以便下次计算偏移量
		//m_dragStartPosition = event->pos();
		//qDebug() << "移动了";
	}
	QGraphicsView::mouseMoveEvent(event);  
}

void GraphicView::mouseReleaseEvent(QMouseEvent* event)
{
	// 释放鼠标时，结束拖动
	if (event->button() == Qt::LeftButton) {

	}
	setCursor(Qt::OpenHandCursor);
	QGraphicsView::mouseReleaseEvent(event);  
}

void GraphicView::keyPressEvent(QKeyEvent* event)
{

}
