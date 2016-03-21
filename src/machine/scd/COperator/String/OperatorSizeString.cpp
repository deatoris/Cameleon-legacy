#include "OperatorSizeString.h"

#include<DataString.h>
#include<DataNumber.h>
OperatorSizeString::OperatorSizeString(){
    this->structurePlug().addPlugIn(DataString::KEY,"in.str");
    this->structurePlug().addPlugOut(DataNumber::KEY,"out.num");
    this->path().push_back("String");
    this->setKey("OperatorSizeString");
    this->setName("SizeString");
    this->setInformation("Size of the input string");
}

void OperatorSizeString::exec(){
    string str = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue((int)str.size());
}

COperator * OperatorSizeString::clone(){
    return new OperatorSizeString;
}
