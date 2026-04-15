#include "OCRFastPoint.h"
#include<QFileDialog>
#include<QDebug>
namespace SideBarUtilsTools {
    void clearLayout(QLayout* layout) {
        if (!layout)
            return;

        QLayoutItem* item;
        while ((item = layout->takeAt(0)) != nullptr) {
            if (item->widget()) {
                item->widget()->hide();
            }
            else {
                clearLayout(item->layout());
            }
        }
    }
}
OCRFastPoint::OCRFastPoint(ApplicationInstance* app , QWidget *parent)
    : QMainWindow(parent)
{
    ocrapp = app;
    DataHelper::InitInstance();
    
    ui.setupUi(this);
    Setting::setGlobalVariablesToSettings();
    Setting::writeToFileInAppDirectory();

    ui.leftwidget->setFixedWidth(220);
    ui.upwidget->setFixedHeight(50);
    //初始化控件,添加到widgetlist中

    splitter = new QSplitter(Qt::Vertical, ui.downwidget);
    
    treeView = new QTreeView();
    treeView->setFixedWidth(220);
    tableView = new QTableView(splitter);
    tableView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    // 启用双击编辑功能
    tableView->setEditTriggers(QAbstractItemView::DoubleClicked);
    // 清除任何选中的单元格
    tableView->clearSelection();
    // 禁用表头编辑
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    tableView->verticalHeader()->setSectionResizeMode(QHeaderView::Interactive);



    treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    view = new GraphicView(splitter);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->addWidget(tableView);
    splitter->addWidget(view);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    button1 = new QPushButton("侧边栏");
    button2 = new QPushButton("截图识别");
    //button3 = new QPushButton("修改图标大小");
    button4 = new QPushButton("打开表格文件");
    button5 = new QPushButton("调整表格可视范围");
    button6 = new QPushButton("保存表格并退出");

    button5->setEnabled(false);
    button6->setEnabled(false);

    widgetLeftList.append(treeView);

    widgetRightUpList.append(button1);
    widgetRightUpList.append(button2);
    //widgetRightUpList.append(button3);
    widgetRightUpList.append(button4);
    widgetRightUpList.append(button5);
    widgetRightUpList.append(button6);

    widgetRightDownList.append(tableView);
    widgetRightDownList.append(view);
    



    addLayout(widgetLeftList, Role::sideBar);
    addLayout(widgetRightUpList, Role::mainUpBar);
    addLayout(widgetRightDownList, Role::mainDownBar);
    

    QSize viewSize = view->size();
    scene=new GraphicScene();
    scene->setBackgroundBrush(Qt::gray);

    view->setScene(scene);
    view->show();
    InitTreeView();
    //connect(ui.treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &OCRFastPoint::onTreeItemSelected);
    connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, &OCRFastPoint::onTreeItemSelected);
    connect(this, &OCRFastPoint::changeLedlist, this, &OCRFastPoint::onChangeLedlist);

    __initMemory();
    __initConnection();
    connect(button1, &QPushButton::clicked, this, &OCRFastPoint::on_button1_clicked);
    connect(button2, &QPushButton::clicked, this, &OCRFastPoint::on_button2_clicked);
    //connect(button3, &QPushButton::clicked, this, &OCRFastPoint::on_button3_clicked);
    connect(button4, &QPushButton::clicked, this, &OCRFastPoint::on_button4_clicked);
    connect(button5, &QPushButton::clicked, this, &OCRFastPoint::on_button5_clicked);
    connect(button6, &QPushButton::clicked, this, &OCRFastPoint::on_button6_clicked);
}


OCRFastPoint::~OCRFastPoint()
{
    qDebug() << "deleting ocr.....";
    delete ledExcel;
    delete ocrapp;
    delete &ui;
}

