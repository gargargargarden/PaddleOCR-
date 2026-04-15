#include"LedExcel.h"
#include <windows.h>
LedExcel::LedExcel(QString filename):excel(nullptr),workBooks(nullptr),workBook(nullptr),workSheets(nullptr),workSheet(nullptr),data(nullptr),isOpened(false)
{
    OleInitialize(0);
    this->fileName = filename;
}

LedExcel::~LedExcel()
{
    OleUninitialize();
    release();
    qDebug() << "~ledexcel  deleting......";
}

bool LedExcel::open()
{
    if (fileName.isEmpty())
    {
        //qDebug() << "failed to open xlsx file";
        return false;
    }
    qDebug() << "filename:" << fileName;
    excel = new QAxObject("Excel.Application"); //初始化excel对象
    if (excel == NULL)
    {
        //qDebug() << "failed to create xlsx file";
        return false;
    }
    excel->dynamicCall("SetVisible(bool)", false); //false不显示窗体
    excel->setProperty("DisplayAlerts", false); //不显示警告。
    workBooks = excel->querySubObject("WorkBooks"); //获取全部工作簿对象

    QFile file(fileName);
    if (file.exists())
    {
        qDebug() << "import to all worksheet";
        //导入文件到全部工作簿对象中，并将其设置为当前工作簿
        workBook = workBooks->querySubObject("Open(const QString &)", fileName);
    }
    else
    {
        //文件不存在则创建
        qDebug() << "create worksheet";
        workBooks->dynamicCall("Add");
        workBook = excel->querySubObject("ActiveWorkBook");
    }

    workSheets = workBook->querySubObject("Sheets"); //获得所有工作表对象

    int sheetCount = workSheets->property("Count").toInt();
    this->sheetCount = sheetCount;

    isOpened = true;
    return true;
}

void LedExcel::close()
{
    release();
}

bool LedExcel::save()
{
    qDebug() << "WzExcel: save()";
    //QAxObject* activeWorkBook = workBook->querySubObject();

    if (!isOpened)
    {
        qDebug() << "保存失败，文件没有打开，请先调用open函数";
        return false;
    }

    QFile file(this->fileName);
    if (file.exists())
    {
        for (int i = 0; i < modifiedCells.size(); ++i) {
            QAxObject* data = modifiedCells[i].first;   // 获取 QAxObject*，即 Excel 单元格
            QString value = modifiedCells[i].second;    // 获取单元格的新值

            data->dynamicCall("Value", value);

            qDebug() << "Saving cell with new value:" << value;
        }

        // 清空修改列表
        modifiedCells.clear();

        //文件存在则保存
        //workBook->dynamicCall("Save()",true);
        path= workBook->dynamicCall("FullName()").toString();
        qDebug() << "try to output path" << path;
        //workBook->dynamicCall("SaveCopyAs(QString)",QDir::toNativeSeparators(path));
        workBook->dynamicCall("SaveAs(const QString&)", path);
        //workBook->dynamicCall("Save()",true);
        workBook->dynamicCall("Close()");
        workBooks->dynamicCall("Close()");
        excel->dynamicCall("Quit()");
        
    }
    else
    {
        //文件不存在则另存为
        //this->saveAs(this->fileName);
    }
    return true;
}

bool LedExcel::saveAs()
{
    qDebug() << "WzExcel: saveAs(const QString &fileName)";

    if (!isOpened)
    {
        qDebug() << "另存为失败，文件没有打开，请先调用open函数";
        return false;
    }

    workBook->dynamicCall("SaveAs(const QString &)",
        QDir::toNativeSeparators(fileName));
    return true;
}

bool LedExcel::insertValue(const int& row, const int& column, const QString& value)
{

    if (!isOpened)
    {
        qDebug() << "insert failed !isOpened";
        return false;
    }

    if (workSheet == NULL)
    {
        qDebug() << "insert failed workSheet == NULL";
        return false;
    }
    
    data = workSheet->querySubObject("Cells(int,int)", row, column);
    if (!data->isNull()) {
        modifiedCells.append(QPair<QAxObject*, QString>(data, value));
        qDebug() << "Excel insert:" << value << "x:" << row << "y:" << column;
    }
    
    return true;
}

QVariantList LedExcel::getTable(int sheet)
{

    workSheet = workSheets->querySubObject("Item(int)", sheet);
    QString sheetName = workSheet->property("Name").toString();

    QAxObject* rows = workSheet->querySubObject("Rows");
    QAxObject* columns = workSheet->querySubObject("Columns");

    QString range = generateExcelRange(rowCount, columnCount);
    qDebug() << "new rowCount" << rowCount << "new columnCount" << columnCount;
    QAxObject* allEnvData = workSheet->querySubObject("Range(QString)", range);
    QVariant allEvnDataQVariant = allEnvData->property("Value");
    QVariantList allEnvDataList = allEvnDataQVariant.toList();
    
    return allEnvDataList;


}

QString LedExcel::columnNumberToLetter(int col) {
    QString colStr = "";
    while (col > 0) {
        col--;
        colStr.prepend(QChar('A' + (col % 26)));
        col /= 26;
    }
    return colStr;
}

QString LedExcel::generateExcelRange(int rowCount, int colCount) {
    QString startColumn = "A";  // 起始列是 A
    QString endColumn = columnNumberToLetter(colCount);  // 最后一列的列名

    return startColumn + "1:" + endColumn + QString::number(rowCount);  // 生成范围字符串
}
void LedExcel::release()
{
    isOpened = false;
    if (data != NULL)
    {
        delete data;
        qDebug() << "delete data......";
        data = NULL;
    }
    if (workSheet != NULL)
    {
        delete workSheet;
        qDebug() << "delete workSheet......";
        workSheet = NULL;
    }
    if (workSheets != NULL)
    {
        delete workSheets;
        qDebug() << "delete workSheets......";
        workSheets = NULL;
    }

    if (workBook != NULL)
    {
        workBook->dynamicCall("Close(Boolean)",true);
        qDebug() << "delete workBook......";
        delete workBook;
        workBook = NULL;
    }
    if (workBooks != NULL)
    {
        workBooks->dynamicCall("Close(Boolean)",true);
        qDebug() << "delete workBooks......";
        delete workBooks;
        workBooks = NULL;
    }
    if (excel != NULL)
    {
        excel->dynamicCall("Quit(void)",false);
        qDebug() << "delete excel......";
        delete excel;
        excel = NULL;
    }
}
