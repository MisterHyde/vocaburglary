#include "customfunctions.h"

QString CF::undoStringList(QStringList list){
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
