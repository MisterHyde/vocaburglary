#include "managedjsondb.h"

ManagedJsonDb::ManagedJsonDb()
{
    database = new QFile("vocabulary.json");
}
ManagedJsonDb::~ManagedJsonDb()
{
    delete database;
}

bool ManagedJsonDb::insertRec(QString in, QString aus, QString commentin, QString commentaus){
    QJsonObject jObj;
    jObj["inland"] = in;
    jObj["ausland"] = aus;
    jObj["commentin"] = commentin;
    jObj["commentaus"] = commentaus;
    jObj["rightt"] = 0;
    jObj["wrong"] = 0;

    // Open the file
    if(!database->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)){
        qDebug() << "Couldn't open save file";
        return -1;
    }
    QTextStream out(database);
    QJsonDocument jDoc(jObj);
    out << jDoc.toJson();
    out.flush();
    database->close();
//    if(database->write(jDoc.toJson())>0)
//        return true;
//    else
//        return false;
}

QList<QStringList> ManagedJsonDb::getVocs()
{
    //QString fileName = QFileDialog::getOpenFileName(this);
    //QFile importFile(fileName);

    if(!database->open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fehler beim öffnen der Datei (ManagedJsonDb::getVocs())";
        return QList<QStringList>();
    }
    QTextStream read(database);

    QJsonObject jObj;
    QJsonDocument jDoc;
    //QJsonArray jArr;
    QList<QStringList> records;

    // Read everything out of the file
    //QByteArray byteArray = importFile.readAll();
    //QByteArray byteArray = read.readAll().toUtf8();
    // Convert the text we'd read to a QJsonDocument
    //jDoc = QJsonDocument::fromJson(byteArray);

    // Convert the QJsonDocument to a QJsonObject
    //jObj = jDoc.object();
    // Convert everything which is containt under the node "Vokabeln" into a QJsonArray
    //jArr = jObj["Vokabeln"].toArray();
    //jArr = jObj.toVariantMap();
    // Convert that QJsonArray in a QVariantList which we can finally use to work with
    //QVariantList vList = jArr.toVariantList();
    // Use c++11 "foreach" loop each entry in vList is mapped to v
//    for(QVariant v: vList){
//        QMap<QString,QVariant> map = v.toMap();
//        QStringList record;
//        record.append(map.value("inland").toString());
//        record.append(map.value("ausland").toString());
//        record.append(map.value("commentin").toString());
//        record.append(map.value("commentaus").toString());
//        record.append(map.value("rightt").toString());
//        record.append(map.value("wrong").toString());

//        records.append(record);
//    }
    QString jsonElement = "";
    while(!read.atEnd()){
        QString buff2 = read.readLine();
        jsonElement += buff2;
        if(buff2 == "}"){
            jDoc = QJsonDocument::fromJson(jsonElement.toUtf8());
            jObj = jDoc.object();
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

    database->close();
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
