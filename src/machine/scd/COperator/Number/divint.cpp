#include "divint.h"


numberdivint::numberdivint(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"v1.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"v2.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"r.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"m.num");

    this->path().push_back("Number");
    this->setKey("numberdivintNumber");
    this->setName("DivNumberInt");
    this->setInformation("v=v1/v2;r result;m modulo");
}
void numberdivint::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int r = v1/v2;
    int m = (int)v1%(int)v2;
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(r);
    dynamic_cast<DataNumber *>(this->plugOut()[1]->getData())->setValue(m);
}

COperator * numberdivint::clone(){
    return new numberdivint();
}
