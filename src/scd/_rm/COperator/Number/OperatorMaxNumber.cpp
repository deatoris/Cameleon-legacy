#include "OperatorMaxNumber.h"


OperatorMaxNumber::OperatorMaxNumber(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"v1.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"v2.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"v.num");
    this->path().push_back("Number");
    this->setKey("OperatorMaxNumber");
    this->setName("MaxNumber");
    this->setInformation("v=max(v1,v2)");
}
void OperatorMaxNumber::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(max(v1,v2));
}

COperator * OperatorMaxNumber::clone(){
    return new OperatorMaxNumber();
}
