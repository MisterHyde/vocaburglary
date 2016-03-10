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
#include <QTableWidget>
#include <QHeaderView>

namespace CustomFunctions{
    QString undoStringList(QStringList);
    QString csvToJson(QString pCsvPath);
    void parseXml(QString path);
    QSize myGetQTableWidgetSize(QTableWidget *tw);
}

#endif // CUSTOMFUNCTIONS_H
