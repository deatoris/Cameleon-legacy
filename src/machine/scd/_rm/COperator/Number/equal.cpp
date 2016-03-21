#include "equal.h"


equalnumber::equalnumber(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"v1.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"v2.num");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Number");
    this->setKey("NumberequalNumber");
    this->setName("EqualNumber");
    this->setInformation("out = true for v1=v2, false otherwise");
}
void equalnumber::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();

    bool t;
    if(v1 == v2){
        t=true;
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setData(&t);
    }else{
        t=false;
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setData(&t);
    }
}

COperator * equalnumber::clone(){
    return new equalnumber();
}
