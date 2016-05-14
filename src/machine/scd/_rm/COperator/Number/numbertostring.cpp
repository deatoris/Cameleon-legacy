#include "numbertostring.h"

NumberToString::NumberToString(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"in.num");
    this->structurePlug().addPlugOut(DataString::KEY,"result.str");
    this->path().push_back("Number");
    this->setKey("NumberNumberToStringNumber");
    this->setName("ToStringNumber");
}
void NumberToString::exec(){
    string s = this->plugIn()[0]->getData()->toString();
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setData(&s);
}

COperator * NumberToString::clone(){
    return new NumberToString();
}

