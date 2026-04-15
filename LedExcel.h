#pragma once
#include<QAxObject>
#include<QFile>
#include <QDir>
#include <QDebug>

class LedExcel {

public:
	LedExcel(QString filename);
	~LedExcel();

	int getSheetCount() {
		return this->sheetCount;
	}
	void setRowCount(int row) {
		this->rowCount = row;
	}
	void setColumnCount(int column) {
		this->columnCount = column;
	}
	void setSheet(int sheet) {
		this->sheet = sheet;
	}
	QString columnNumberToLetter(int col);
	QString generateExcelRange(int rowCount, int colCount);
	bool open();
	void close();
	bool save();
	bool saveAs();
	bool setVisible();
	bool setCurrentWorkSheet();
	bool createWorkSheet();
	bool deleteWorkSheet();
	QString getValue(const int& row, const int& column);
	bool insertValue(const int& row, const int& colum, const QString& value);
	QVariantList getTable(int sheet);

private:
	void release();
private:
	QString fileName; //文件名 

	bool isOpened;
	QAxObject* excel;
	QAxObject* workBooks;
	QAxObject* workBook;
	QAxObject* workSheets;
	QAxObject* workSheet;
	QAxObject* data;

	int sheetCount;
	int sheet;
	int rowCount;
	int columnCount;
	QString path ;

	// 存储需要保存数据的单元格位置
	QList<QPair<QAxObject*, QString>> modifiedCells;
};