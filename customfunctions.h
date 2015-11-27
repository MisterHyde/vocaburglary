#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H
#include<QStringList>
#include<QDebug>
#include<QFile>

namespace CustomFuctions{
QString undoStringList(QStringList);
QString csvToJson(QString pCsvPath);
}

#endif // CUSTOMFUNCTIONS_H
