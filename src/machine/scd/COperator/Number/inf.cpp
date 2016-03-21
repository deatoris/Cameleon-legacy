#include "inf.h"


inf::inf(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"v1.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"v2.num");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Number");
    this->setKey("NumberinfNumber");
    this->setName("InfNumber");
    this->setInformation("out = true for v1<v2, false otherwise");
}
void inf::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();

    if(v1 < v2){
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
    }else{
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(false);
    }
}

COperator * inf::clone(){
    return new inf();
}
