#include "numberfromstring.h"


NumberFromString::NumberFromString(){
    this->structurePlug().addPlugIn(DataString::KEY,"in1.str");
    this->structurePlug().addPlugOut(DataNumber::KEY,"result.num");
    this->path().push_back("Number");
    this->setKey("NumberNumberFromStringNumber");
    this->setName("FromStringNumber");
}
void NumberFromString::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();


    QString s = v1.c_str();
    double v2 = s.toDouble();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(v2);
}

COperator * NumberFromString::clone(){
    return new NumberFromString();
}
