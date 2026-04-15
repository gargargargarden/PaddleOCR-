#include "TestTreeview.h"
TestTreeview::TestTreeview(QWidget *parent):QWidget(parent) {

    treeView.setWindowTitle("QTreeView 示例 - 标准模型");
    treeView.resize(400, 300);

    // 创建标准模型

    model.setHorizontalHeaderLabels({ "名称", "类型", "大小" });

    // 添加数据
    rootItem = model.invisibleRootItem();

    // 第一级节点
    folder1 = new QStandardItem("文件夹1");
    folder2 = new QStandardItem("文件夹2");

    // 第二级节点
    file11 = new QStandardItem("文件1.txt");
    file11->setData("文本文件", Qt::UserRole + 1); // 自定义数据
    file11->setData(1024, Qt::UserRole + 2);      // 文件大小

    QStandardItem* file12 = new QStandardItem("文件2.jpg");
    file12->setData("图像文件", Qt::UserRole + 1);
    file12->setData(2048, Qt::UserRole + 2);

    // 构建树结构
    folder1->appendRow(file11);
    folder1->appendRow(file12);
    rootItem->appendRow(folder1);
    rootItem->appendRow(folder2);

    // 设置模型
    treeView.setModel(&model);

    // 展开所有节点
    treeView.expandAll();

    // 显示
    treeView.show();
}

TestTreeview::~TestTreeview()
{
}
