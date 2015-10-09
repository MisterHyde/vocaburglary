#include "managedb.h"

Managedb::Managedb()
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("dictionary");
    db.setUserName("felix");
    db.setPassword("Supp3nG3!L");

    db.open();
}

// The first two parameters specifing the primary key
// The third specify if the the attribute rightt or wrong should be incremented
bool Managedb::updateRecRank(QString in, QString aus, bool right)
{
    QString queryString;
    QVariant qvIn(in);
    QVariant qvAus(aus);

    if(right){
        queryString = "UPDATE words SET rightt = rightt + 1 where inland = :in AND ausland = :aus;";
    }
    else{
        queryString = "UPDATE words SET wrong = wrong + 1 where inland = :in AND ausland = :aus;";
    }

    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(QString(":in"), qvIn);
    query.bindValue(QString(":aus"), qvAus);
    query.exec();

    return query.isActive();
}

bool Managedb::insertRec(QString in, QString aus, QString commentin, QString commentaus)
{
    QVariant qvIn(in);
    QVariant qvAus(aus);
    QVariant qvComIn(commentin);
    QVariant qvComAus(commentaus);

    QSqlQuery query;
    query.prepare("INSERT INTO words (inland, ausland, commentin, commentaus, rightt, wrong, rank) "
                  "VALUES (:inland, :ausland, :commentin, :commentaus, 0, 0 ,0)");
    query.bindValue(QString(":inland"), QVariant(qvIn));
    query.bindValue(QString(":ausland"), QVariant(qvAus));
    query.bindValue(QString(":commentin"), QVariant(qvComIn));
    query.bindValue(QString(":commentaus"), QVariant(qvComAus));
    query.exec();

    qDebug() << "insertRec: " << in << aus << commentin << commentaus;

    return query.isActive();
}

QList<QStringList> Managedb::getVocs()
{
    QStringList inland;
    QStringList ausland;
    QStringList commentin;
    QStringList commentaus;
    QStringList right;
    QList<QStringList> records;
    QList<int> intList;
    int listCount;

    QSqlQuery query("SELECT inland,ausland,commentin,commentaus,rightt FROM words;");

    while(query.next()) {
        inland.append(query.value(0).toString());
        ausland.append(query.value(1).toString());
        commentin.append(query.value(2).toString());
        commentaus.append(query.value(3).toString());
        right.append(query.value(4).toString());
    }

    // If the two columns "inland" and "ausland" contains a different amount of items: return false
    listCount = inland.count();
    if(listCount != ausland.count())
        return records;

    // The range of the list goes from 0 to listCount-1
    for(int i=0; i<listCount; i++){
        intList.append(i);
    }

    qDebug() << "listCount(managedb): " << listCount;

    // Shuffle all lists
    std::random_shuffle(intList.begin(),intList.end());
    QStringList inbuff = inland;
    QStringList ausbuff = ausland;
    QStringList incombuff = commentin;
    QStringList auscombuff = commentaus;
    inland.clear();
    ausland.clear();
    commentin.clear();
    commentaus.clear();

    for(int i=0; i<listCount; i++){
        inland.insert(i, inbuff.at(intList.at(i)));
        ausland.insert(i, ausbuff.at(intList.at(i)));
        commentin.insert(i, incombuff.at(intList.at(i)));
        commentaus.insert(i, auscombuff.at(intList.at(i)));
    }

    // Save all columns in one list
    records << inland << ausland << commentin << commentaus;

    return records;
}

bool Managedb::updateRecAusland(QString aus, QString in)
{
    QString bubl = "UPDATE words SET ausland='" + aus + "' where inland='" + in + "';";
    qDebug() << bubl;
    QSqlQuery query(bubl);
    query.exec();

    return query.isActive();
}