void OCRFastPoint::InitTreeView()
{


    // 创建标准模型
    // 禁用所有编辑功能
    /*ui.treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui.treeView->header()->hide();*/
    treeView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    treeView->header()->hide();
    // 添加数据

    buildingList.clear();
    floorList.clear();
    model.clear();
    rootItem = model.invisibleRootItem();

    buildingList = DataHelper::Instance()->GetBuildingList();
    floorList = DataHelper::Instance()->GetFloorList();
    for (int i = 0; i < buildingList.size(); ++i) {
        QString name = buildingList[i]->getName(); 
        QStandardItem* folder1 = new QStandardItem(name);
        
         for (int j = 0; j < floorList.size(); ++j) {
             if (floorList[j]->buildingId() == buildingList[i]->Id()) {
                 QString name = floorList[j]->getName();
                 folder11 = new QStandardItem(name);
                 folder11->setData(floorList[j]->id(), Qt::UserRole+1);
                 folder1->appendRow(folder11);
             }   
         }
         // 将 folder1 添加到模型或父项
         rootItem->appendRow(folder1);
    }


    treeView->setModel(&model);
    treeView->expandAll();
    treeView->show();
}

void OCRFastPoint::addLayout(const QWidgetList& widgetList, Role r)
{
    switch (r) {
    case Role::sideBar:

        ui.leftwidget->setLayout(new QVBoxLayout(ui.leftwidget));
        for (auto& w : widgetList) {
            ui.leftwidget->layout()->addWidget(w);
        }

        break;
    case Role::mainUpBar:
        ui.upwidget->setLayout(new QHBoxLayout(ui.upwidget));
        for (auto& w : widgetList) {
            ui.upwidget->layout()->addWidget(w);
        }
        break;

    case Role::mainDownBar:
        ui.downwidget->setLayout(new QVBoxLayout(ui.downwidget));
        for (auto& w : widgetList) {
            ui.downwidget->layout()->addWidget(w);
        }

        //ui.downwidget->layout()->addWidget(splitter);
        splitter->setLayout(ui.downwidget->layout());
        break;

    }
}

void OCRFastPoint::removeLayout(Role r)
{
    switch (r) {
    case Role::sideBar:
        SideBarUtilsTools::clearLayout(ui.leftwidget->layout());
        break;
    case Role::mainUpBar:
        SideBarUtilsTools::clearLayout(ui.rightwidget->layout());
        break;

    case Role::mainDownBar:
        SideBarUtilsTools::clearLayout(ui.upwidget->layout());
        break;
}
}
void OCRFastPoint::resizeEvent(QResizeEvent*) {
    int w = width();
    int h = height();

    int sideWidth = ui.leftwidget->width();
    int sideHeight = h;
    int mainHeight = h;

    if (hidden_state) {
        ui.leftwidget->setGeometry(-sideWidth, 0, sideWidth, sideHeight);
        ui.rightwidget->setGeometry(0, 0, w, mainHeight);
    }
    else {
        ui.leftwidget->setGeometry(0, 0, sideWidth, sideHeight);
        ui.rightwidget->setGeometry(sideWidth, 0, w - sideWidth, mainHeight);
    }
}
void OCRFastPoint::do_hide_animations()
{
    animation_side->setStartValue(ui.leftwidget->geometry());
    /* move to the hidden place */
    animation_side->setEndValue(
        QRect(-ui.leftwidget->width(), ui.leftwidget->y(),
            ui.leftwidget->width(), ui.leftwidget->height()));

    animation_main->setStartValue(ui.rightwidget->geometry());
    animation_main->setEndValue(QRect(
        0, ui.rightwidget->y(),
        width(), ui.rightwidget->height()));

    group->start();
}

void OCRFastPoint::do_show_animations()
{
    animation_side->setStartValue(ui.leftwidget->geometry());
    /* move to the hidden place */
    animation_side->setEndValue(QRect(0, ui.leftwidget->y(),
        ui.leftwidget->width(),
        ui.leftwidget->height()));

    animation_main->setStartValue(ui.rightwidget->geometry());
    animation_main->setEndValue(
        QRect(ui.leftwidget->width() + 0,
            ui.rightwidget->y(),
            width() - 0 - ui.leftwidget->width(),
            ui.rightwidget->height()));
    /*animation_side->setStartValue(ui.middlewidget->geometry());
    animation_main->setEndValue(
        QRect(ui.leftwidget->width() + 0,
            ui.middlewidget->y(),
            width() - 0 - ui.leftwidget->width(),
            ui.middlewidget->height()));*/
    ui.rightwidget->setVisible(true);
    group->start();
}

