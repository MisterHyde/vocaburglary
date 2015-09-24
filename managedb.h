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
    bool updateRec(QString in, QString aus, bool right);

 private:
    QSqlDatabase db;
};

#endif // MANAGEDB_H
