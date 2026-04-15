#pragma once
// QT
#include <QtWidgets/QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include<QFont>
#include<QMessagebox>
#include<QLineEdit>
#include<QDialogButtonBox>
#include<QGraphicsTextItem>
#include <QPropertyAnimation>
#include<QPushButton>
#include<QHeaderView>
#include<QParallelAnimationGroup>
#include<QTableView>
#include<QSplitter>
#include<QFile>
#include<QTextStream>
#include<QStringList>
#include<QDebug>
#include<QSpinBox>

#include"DataHelper.h"
#include"GraphicView.h"
#include"GraphicItem.h"
#include"GraphicScene.h"
#include "ui_OCRFastPoint.h"
#include"setting.h"
#include"LedExcel.h"
#include"ApplicationInstance.h"

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
//#include <PaddleOCR/include/ocr_det.h>
//#include <PaddleOCR/include/ocr_rec.h>
#include"PaddleOCR/include/config.h"

#include"ScreenWidget/screen.h"
#include"ScreenWidget/screenwidget.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")// 该指令仅支持VS环境
#endif
using namespace std;
using namespace cv;
using namespace PaddleOCR;
class OCRFastPoint : public QMainWindow
{
    Q_OBJECT

public:
    explicit OCRFastPoint(ApplicationInstance* app , QWidget *parent = nullptr );
    ~OCRFastPoint();
    void InitTreeView();
	//void mouseDoubleClickEvent(QMouseEvent* event);
	OCRConfig* config = nullptr;
	DBDetector* det = nullptr;
	Classifier* cls = nullptr;
	CRNNRecognizer* rec = nullptr;
	QWidgetList widgetLeftList;
	QWidgetList widgetRightUpList;
	QWidgetList widgetRightDownList;
	QWidgetList widgetMiddleList;

	QSplitter* splitter;
	enum class Role {
		sideBar,
		mainUpBar,
		mainDownBar,
		middleBar
	};
	void addLayout(const QWidgetList& widgetList, Role r);
	void removeLayout(Role r);

	void do_hide_animations();
	void do_show_animations();
	void __initMemory();
	void __initConnection();
	void resizeEvent(QResizeEvent*);


signals:
	void changeLedlist(QVariant&);
	void clicked(const QModelIndex&);
public slots:
	void onTreeItemSelected(const QModelIndex& current, const QModelIndex& previous);
	void onChangeLedlist(QVariant&);

	void on_action_triggered();
	void on_action_2_triggered();

	
	void onTableClicked(const QModelIndex& index);
	void onDataChanged(const QModelIndex& index);
private slots:
	void on_button1_clicked();
	void on_button2_clicked();
	void on_button3_clicked();
	void on_button4_clicked();
	void on_button5_clicked();
	void on_button6_clicked();

	void updateRowLabel(int value);

	void updateColLabel(int value);

	QString columnNumberToLetter(int col);
	void getSpinbox();
private:
    Ui::OCRFastPointClass ui;
	GraphicScene* scene;
	GraphicView* view;
	QTreeView* treeView;
	QTableView* tableView;
	QStandardItemModel model;
	QStandardItem* rootItem;
	QStandardItem* folder11;
	QStandardItem* item;
	GraphicItem* graphicItem;
	QGraphicsPixmapItem* backgroundPixmapItem=nullptr;

	bool m_bSideflag = false;


	bool found = false;
	bool screen = false;
	QList<Building*> buildingList;
	QList<Floor*> floorList;
	QList<Led*> areaLedlist;
	QList<Area*> areaList;
	QList<GraphicItem*> loadedLights;
	QList<QGraphicsItem*> items;
	QList<Led*> ledList;

	QHash<int, Led*> ledHash;  // Key: Led的ID, Value: Led指针

	qreal pixmapWidth;
	qreal pixmapHeight;
	QPushButton* button1;
	QPushButton* button2;
	QPushButton* button3;
	QPushButton* button4;
	QPushButton* button5;
	QPushButton* button6;
	bool hidden_state = false;
	QPropertyAnimation* animation_main; ///< Animation for main content.
	QPropertyAnimation* animation_side; ///< Animation for sidebar content.
	QPropertyAnimation* animation_middle;
	QParallelAnimationGroup* group; ///< Group to synchronize animations.
	void inline setState(bool st) {
		hidden_state = st;
		hidden_state ? do_hide_animations() : do_show_animations();
	}
	LedExcel* ledExcel;

	QLabel* label1;
	QLabel* label2;
	QLabel* label3;
	QSpinBox* spinBox;
	int rowCount;
	int columnCount;
	QDialog* dialog5;

	QVariantList tableList;
	QStandardItemModel* tableModel;
	ApplicationInstance* ocrapp;
};

