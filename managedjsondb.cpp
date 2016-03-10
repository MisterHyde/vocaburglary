#include "managedjsondb.h"

ManagedJsonDb::ManagedJsonDb()
{

}

bool ManagedJsonDb::insertRec(QString in, QString aus, QString commentin, QString commentaus){
    QJsonObject jobj;
    QJsonArray jArr;


}

QList<QStringList> ManagedJsonDb::getVocs()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    QFile importFile(fileName);
    if(!importFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fehler beim öffnen der Datei" << fileName;
        return QList<QStringList>();
    }

    QJsonObject jObj;
    QJsonDocument jDoc;
    QJsonArray jArr, jArr2;
    QList<QStringList> records;

    // Read everything out of the file
    QByteArray byteArray = importFile.readAll();
    // Convert the text we'd read to a QJsonDocument
    jDoc = QJsonDocument::fromJson(byteArray);

    // Convert the QJsonDocument to a QJsonObject
    jObj = jDoc.object();
    // Convert everything which is containt under the node "Vokabeln" into a QJsonArray
    jArr = jObj["Vokabeln"].toArray();
    // Convert that QJsonArray in a QVariantList which we can finally use to work with
    QVariantList vList = jArr.toVariantList();
    // Use c++11 "foreach" loop each entry in vList is mapped to v
    for(QVariant v: vList){
        QMap<QString,QVariant> map = v.toMap();
        QStringList record;
        record.append(map.value("inland").toString());
        record.append(map.value("ausland").toString());
        record.append(map.value("commentin").toString());
        record.append(map.value("commentaus").toString());
        record.append(map.value("rightt").toString());
        record.append(map.value("wrong").toString());

        records.append(record);
    }

    return records;
}
