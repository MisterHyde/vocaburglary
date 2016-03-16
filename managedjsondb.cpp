#include "managedjsondb.h"

ManagedJsonDb::ManagedJsonDb()
    :pathToJson("vocabulary.json")
{
    //database.setFileName("vocabulary.json");
    //database = new QFile("vocabulary.json");
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

QList<QStringList> ManagedJsonDb::getVocs()
{
    QFile database(pathToJson);
    if(!database.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fehler beim öffnen der Datei (ManagedJsonDb::getVocs())";
        return QList<QStringList>();
    }

    QTextStream read(&database);
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
    return false;
}

bool ManagedJsonDb::updateRecAusland(QString aus, QString in)
{
    return false;
}
