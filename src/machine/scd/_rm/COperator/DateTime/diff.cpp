#include "diff.h"

diff::diff(){
    this->structurePlug().addPlugIn(DataDateTime::KEY,"input1.datetime");
    this->structurePlug().addPlugIn(DataDateTime::KEY,"input2.datetime");
    this->structurePlug().addPlugOut(DataNumber::KEY,"msecs.num");
    this->path().push_back("DateTime");
    this->setKey("datetimediff");
    this->setName("diff");
}

void diff::exec(){
    DataDateTime* input1 = dynamic_cast<DataDateTime *>(this->plugIn()[0]->getData());
    DataDateTime* input2 = dynamic_cast<DataDateTime *>(this->plugIn()[1]->getData());
    DataNumber* datetimeout = dynamic_cast<DataNumber *>(this->plugOut()[0]->getData());

    QDateTime t1 = input1->getValue();
    QDateTime t2 = input2->getValue();
    qint64 i = t2.toMSecsSinceEpoch() - t1.toMSecsSinceEpoch();
    if(i<0) i=-i;
    datetimeout->setValue(i);
}

COperator * diff::clone(){
    return new diff();
}
