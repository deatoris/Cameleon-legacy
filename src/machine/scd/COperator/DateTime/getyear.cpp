#include "getyear.h"

getyear::getyear(){
    this->structurePlug().addPlugIn(DataDateTime::KEY,"element.datetime");
    this->structurePlug().addPlugOut(DataDateTime::KEY,"element.datetime");
    this->structurePlug().addPlugOut(DataNumber::KEY,"year.num");
    this->path().push_back("DateTime");
    this->setKey("datetimegetyear");
    this->setName("getyear");
}

void getyear::exec(){
    DataDateTime* genin = dynamic_cast<DataDateTime *>(this->plugIn()[0]->getData());
    DataDateTime* genout = dynamic_cast<DataDateTime *>(this->plugOut()[0]->getData());
    DataNumber* datetimeout = dynamic_cast<DataNumber *>(this->plugOut()[1]->getData());

    datetimeout->setValue(genin->getValue().date().year());
    genout->dumpReception(genin);
}

COperator * getyear::clone(){
    return new getyear();
}
