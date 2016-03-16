#ifndef MANAGEDJSONDB_H
#define MANAGEDJSONDB_H
#include <QList>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <algorithm>
#include <random>
#include <chrono>
#include <iterator>

class ManagedJsonDb : public QWidget
{
public:
    ManagedJsonDb();
    ~ManagedJsonDb();
    bool insertRec(QString in, QString aus, QString commentin, QString commentaus);
    QList<QStringList> getVocs();
    bool updateRank(QString in, QString aus, bool right);
    bool updateRecAusland(QString aus, QString in);
    int dbToJson();
    int jsonToDb();

private:
    const QString pathToJson;
    QFile *database;
    QList<QStringList> records;
};

#endif // MANAGEDJSONDB_H
