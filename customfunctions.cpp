#include "customfunctions.h"

QString CustomFuctions::undoStringList(QStringList list){
    QString buff = "";
    if(list.size()>1){
        qint8 size = list.size();
        for(int i=0;i<size;++i){
            buff += list.at(i);
            if(i != size-1)
                buff +=",";
        }
    }
    else if(list.size() == 0){
        ;
    }
    else{
        buff = list.at(0);
    }

    return buff;
}


QString CustomFuctions::csvToJson(QString pCsvPath)
{
/*    QFile csvFile(pCsvPath);
    QFile jsonFile("tmp_vocabulary.json");
    QList<QStringList> records;
    QJsonObject jsonObj;
    QJsonArray jsonArr;

    if(!csvFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Fehler beim öffnen der Datei " << pCsvPath;
        return -1;
    }
    if(!jsonFile.open(QIODevice::WriteOnly | QIODevice::Text)){
        qDebug() << "Fehler beim öffnen der Datei tmp_vocabulary.json";
        return -1;
    }

    while(!csvFile.atEnd()){
        QByteArray line = csvFile.readLine();
        QStringList record = QString(line).split(";");
        QJsonObject record;
        record["deutsch"] = record.at(1);
    }

    csvFile.close();*/
    return QString();

}