void OCRFastPoint::onChangeLedlist(QVariant& value)
{
    int floorid = value.toInt();
    areaList = DataHelper::Instance()->GetAreaList();
    for (int i = 0; i < areaList.length(); i++) { //有空换成base-for写法，即const Led& led:areaLedlist
        // 根据floorid查找areaid，可能多个areaList[i]->id()
        if (floorid == areaList[i]->floorId()) {
            ledList = DataHelper::Instance()->GetLedlistByAreaId(areaList[i]->floorId());
            // 
            areaLedlist.append(ledList); 
            ledList.clear();
        }
    }
    for (int i = 0; i < areaLedlist.length(); i++) {

        int ledType = areaLedlist[i]->typeDetail();
        int phyId = areaLedlist[i]->physId().toInt();
        if (phyId == 893848) {
            qDebug() << "893848-----------------------";
        }
        for (int i = 0; i < areaLedlist.length(); i++) {
            qDebug() << "floorid:"<<floorid << areaLedlist[i]->physId();
        }
        QPointF point(areaLedlist[i]->x(), areaLedlist[i]->y());
        GraphicItem* light = new GraphicItem(ledType, phyId);
        light->setData(0, phyId);
        light->setData(phyId, ledType);
        light->setPos(point);
        light->setZValue(1);
        light->setParentItem(backgroundPixmapItem);

        QGraphicsTextItem* phtext = new QGraphicsTextItem(QString::number(phyId),light);
        QFont font = phtext->font();
        font.setPointSize(50); // 设置字体大小
        font.setBold(true);     // 设置加粗
        phtext->setFont(font);

        phtext->setPos(0, light->boundingRect().height());
        phtext->setZValue(1);

        scene->addItem(light);
    }
    scene->getRect(pixmapWidth,pixmapHeight);
    // 填充哈希表
    //for (Led* led : areaLedlist) {
    //    ledHash.insert(led->physId().toInt(), led);
    //}

    // 查找
    Led* foundLed = ledHash.value(1, nullptr);


    areaLedlist.clear();
    //ui.graphicsView->update();
    view->update();
}



void OCRFastPoint::onTreeItemSelected(const QModelIndex& current, const QModelIndex& previous) {
    Q_UNUSED(previous);
    qDebug() << "当前的选中为：" << current.data(2).toString();
    item = model.itemFromIndex(current);
    int csfloorId = item->data(Qt::UserRole + 1).toInt();
    qDebug() << "item" << item->data(Qt::UserRole + 1).toInt();
    QVariant variantValue = QVariant(csfloorId);
    scene->clear();

    // 查询floorId，来获取ImageBytes
    for (int j = 0; j < floorList.size(); ++j) {
        if (floorList[j]->id() == csfloorId) {
            // 发射信号
            QByteArray imageArray = floorList[j]->imageBytes();
            QString imageType = floorList[j]->imageType();
            QPixmap pixmap;
            if (pixmap.loadFromData(imageArray, imageType.toUtf8())) {


                backgroundPixmapItem = new QGraphicsPixmapItem(pixmap);
                backgroundPixmapItem->setZValue(-1);
                backgroundPixmapItem->setFlag(QGraphicsItem::ItemIsMovable, false);
                backgroundPixmapItem->setData(0, csfloorId);
                QRectF sceneRect = scene->sceneRect();

                qreal centerX = sceneRect.center().x();
                qreal centerY = sceneRect.center().y();

                pixmapWidth = pixmap.width();
                pixmapHeight = pixmap.height();

                // 计算将图片放置在场景中心的位置，使其中心对齐
                qreal xPos = centerX - pixmapWidth / 2;
                qreal yPos = centerY - pixmapHeight / 2;
                qDebug() << "新的中心点" << xPos << yPos;

                
                // 设置背景图片的位置
                backgroundPixmapItem->setPos(xPos, yPos);
                scene->addItem(backgroundPixmapItem);
                emit changeLedlist(variantValue);
                scene->update();
                //ui.graphicsView->setScene(scene);
                view->setScene(scene);
                
                //ui.graphicsView->show();
                view->show();
            }

        }

    }
}
void OCRFastPoint::on_action_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开工程"),
        "D:/",
        tr("Database files (*.db)"));

    // 检查是否选择了文件
    if (fileName.isEmpty()) {
        // 如果没有选择文件，显示警告
        QMessageBox::warning(this, "Warning!", "Failed to open the database file!");
    }
    DataHelper::Instance()->setFilename(fileName);
    DataHelper::Instance()->loadBuildingList();
    DataHelper::Instance()->loadFloorList();
    DataHelper::Instance()->loadLedList();
    DataHelper::Instance()->loadAreaList();
    InitTreeView();

}
void OCRFastPoint::on_action_2_triggered()
{
    QDialog* dialog = new QDialog(this);
    //当主窗口不关闭，即不进行析构时，多次打开关闭对话框会导致内存泄漏
    dialog->setAttribute(Qt::WA_DeleteOnClose);//防止内存泄漏
    dialog->resize(300, 200);
    //dialog->setWindowIcon(QIcon(":new/texture/Images/Sunny.jpg"));
    dialog->setWindowTitle("说明");
    QLabel* label = new QLabel("1.截图识别开：双击点位在图纸上截图识别灯具编码，cpu性能越好，识别越准确，越快", dialog);
    QLabel* label2 = new QLabel("2.截图识别关：没有识别功能，双击点位手动输入灯具编码进行布点", dialog);
    QLabel* label3 = new QLabel("3.打开表格文件，调整显示范围后表格才会显示，读取表格需要耐心等待7-8s", dialog);
    QLabel* label4 = new QLabel("4.工作页一页一保存", dialog);
    label->setWordWrap(true);  // 允许文字换行
    label2->setWordWrap(true);
    label3->setWordWrap(true);
    label4->setWordWrap(true);
    // 创建布局，将 QLabel 添加到对话框中
    QVBoxLayout* dialogLayout = new QVBoxLayout(dialog);
    dialogLayout->addWidget(label);
    dialogLayout->addWidget(label2);
    dialogLayout->addWidget(label3);
    dialogLayout->addWidget(label4);

    // 设置对话框布局
    dialog->setLayout(dialogLayout);
    dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    dialog->exec();

}

