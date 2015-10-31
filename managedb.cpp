#include "managedb.h"

/**
 * @brief Managedb::Managedb
 */

Managedb::Managedb()
    :tableOneName("vocabulary"), tableTwoName("irregular")
{
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("englishdic");
    db.setUserName("felix");
    db.setPassword("Augsburg13");
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
        queryString = "UPDATE " + tableOneName + " SET rightt = rightt + 1 where inland = :in AND ausland = :aus;";
    }
    else{
        queryString = "UPDATE " + tableOneName + " vocabulary SET wrong = wrong + 1 where inland = :in AND ausland = :aus;";
    }

    QSqlQuery query;
    query.prepare(queryString);
    query.bindValue(QString(":in"), qvIn);
    query.bindValue(QString(":aus"), qvAus);
    query.exec();

    return query.isActive();
}

// Insert a new word pair with comments and set rank to zero
bool Managedb::insertRec(QString in, QString aus, QString commentin, QString commentaus)
{
    QVariant qvIn(in);
    QVariant qvAus(aus);
    QVariant qvComIn(commentin);
    QVariant qvComAus(commentaus);
    bool success = false;

    QSqlQuery query;
    query.prepare("INSERT INTO " + tableOneName + " (inland, ausland, commentin, commentaus, rightt, wrong, rank) "
                  "VALUES (:inland, :ausland, :commentin, :commentaus, 0, 0, 0);");
    query.bindValue(QString(":inland"), qvIn);
    query.bindValue(QString(":ausland"), qvAus);
    query.bindValue(QString(":commentin"), qvComIn);
    query.bindValue(QString(":commentaus"), qvComAus);
    query.exec();

    //success = query.prepare("INSERT INTO " + tableOneName + " (inland, ausland, commentin, commentaus, rightt, wrong, rank) "
    //              "VALUES (" + in + "," + aus + "," + commentin + "," + commentaus + ", 0, 0 ,0)");

    if(!success){
        QSqlError err = query.lastError();
        qDebug() << err.text();
        qDebug() << query.lastQuery();
    }


    return success;
}

// Loads all records from the database and the list records which contains all words shuffled and in the follow data structure
// records{ inland(QStringList), ausland(QStringList), commentIn(QStringList), commentAus(QStringList) }
QList<QStringList> Managedb::getVocs()
{
    QList<QStringList> records;
    QStringList record;
    QList<QStringList> recordsbuff;
    QList<int> intList;
    int listCount;

    QSqlQuery query("SELECT inland,ausland,commentin,commentaus,rightt FROM " + tableOneName + ";");

    while(query.next()) {
        record.append(query.value(0).toString());
        record.append(query.value(1).toString());
        record.append(query.value(2).toString());
        record.append(query.value(3).toString());
        record.append(query.value(4).toString());
        record.append("false");
        recordsbuff.append(record);
        record.clear();
    }

    if(recordsbuff.isEmpty()){
        //pop up message
        qDebug() << "UUUps aus der Datenbank kam nichts...";
    }

    listCount = recordsbuff.size();
    // The range of the list goes from 0 to listCount-1
    for(int i=0; i<listCount; i++){
        intList.append(i);
    }

    // Shuffle the in the above step created intList.
    std::random_shuffle(intList.begin(),intList.end());

    // Use the shuffled intList to get shuffled word lists
    for(int i=0; i<listCount; i++){
        records.append(recordsbuff.at(i));
    }

    return records;
}

bool Managedb::updateRecAusland(QString aus, QString in)
{
    QString bubl = "UPDATE " + tableOneName + " SET ausland='" + aus + "' where inland='" + in + "';";
    qDebug() << "updateRecAusland: " << bubl;
    QSqlQuery query(bubl);
    query.exec();

    return query.isActive();
}

int Managedb::dbToJson()
{
    QJsonObject jobj;
    QJsonArray jsonArr;
    QSqlQuery query("SELECT inland,ausland,commentin,commentaus,rightt FROM " + tableOneName + ";");

    while(query.next()){
        QJsonObject vocPair;
        vocPair["inland"] = query.value(0).toString();
        vocPair["ausland"] = query.value(1).toString();
        vocPair["commentin"] = query.value(2).toString();
        vocPair["commentaus"] = query.value(3).toString();
        vocPair["rightt"] = query.value(4).toString();

        jsonArr.append(vocPair);
    }
    jobj["Vokabeln"] = jsonArr;

    QFile exportFile("vocabulary.json");

    if(!exportFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Couldn't open save file";
        return -1;
    }

    QJsonDocument jdoc(jobj);
    exportFile.write(jdoc.toJson());

    return 1;
}

int Managedb::jsonToDb()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    qDebug() << fileName;
    QFile importFile(fileName);
    if(!importFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fehler beim öffnen der Datei " << fileName;
        return -1;
    }

    QJsonObject jObj;
    QJsonDocument jDoc;
    QJsonArray jArr;
    //QJsonArray::Iterator i;
    QSqlQuery query;
    //query.exec("START TRANSACTION;");

    qDebug() << "Ja genau wir sind hier!";
    QByteArray byteArray = importFile.readAll();
    jDoc = QJsonDocument::fromJson(byteArray);
    jObj = jDoc.object();
    QJsonObject buff(jObj["Vokabeln"].toObject());
    for(int i=0; i<jArr.size();i++){
        query.prepare("INSERT INTO " + tableOneName + " (inland, ausland, commentin, commentaus, rightt)\
            VALUES(:inland, :ausland, :commentin, :commentaus, :right);");
        query.bindValue(QString(":inland"), QVariant(buff["inland"].toString()));
        query.bindValue(QString(":ausland"), QVariant(buff["ausland"].toString()));
        query.bindValue(QString(":commentin"), QVariant(buff["commentin"].toString()));
        query.bindValue(QString(":commentaus"), QVariant(buff["commentaus"].toString()));
        query.bindValue(QString(":rightt"), QVariant(buff["rightt"].toString()));

        query.exec();
    }

    //query.exec("COMMIT;");

    return 1;
}
