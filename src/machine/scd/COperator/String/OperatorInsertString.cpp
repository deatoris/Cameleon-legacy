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
    this->setInformation("The current string content is extended by inserting some additional content at a specific location within the string content.");
}
void OperatorInsertString::exec(){
    string in_str = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    int    num =    dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    string str =    dynamic_cast<DataString *>(this->plugIn()[2]->getData())->getValue();
    if(in_str.size()>=num){
        in_str.insert(num,str);
    }else{
        this->error("in.num is out of range");
        return;
    }
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(in_str);
}
COperator * OperatorInsertString::clone(){
    return new OperatorInsertString;
}
