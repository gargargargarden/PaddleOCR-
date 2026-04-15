#pragma once
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include<QWidget>
class TestTreeview : public QTreeView ,QWidget
{
	Q_OBJECT
public:
	TestTreeview(QWidget *parent = nullptr);
	~TestTreeview();
	QTreeView treeView;
	QStandardItemModel model;
	QStandardItem* rootItem;
	QStandardItem* folder1;
	QStandardItem* folder2;
	QStandardItem* file11;
	QStandardItem* file12;
};

