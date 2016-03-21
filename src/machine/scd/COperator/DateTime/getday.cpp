#include "getday.h"

getday::getday(){
    this->structurePlug().addPlugIn(DataDateTime::KEY,"element.datetime");
    this->structurePlug().addPlugOut(DataDateTime::KEY,"element.datetime");
    this->structurePlug().addPlugOut(DataNumber::KEY,"day.num");
    this->path().push_back("DateTime");
    this->setKey("datetimegetday");
    this->setName("getday");
}

void getday::exec(){
    DataDateTime* genin = dynamic_cast<DataDateTime *>(this->plugIn()[0]->getData());
    DataDateTime* genout = dynamic_cast<DataDateTime *>(this->plugOut()[0]->getData());
    DataNumber* datetimeout = dynamic_cast<DataNumber *>(this->plugOut()[1]->getData());

    datetimeout->setValue(genin->getValue().date().day());
    genout->dumpReception(genin);
}

COperator * getday::clone(){
    return new getday();
}
