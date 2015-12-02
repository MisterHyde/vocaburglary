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

void CustomFuctions::parseXml(QString path)
{
    QFile source(path);
    if(!source.open(QIODevice::ReadOnly)){
        qDebug() << "Xml file couldn't be opened";
        return;
    }

    QXmlStreamReader xml(&source);
    QMap <QString, QString> map;
    int i=0;

    while(!xml.atEnd() && i<100){
        qDebug() << "current element:" << i;
        if(xml.readNext() == QXmlStreamReader::StartElement || true){
            QString buff = xml.readElementText(QXmlStreamReader::IncludeChildElements);
            if(!buff.isEmpty()){
                buff.remove("\n ");
                QStringList buffList= buff.split(" \n");
                qDebug() << buffList.at(0).trimmed() << "\n"
                         << buffList.at(1).trimmed() << "\n"
                         << buffList.at(2).trimmed();
                int numb = buffList.size();
                QJsonArray jArr;
                for(int j=1; j<numb-2; j++){
                    QJsonObject jobt;
                    // When listelement at j is empty get to the next one
                    if(buffList.at(j).trimmed().isEmpty())
                        j++;
                    jobt["inland"] = buffList.at(j+1).trimmed();
                    jobt["ausland"] = buffList.at(j+2).trimmed();
                    jobt["commentin"] = QString(j);
                    jobt["commentaus"] = QString(j+1);
                    jobt["rightt"] = QString(j+2);

                    jArr.append(jobt);
                }

                QJsonObject jobj;
                jobj["Vokabeln"] = jArr;
                QFile exportFile("vocabulary.json");
                if(!exportFile.open(QIODevice::WriteOnly | QIODevice::Text)){
                    qDebug() << "ERROR: couldn't open json file.";
                    return;
                }
                QJsonDocument jdoc(jobj);
                exportFile.write(jdoc.toJson());
            }
        }
        i++;
    }

    if(xml.hasError())
        qDebug() << "An error in the XML file occured:" << xml.errorString();

    return;
}
