#include "absnumber.h"
#include<cmath>
OperatorAbsNumber::OperatorAbsNumber(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"vin.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"vout.num");
    this->path().push_back("Number");
    this->setKey("OperatorAbsNumber");
    this->setName("AbsNumber");
    this->setInformation("vout = abs(vin)");
}
void OperatorAbsNumber::exec(){
    double v = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(abs(v));
}

COperator * OperatorAbsNumber::clone(){
    return new OperatorAbsNumber();
}
