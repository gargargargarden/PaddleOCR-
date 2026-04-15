#include "GraphicScene.h"
#include <fstream>
#include <iostream>
#include <exception>
cv::Mat QImage2cvMat(QImage image);
// 打开日志文件，如果文件不存在则创建
std::ofstream log_file("initialization_log.txt", std::ios::app);
GraphicScene::GraphicScene()
{
    config = new OCRConfig("config.txt");
    config->PrintConfigInfo();
    try {
        // 初始化 DBDetector 对象
        det = new DBDetector(config->det_model_dir, config->use_gpu, config->gpu_id,
            config->gpu_mem, config->cpu_math_library_num_threads,
            config->use_mkldnn, config->max_side_len, config->det_db_thresh,
            config->det_db_box_thresh, config->det_db_unclip_ratio,
            config->visualize, config->use_tensorrt, config->use_fp16);

        // 如果初始化成功，输出信息
        std::cout << "DBDetector initialized successfully!" << std::endl;

    }
    catch (const std::exception& e) {
        // 如果发生异常，输出错误信息
        std::cerr << "Error initializing DBDetector: " << e.what() << std::endl;
    }

    //Classifier *cls = nullptr;
    if (config->use_angle_cls == true) {
        cls = new Classifier(config->cls_model_dir, config->use_gpu, config->gpu_id,
            config->gpu_mem, config->cpu_math_library_num_threads,
            config->use_mkldnn, config->cls_thresh,
            config->use_tensorrt, config->use_fp16);
    }

    rec = new CRNNRecognizer(config->rec_model_dir, config->use_gpu, config->gpu_id,
        config->gpu_mem, config->cpu_math_library_num_threads,
        config->use_mkldnn, config->char_list_file,
        config->use_tensorrt, config->use_fp16);

}

void GraphicScene::setBackgroundImage(const QString& path)
{
    m_background = QPixmap(path);
    if (!m_background.isNull()) {
        setSceneRect(m_background.rect()); // 调整场景大小匹配图片
        update(); // 触发重绘
    }
    LedList = DataHelper::Instance()->GetLedList();
}

void GraphicScene::drawBackground(QPainter* painter, const QRectF& rect)
{

}

void GraphicScene::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mousePressEvent(event);  
}

void GraphicScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    if (doubleDia) {
        QPointF scenePos = event->scenePos();
        QPointF localPos = m_currentItem->mapFromScene(scenePos);  // 转换为相对于m_currentItem的局部坐标


        dialog = new QDialog();
        dialog->setAttribute(Qt::WA_DeleteOnClose);//防止内存泄漏
        dialog->resize(200, 100);
        //dialog->setWindowIcon(QIcon(":new/texture/Images/Sunny.jpg"));
        dialog->setWindowTitle("灯具布点");
        dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        // 设置对话框显示位置
        //dialog->move(event->pos());  // 将对话框移动到鼠标点击的全局位置
        QVBoxLayout* layout = new QVBoxLayout(dialog);

        QLabel* label = new QLabel("请输入灯具编号:", dialog);
        layout->addWidget(label);

        QLineEdit* lineEdit = new QLineEdit(dialog);
        lineEdit->setPlaceholderText("例如: LED-001"); // 占位提示文本
        layout->addWidget(lineEdit);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            dialog
        );
        layout->addWidget(buttonBox);


        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
        connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
            if (lineEdit->text().isEmpty()) {
                QMessageBox::warning(dialog, "警告", "输入不能为空！");
                return; // 阻止对话框关闭
            }
            else {
                Led* led = DataHelper::Instance()->GetLedById(lineEdit->text());
                //QPointF Pos = event->pos();
                if (led!=nullptr) {
                    bool found = false;
                    for (QGraphicsItem* item : scene->items()) {
                        qDebug() << "item  data:" << lineEdit->text();
                        if (!item->data(lineEdit->text().toInt()).toString().isEmpty()) {
                            // 获取当前图形项的中心点
                            QPointF currentCenter = item->boundingRect().center();

                            item->setPos(localPos);
                            DataHelper::Instance()->updateLedPosById(lineEdit->text().toInt(), localPos);
                            found = true;
                            break;
                        }
                    }

                
                    // 如果没有找到对应的图形项
                    if (!found) {
                        qDebug() << "ledId" << led->physId();
                        // 新建一个 item，并更新它的 AreaId、坐标等
                        int phyId = led->physId().toInt();
                        int ledType = led->typeDetail();

                        GraphicItem* newLight = new GraphicItem(ledType,phyId);
                        newLight->setData(0, phyId);  // 设置数据
                        newLight->setData(phyId, ledType);  // 设置 LED 类型
                        
                        newLight->setZValue(1);  // 设置图层级别
                        newLight->setParentItem(m_currentItem);  // 设置父项
                        int floorId = m_currentItem->data(0).toInt();
                        qDebug() << "choose item floorId:" << floorId;
                        

                        QGraphicsTextItem* phtext = new QGraphicsTextItem(QString::number(phyId), newLight);
                        QFont font = phtext->font();
                        font.setPointSize(50);
                        font.setBold(true);
                        phtext->setFont(font);
                        phtext->setPos(0, newLight->boundingRect().height());
                        phtext->setZValue(1);

                        

                        DataHelper::Instance()->updateLedAreaId(phyId, floorId);
                        // 更新 LED 坐标
                        DataHelper::Instance()->updateLedPosById(lineEdit->text().toInt(), localPos);
                        found = true;
                        newLight->setPos(localPos);  // 设置位置
                        scene->addItem(newLight);
                    }

                
                }
                else {

                    QMessageBox::warning(dialog, "警告", "灯具编号不存在！");
                }
            }
            dialog->accept(); // 输入有效时才关闭
            });

        dialog->exec();
        QGraphicsScene::mouseReleaseEvent(event);
    }
    else {

        ScreenWidget* sw = new ScreenWidget;
        
        connect(sw, SIGNAL(sig_SelectImg(QImage)), this, SLOT(slt_SelectImg(QImage)), Qt::QueuedConnection);
        QPointF scenePos = event->scenePos();
        QPointF localPos = m_currentItem->mapFromScene(scenePos);  // 转换为相对于m_currentItem的局部坐标
        

        dialog = new QDialog();
        dialog->setAttribute(Qt::WA_DeleteOnClose);//防止内存泄漏
        dialog->resize(200, 100);
        //dialog->setWindowIcon(QIcon(":new/texture/Images/Sunny.jpg"));
        dialog->setWindowTitle("灯具布点");
        dialog->setWindowFlag(Qt::WindowContextHelpButtonHint, false);
        QVBoxLayout* layout = new QVBoxLayout(dialog);

        QLabel* label = new QLabel("请输入灯具编号:", dialog);
        layout->addWidget(label);

        lineEdit = new QLineEdit(dialog);
        lineEdit->setPlaceholderText("例如: LED-001"); // 占位提示文本
        layout->addWidget(lineEdit);

        QDialogButtonBox* buttonBox = new QDialogButtonBox(
            QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
            dialog
        );
        layout->addWidget(buttonBox);


        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
        connect(buttonBox, &QDialogButtonBox::accepted, [=]() {
            if (lineEdit->text().isEmpty()) {
                QMessageBox::warning(dialog, "警告", "输入不能为空！");
                return; // 阻止对话框关闭
            }
            else {
                Led* led = DataHelper::Instance()->GetLedById(lineEdit->text());
                //QPointF Pos = event->pos();
                if (led != nullptr) {
                    bool found = false;
                    for (QGraphicsItem* item : scene->items()) {
                        if (!item->data(lineEdit->text().toInt()).toString().isEmpty()) {
                            // 获取当前图形项的中心点
                            QPointF currentCenter = item->boundingRect().center();

                            item->setPos(localPos);
                            DataHelper::Instance()->updateLedPosById(lineEdit->text().toInt(), localPos);
                            found = true;
                            break;
                        }
                    }


                    // 如果没有找到对应的图形项
                    if (!found) {
                        qDebug() << "ledId" << led->physId();
                        // 新建一个 item，并更新它的 AreaId、坐标等
                        int phyId = led->physId().toInt();
                        int ledType = led->typeDetail();

                        GraphicItem* newLight = new GraphicItem(ledType, phyId);
                        newLight->setData(0, phyId);  // 设置数据
                        newLight->setData(phyId, ledType);  // 设置 LED 类型
                         // 设置位置
                        newLight->setZValue(1);  // 设置图层级别
                        newLight->setParentItem(m_currentItem);  // 设置父项
                        int floorId = m_currentItem->data(0).toInt();
                        qDebug() << "choose item floorId:" << floorId;

                        QGraphicsTextItem* phtext = new QGraphicsTextItem(QString::number(phyId), newLight);
                        QFont font = phtext->font();
                        font.setPointSize(50);
                        font.setBold(true);
                        phtext->setFont(font);
                        phtext->setPos(0,newLight->boundingRect().height());
                        phtext->setZValue(1);

                        DataHelper::Instance()->updateLedAreaId(phyId, floorId);
                        // 更新 LED 坐标
                        DataHelper::Instance()->updateLedPosById(lineEdit->text().toInt(), localPos);
                        found = true;
                        newLight->setPos(localPos);
                        scene->addItem(newLight);
                    }
                }
                else {
                    QMessageBox::warning(dialog, "警告", "灯具编号不存在！");
                }
            }
            dialog->accept(); // 输入有效时才关闭
            });
        sw->showFullScreen();
        
        QGraphicsScene::mouseReleaseEvent(event);
    }
    
}
std::string GraphicScene::qstr2str(const QString qstr)
{
    QByteArray cdata = qstr.toLocal8Bit();
    return std::string(cdata);
}
void GraphicScene::getRect(qreal width, qreal height) {
    getScene();
    scene = this;
    //scene->items();
    foreach(QGraphicsItem * item, scene->items()) {
        QGraphicsItem* parent = item->parentItem();  // 获取父项
        if (parent) {
            //qDebug() << "Item:" << item << " has parent:" << parent;
        }
        else {
            qDebug() << "Item:" << item << " has no parent.";
            m_currentItem = item;
            setBgImg = new GraphicItem();
            setBgImg->setBgImgItem(item);
        }
    }
     pixmapWidth = width/2;
     pixmapHeight = height/2;
}

