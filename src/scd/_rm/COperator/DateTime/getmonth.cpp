#include "getmonth.h"

getmonth::getmonth(){
    this->structurePlug().addPlugIn(DataDateTime::KEY,"element.datetime");
    this->structurePlug().addPlugOut(DataDateTime::KEY,"element.datetime");
    this->structurePlug().addPlugOut(DataNumber::KEY,"month.num");
    this->path().push_back("DateTime");
    this->setKey("datetimegetmonth");
    this->setName("getmonth");
}

void getmonth::exec(){
    DataDateTime* genin = dynamic_cast<DataDateTime *>(this->plugIn()[0]->getData());
    DataDateTime* genout = dynamic_cast<DataDateTime *>(this->plugOut()[0]->getData());
    DataNumber* datetimeout = dynamic_cast<DataNumber *>(this->plugOut()[1]->getData());

    datetimeout->setValue(genin->getValue().date().month());
    genout->dumpReception(genin);
}

COperator * getmonth::clone(){
    return new getmonth();
}
