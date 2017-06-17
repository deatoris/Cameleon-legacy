#include "OperatorMinNumber.h"

OperatorMinNumber::OperatorMinNumber(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"v1.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"v2.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"v.num");
    this->path().push_back("Number");
    this->setKey("OperatorMinNumber");
    this->setName("MinNumber");
    this->setInformation("v=Min(v1,v2)");
}
void OperatorMinNumber::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(min(v1,v2));
}

COperator * OperatorMinNumber::clone(){
    return new OperatorMinNumber();
}
