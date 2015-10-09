#ifndef MANAGEDB_H
#define MANAGEDB_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <algorithm>
#include <random>
#include <chrono>

class Managedb
{
 public:
    Managedb();

    bool insertRec(QString in, QString aus, QString commentin, QString commentaus);
    QList<QStringList> getVocs();
    bool updateRecRank(QString in, QString aus, bool right);
    bool updateRecAusland(QString aus, QString in);

 private:
    QSqlDatabase db;
};

#endif // MANAGEDB_H
