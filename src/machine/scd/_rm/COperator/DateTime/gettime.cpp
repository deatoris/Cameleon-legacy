#include "gettime.h"

gettime::gettime(){
    this->structurePlug().addPlugIn(DataDateTime::KEY,"element.datetime");
    this->structurePlug().addPlugOut(DataNumber::KEY,"time.num");
    this->path().push_back("DateTime");
    this->setKey("datetimegettime");
    this->setName("gettime");
}

void gettime::exec(){
    DataDateTime* genin = dynamic_cast<DataDateTime *>(this->plugIn()[0]->getData());
    DataNumber* datetimeout = dynamic_cast<DataNumber*>(this->plugOut()[0]->getData());

    datetimeout->setValue(genin->getValue().time().msec());
}

COperator * gettime::clone(){
    return new gettime();
}
