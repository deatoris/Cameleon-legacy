#include "OperatorInsertString.h"
#include<DataString.h>
#include<DataNumber.h>
OperatorInsertString::OperatorInsertString(){
    this->structurePlug().addPlugIn(DataString::KEY,"in.str");
    this->structurePlug().addPlugIn(DataNumber::KEY,"in.num");
    this->structurePlug().addPlugIn(DataString::KEY,"str.str");
    this->structurePlug().addPlugOut(DataString::KEY,"out.num");
    this->path().push_back("String");
    this->setKey("OperatorInsertString");
    this->setName("InsertString");
    this->setInformation("Insert of the string str in the input string in at the ");
}
void OperatorInsertString::exec(){
    string in_str = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    int    num =    dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    string str =    dynamic_cast<DataString *>(this->plugIn()[2]->getData())->getValue();
    in_str.insert(num,str);
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(in_str);
}
COperator * OperatorInsertString::clone(){
    return new OperatorInsertString;
}
