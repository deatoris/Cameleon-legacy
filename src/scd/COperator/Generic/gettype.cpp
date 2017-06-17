#include "gettype.h"
#include "DataString.h"
gettype::gettype(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input.gen");
    this->structurePlug().addPlugOut(DataString::KEY,"gettype.str");
    this->path().push_back("Generic");
    this->setKey("gettype");
    this->setName("GetType");
}

void gettype::exec(){
    string s = this->plugIn()[0]->getData()->getKey();
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(s);    
}

COperator * gettype::clone(){
    return new gettype();
}
