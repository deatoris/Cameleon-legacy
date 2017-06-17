#include "tostring.h"
#include "DataString.h"

tostring::tostring(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input.gen");
    this->structurePlug().addPlugOut(DataString::KEY,"tostring.str");
    this->path().push_back("Generic");
    this->setKey("tostring");
    this->setName("TosSring");
    this->setInformation("save data information in tostring");
}

void tostring::exec(){
    string s = this->plugIn()[0]->getData()->toString();
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(s);
}

COperator * tostring::clone(){
    return new tostring();
}

