#include "compare.h"

compare::compare(){
    this->structurePlug().addPlugIn(DataString::KEY,"in1.str");
    this->structurePlug().addPlugIn(DataString::KEY,"in2.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"compare.bool");
    this->path().push_back("String");
    this->setKey("stringcomparestring");
    this->setName("CompareString");
    this->setInformation("Compare=true for in1=in2, false otherwise");
}

void compare::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string v2 = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    if(v1.compare(v2) == 0){
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
    }else{
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(false);
    }
}

COperator * compare::clone(){
    return new compare();
}