void OCRFastPoint::onTableClicked(const QModelIndex& index)
{
    if (index.isValid()) {
        QString cellText = index.data().toString();
        int x = index.row();
        int y = index.column();
        qDebug() << "celltext:" << cellText;
        ledExcel->insertValue(x,y, cellText);
        
    }
}
void OCRFastPoint::onDataChanged(const QModelIndex& index)
{
    if (index.isValid()) {
        QString cellText = index.data().toString();
        int x = index.row()+1;
        int y = index.column()+1;
        qDebug() << "onDataChanged:" << cellText;
        
        //if (x != 0 && y != 0 && !cellText.isEmpty()) {
            // 执行插入操作
            qDebug() << "Inserting value at (" << x << "," << y << "):" << cellText;
            ledExcel->insertValue(x, y, cellText);
   /*     }
        else {
            qDebug() << "Invalid coordinates or empty cell data. Skipping insert.";
        }*/

    }
}
void OCRFastPoint::on_button2_clicked()
{
    // 默认关闭截图功能

    if (screen) {
        //ui.pushButton_3->setEnabled(true);
        button2->setText("截图识别开");
        scene->setDoubleDialog(true);
        screen = false;
        qDebug() << "截图识别开";
    }
    else {
        //ui.pushButton_3->setEnabled(false);
        button2->setText("截图识别关");
        scene->setDoubleDialog(false);
        screen = true;
        qDebug() << "截图识别关";
    }
}
void OCRFastPoint::on_button3_clicked()
{

}

void OCRFastPoint::on_button4_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开表格文件"),
        "D:/",
        tr("Database files (*.csv *.xlsx);;All files (*.*)"));

    // 检查是否选择了文件
    if (fileName.isEmpty()) {
        
    }
   

    QFileInfo fileInfo(fileName);
    ledExcel = new LedExcel(fileName);
    ledExcel->setRowCount(200);
    ledExcel->setColumnCount(50);
    if (ledExcel->open()) {
        button5->setEnabled(true);
        button6->setEnabled(true);
    }
    

}

