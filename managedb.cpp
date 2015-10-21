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

    QSqlQuery query;
    query.prepare("INSERT INTO  " + tableOneName + "  (inland, ausland, commentin, commentaus, rightt, wrong, rank) "
                  "VALUES (:inland, :ausland, :commentin, :commentaus, 0, 0 ,0)");
    query.bindValue(QString(":inland"), QVariant(qvIn));
    query.bindValue(QString(":ausland"), QVariant(qvAus));
    query.bindValue(QString(":commentin"), QVariant(qvComIn));
    query.bindValue(QString(":commentaus"), QVariant(qvComAus));
    query.exec();

    qDebug() << "insertRec: " << in << aus << commentin << commentaus;

    return query.isActive();
}

// Loads all records from the database and the list records which contains all words shuffled and in the follow data structure
// records{ inland(QStringList), ausland(QStringList), commentIn(QStringList), commentAus(QStringList) }
QList<QStringList> Managedb::getVocs()
{
    QStringList inbuff;
    QStringList ausbuff;
    QStringList incombuff;
    QStringList auscombuff;
    QStringList rightbuff;
    QList<QStringList> records;
    QList<int> intList;
    int listCount;

    QSqlQuery query("SELECT inland,ausland,commentin,commentaus,rightt FROM " + tableOneName + ";");

    while(query.next()) {
        inbuff.append(query.value(0).toString());
        ausbuff.append(query.value(1).toString());
        incombuff.append(query.value(2).toString());
        auscombuff.append(query.value(3).toString());
        rightbuff.append(query.value(4).toString());
    }
    if(inbuff.isEmpty()){
        //pop up message
    }

    // If the two columns "inland" and "ausland" contains a different amount of items: return false
    listCount = inbuff.count();
    if(listCount != ausbuff.count())
        return records;

    // The range of the list goes from 0 to listCount-1
    for(int i=0; i<listCount; i++){
        intList.append(i);
    }

    // Shuffle the in the above step created intList.
    std::random_shuffle(intList.begin(),intList.end());

    QStringList inland;
    QStringList ausland;
    QStringList commentin;
    QStringList commentaus;
    QStringList right;

    // Use the shuffled intList to get shuffled word lists
    for(int i=0; i<listCount; i++){
        inland.insert(i, inbuff.at(intList.at(i)));
        ausland.insert(i, ausbuff.at(intList.at(i)));
        commentin.insert(i, incombuff.at(intList.at(i)));
        commentaus.insert(i, auscombuff.at(intList.at(i)));
        right.insert(i, rightbuff.at(intList.at(i)));
    }

    // Save all columns in one list
    records << inland << ausland << commentin << commentaus << right;

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
    jobj["Vocabeln"] = jsonArr;

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
    QFile importFile("vocabulary.json");
    //Ui::Dialog *dialog = new Ui::Dialog;
    //dialog->setupUi(this);
}
