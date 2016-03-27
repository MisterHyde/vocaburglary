#ifndef MANAGEDB_H
#define MANAGEDB_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QFileDialog>
#include <algorithm>
#include <random>
#include <chrono>
#include <iterator>

namespace Ui {
class Managedb;
}

class Managedb : public QWidget
{
 public:
    // Functions to work with a SQL database
    Managedb();
    bool insertRec(QString in, QString aus, QString commentin, QString commentaus);
    QList<QStringList> getVocs();
    bool updateRank(QString in, QString aus, bool right);
    bool updateRecAusland(QString newText, QString oldIn, QString oldOut, int which);
    int dbToJson();
    int jsonToDb();

 private:
    QSqlDatabase db;
    const QString tableOneName;
    const QString tableTwoName;
};

#endif // MANAGEDB_H

/**
 * CREATE TABLE vocabulary(
 *    inland varchar(40),
 *	  ausland varchar(40),
 *	  commentin varchar(40),
 *	  commentaus varchar(40),
 *	  rightt integer,
 *	  wrong integer,
 * 	  rank integer,
 *    CONSTRAINT pk_vocabulary Primary Key (ausland),
 *    CONSTRAINT u_oid Unique (oid),
 *    CONSTRAINT u_vocabulary Unique(ausland)
 * )WITH OIDS;
 *
 * CREATE TABLE irregular(
 *      infinitive varchar(40),
 *      I varchar(40),
 *      you varchar(40),
 *  	she varchar(40),
 *  	we varchar(40),
 *  	you2 varchar(40),
 *  	they varchar(40),
 *  	CONSTRAINT fk_vocabulary Foreign Key(oid)  REFERENCES vocabulary(oid)
 * )WITH OIDS;
 */