void OCRFastPoint::on_button5_clicked()
{
    dialog5 = new QDialog();
    spinBox = new QSpinBox();
    QSlider* rowSlider = new QSlider();
    QSlider* colSlider = new QSlider();
    QPushButton* button = new QPushButton();
    button->setText("确定");
    int spinmax=ledExcel->getSheetCount();
    spinBox->setMinimum(1);       
    spinBox->setMaximum(spinmax);     
    spinBox->setSingleStep(1);    
    spinBox->setValue(1);       
    label1 = new QLabel();
    label2 = new QLabel();
    label3 = new QLabel();
    rowSlider = new QSlider(Qt::Horizontal, this);
    rowSlider->setRange(1, 200); // 假设最大行数为 100
    rowSlider->setValue(1);

    
    colSlider = new QSlider(Qt::Horizontal, this);
    colSlider->setRange(1, 50); // 假设最大列数为 30
    colSlider->setValue(1);

    label1->setText("设置显示的行数");
    label2->setText("设置显示的列数");
    label3->setText("工作页");

    // 布局
    QVBoxLayout* layout = new QVBoxLayout(dialog5);
    layout->addWidget(label3);
    layout->addWidget(spinBox);
    layout->addWidget(label1);
    layout->addWidget(rowSlider);
    layout->addWidget(label2);
    layout->addWidget(colSlider);
    layout->addWidget(button);
    setLayout(layout);
    connect(rowSlider, &QSlider::valueChanged, this, &OCRFastPoint::updateRowLabel);
    connect(colSlider, &QSlider::valueChanged, this, &OCRFastPoint::updateColLabel);
    connect(button, &QPushButton::clicked, this, &OCRFastPoint::getSpinbox);
    
    dialog5->exec();
}

void OCRFastPoint::on_button6_clicked()
{

    qDebug() << "saving......";
    ledExcel->save();
    //QCoreApplication::quit();
    ocrapp->exit();
}


void OCRFastPoint:: updateRowLabel(int value) {
    label1->setText("显示的行数: " + QString::number(value));  // 更新行数显示
    ledExcel->setRowCount(value);

}

void OCRFastPoint::updateColLabel(int value) {
    label2->setText("显示的列数: " + QString::number(value)+"  "+columnNumberToLetter(value));  // 更新列数显示
    ledExcel->setColumnCount(value);
}

 QString OCRFastPoint::columnNumberToLetter(int col) {
    QString colStr = "";
    while (col > 0) {
        col--;
        colStr.prepend(QChar('A' + (col % 26)));
        col /= 26;
    }
    return colStr;
}

 void OCRFastPoint::getSpinbox()
 {
     int sheet = spinBox->value();
     tableList = ledExcel->getTable(sheet);
     dialog5->close();
     tableModel = new QStandardItemModel(rowCount, columnCount, tableView);



     

     for (int i = 0; i < tableList.length(); i++)
         {
         
             QVariant allEnvDataList_i = tableList[i];
             if (allEnvDataList_i.canConvert<QVariantList>()) {
                 QVariantList innerList = allEnvDataList_i.toList();
                 for (int j = 0; j < innerList.length(); j++) {
                     QVariant cellData = innerList[j];
                     QStandardItem* item = new QStandardItem(cellData.toString());
                     tableModel->setItem(i, j, item);
                 }
             }
             else {
                 qDebug() << "第" << i + 1 << "个元素的内容：" << allEnvDataList_i.toString();
             }
                 
         }
     connect(tableModel, &QAbstractItemModel::dataChanged, this, &OCRFastPoint::onDataChanged);
     tableView->setModel(tableModel);
     tableView->resizeColumnsToContents();  
     tableView->show();
 }

 



void OCRFastPoint::on_button1_clicked()
{
    
    setState(!hidden_state);
    if (hidden_state) {
        button1->setText("侧边栏开");
    }
    else {
        button1->setText("侧边栏关");   
    }

}
void OCRFastPoint::__initMemory() {
    animation_main = new QPropertyAnimation(ui.rightwidget, "geometry");
    animation_main->setDuration(100);
    animation_main->setEasingCurve(QEasingCurve::InOutQuad);
    animation_side = new QPropertyAnimation(ui.leftwidget, "geometry");
    animation_side->setDuration(100);
    animation_side->setEasingCurve(QEasingCurve::InOutQuad);

    group = new QParallelAnimationGroup(this);
    group->addAnimation(animation_main);
    group->addAnimation(animation_side);
}

void OCRFastPoint::__initConnection() {
    connect(group, &QParallelAnimationGroup::finished, this, [this] {
        ui.leftwidget->setVisible(!hidden_state);
        // have no better idea :(, to update the layout
        resize(size().width() + 1, size().height() + 1);
        resize(size().width() - 1, size().height() - 1);
        });
}
