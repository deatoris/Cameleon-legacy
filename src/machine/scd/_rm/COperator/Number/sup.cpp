#include "sup.h"

sup::sup(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"a.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"b.num");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Number");
    this->setKey("NumbersupNumber");
    this->setName("SupNumber");
    this->setInformation("out = true for  a>b, false otherwise  ");
}
void sup::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    bool t;
    if(v1 > v2){
        t=true;
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setData(&t);
    }else{
        t=false;
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setData(&t);
    }
}

COperator * sup::clone(){
    return new sup();
}