void GraphicScene::slt_SelectImg(QImage img) {
    if (img.isNull()) {
        return;
    }
    QImage im = img.copy();
    ScreenWidget* sw = qobject_cast<ScreenWidget*>(sender());//获取发射信号的对象
    delete sw;
    sw = nullptr;

    cv::Mat srcimg = QImage2cvMat(im);
    copyMakeBorder(srcimg, srcimg, 100, 100, 100, 100, BORDER_CONSTANT, Scalar(255, 255, 255));

    std::vector<std::vector<std::vector<int>>> boxes;
    det->Run(srcimg, boxes);

    std::vector<std::string> str_res;
    rec->Run(boxes, srcimg, cls, str_res);

    QString newstr=convertToQString(str_res);
    lineEdit->setText(newstr);
    dialog->exec();
}

cv::Mat QImage2cvMat(QImage image)
{
    cv::Mat mat;
    //qDebug() << image.format();
    switch (image.format())
    {
    case QImage::Format_ARGB32:
    case QImage::Format_RGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat mat_temp = cv::Mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cvtColor(mat_temp, mat, cv::COLOR_BGRA2BGR);
    }
    break;
    case QImage::Format_RGB888:
        mat = cv::Mat(image.height(), image.width(), CV_8UC3, (void*)image.constBits(), image.bytesPerLine());
        break;
    case QImage::Format_Indexed8:
        mat = cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine());
        break;
    }
    return mat;
}
