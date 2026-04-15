#pragma once
#include<QGraphicsScene>
#include<QPainter>
#include<QGraphicsItem>
#include<QDebug>
#include<QGraphicsSceneMouseEvent>
#include<QDialog>
#include<QVBoxLayout>
#include<QLabel>
#include<QLineEdit>
#include<QDialogButtonBox>
#include<QMessageBox>
#include<QStandardItem>
#include<QImage>

#include"DataHelper.h"
#include"ScreenWidget/screen.h"
#include"ScreenWidget/screenwidget.h"
#include"GraphicItem.h"
#include"Led.h"

#include "glog/logging.h"
#include "omp.h"
#include "opencv2/core.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include <chrono>
#include <iomanip>
#include <iostream>
#include <ostream>
#include <vector>

#include <cstring>
#include <fstream>
#include <numeric>

#include "PaddleOCR/include/config.h"
#include <PaddleOCR/include/ocr_det.h>
#include <PaddleOCR/include/ocr_rec.h>

#include <QImage>
#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")// 该指令仅支持VS环境
#endif

using namespace std;
using namespace cv;
using namespace PaddleOCR;
class GraphicScene:public QGraphicsScene
{
	Q_OBJECT
public:
	GraphicScene();
	~GraphicScene(){}
	void setBackgroundImage(const QString& path);
	//void testScene(QGraphicsSceneMouseEvent* event, GraphicScene* scene);

	void drawBackground(QPainter* painter, const QRectF& rect) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) ;
	void getRect( qreal width, qreal height);
	GraphicScene& getScene() {
		return *this;
	}

	void setDoubleDialog(bool flag) {
		doubleDia = flag;
	}

	std::string qstr2str(const QString qstr);
	QString str2qstr(const std::string str)
	{
		return QString::fromUtf8(str.data());
		//return QString::fromLocal8Bit(str.data());
	}

	QString convertToQString(std::vector<std::string>& str_array) {
		QString result;

		for (const auto& str : str_array) {
			result += str2qstr(str); 
		}

		return result.trimmed(); // 删除最后的空格
	}
	OCRConfig* config = nullptr;
	DBDetector* det = nullptr;
	Classifier* cls = nullptr;
	CRNNRecognizer* rec = nullptr;
signals:
	void emitDouClkPos(QPointF&);

public slots:
	void slt_SelectImg(QImage img);
private:
	QPixmap m_background;
	QGraphicsItem* m_currentItem;  // 当前选中的图形项
	GraphicScene* scene;
	qreal pixmapWidth;
	qreal pixmapHeight;
	bool found = false;
	bool doubleDia = true;
	QLineEdit* lineEdit;
	QDialog* dialog;
	GraphicItem* setBgImg;
	QList<Led*>  LedList;
};

