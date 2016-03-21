#include "current.h"


current::current(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"element.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"element.gen");
    this->structurePlug().addPlugOut(DataDateTime::KEY,"current.datetime");
    this->path().push_back("DateTime");
    this->setKey("datetimecurrent");
    this->setName("current");
}

void current::exec(){
    DataGeneric* genin = dynamic_cast<DataGeneric *>(this->plugIn()[0]->getData());
    DataGeneric* genout = dynamic_cast<DataGeneric *>(this->plugOut()[0]->getData());
    DataDateTime* datetimeout = dynamic_cast<DataDateTime *>(this->plugOut()[1]->getData());

    datetimeout->setValue(QDateTime::currentDateTime());
    string str = datetimeout->toString();
    genout->dumpReception(genin);
}

COperator * current::clone(){
    return new current();
}
