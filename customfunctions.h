#ifndef CUSTOMFUNCTIONS_H
#define CUSTOMFUNCTIONS_H

#include <QStringList>
#include <QDebug>
#include <QFile>
#include <QXmlStreamAttribute>
#include <QXmlStreamReader>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

namespace CustomFuctions{
QString undoStringList(QStringList);
QString csvToJson(QString pCsvPath);
void parseXml(QString path);
}

#endif // CUSTOMFUNCTIONS_H
