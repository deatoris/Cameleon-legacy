#include "concat.h"


concat::concat(){
    this->structurePlug().addPlugIn(DataString::KEY,"in1.str");
    this->structurePlug().addPlugIn(DataString::KEY,"in2.str");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");
    this->path().push_back("String");
    this->setKey("stringconcatstring");
    this->setName("ConcatString");
     this->setInformation("out=[in1,in2]");
}

void concat::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string v2 = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(v1+v2);
}

COperator * concat::clone(){
    return new concat();
}
