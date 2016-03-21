#include "OperatorPowNumber.h"
#include<cmath>
OperatorPowNumber::OperatorPowNumber(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"v.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"n.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"x.num");
    this->path().push_back("Number");
    this->setKey("OperatorPowNumber");
    this->setName("PowNumber");
    this->setInformation("x=v^n");
}
void OperatorPowNumber::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(pow(v1,v2));
}

COperator * OperatorPowNumber::clone(){
    return new OperatorPowNumber();
}
