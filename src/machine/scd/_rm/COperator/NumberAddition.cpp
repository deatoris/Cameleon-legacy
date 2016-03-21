#include "NumberAddition.h"


NumberAddition::NumberAddition(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"in1.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"in2.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"out.num");
    this->path().push_back("Number");
    this->setKey("NumberAddNumber");
    this->setName("Add");
}
void NumberAddition::exec(){
    double v1 = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    double v2 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
   //cout<<v1+v2<<endl;
    //sleep(3);
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(v1+v2);
}

COperator * NumberAddition::clone(){
    return new NumberAddition();
}
