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
 *	  rightt boolean,
 *	  wrong boolean,
 * 	  rank integer,
 *    CONSTRAINT pk_vocabulary Primary Key (ausland),
 *    CONSTRAINT u_oid Unique (oid);
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
 *  	CONSTRAINT fk_vocabulary Foreign Key oid  REFERENCES vocabulary(oid)
 * )WITH OIDS;
 */
