#include "managedjsondb.h"

ManagedJsonDb::ManagedJsonDb()
    :pathToJson("vocabulary.json")
{
//    QFile database(pathToJson);
//    if(!database.open(QIODevice::ReadOnly | QIODevice::Text)){
//        qDebug() << "Couldn't open read file";
//        //return -1;
//    }
//    QFile backup("." + pathToJson + ".bak");
//    if(!backup.open(QIODevice::WriteOnly | QIODevice::Text)){
//        qDebug() << "Couldn't open backup file";
//        //return -1;
//    }
//    QTextStream in(&database);
//    QTextStream out(&backup);
//    out << in.readAll();
}

//ManagedJsonDb::~ManagedJsonDb()
//{
//    delete database;
//}

int ManagedJsonDb::insertRec(QString in, QString aus, QString commentin, QString commentaus)
{
    QJsonObject jObj;
    jObj["inland"] = in;
    jObj["ausland"] = aus;
    jObj["commentin"] = commentin;
    jObj["commentaus"] = commentaus;
    jObj["rightt"] = 0;
    jObj["wrong"] = 0;

    // Open the file
    QFile database(pathToJson);
    if(!database.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qDebug() << "Couldn't open save file";
        return -1;
    }

    QTextStream out(&database);
    QJsonDocument jDoc(jObj);
    out << jDoc.toJson();
    out.flush();
    database.close();
    return 1;
}

QList<QStringList> &ManagedJsonDb::getVocs()
{
    QFile database(pathToJson);
    if(!database.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fehler beim öffnen der Datei (ManagedJsonDb::getVocs())";
        records  = QList<QStringList>();
        return records;
    }

    QTextStream read(&database);

    // Open a backupfile and write all data from
    database.copy("." + pathToJson + ".bak");

    QString jsonElement = "";

    while(!read.atEnd()){
        QString buff2 = read.readLine();
        jsonElement += buff2;
        if(buff2 == "}"){
            QJsonObject jObj = QJsonDocument::fromJson(jsonElement.toUtf8()).object();
            QStringList record;
            record.append(jObj.value("inland").toString());
            record.append(jObj.value("ausland").toString());
            record.append(jObj.value("commentin").toString());
            record.append(jObj.value("commentaus").toString());
            record.append(jObj.value("rightt").toString());
            record.append(jObj.value("wrong").toString());

            records.append(record);
            recordsIndex.append(jObj.value("ausland").toString());
            jsonElement = "";
        }
    }

    database.close();
    return records;
}

int ManagedJsonDb::dbToJson()
{
    return -1;
}

int ManagedJsonDb::jsonToDb()
{
    return -1;
}

bool ManagedJsonDb::updateRank(QString in, QString aus, bool right)
{
    if(right){
        QStringList buff = records.at(recordsIndex.indexOf(aus));
        int rightCount = buff.at(4).toInt() + 1;
        records[recordsIndex.indexOf(aus)][4] = rightCount;
    }
    else{
        QStringList buff = records.at(recordsIndex.indexOf(aus));
        int rightCount = buff.at(4).toInt() + 1;
        records[recordsIndex.indexOf(aus)][4] = rightCount;
    }
    return true;
}

bool ManagedJsonDb::updateRecAusland(QString aus, QString in)
{
    return false;
}
