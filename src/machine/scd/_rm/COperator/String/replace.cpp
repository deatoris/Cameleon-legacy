#include "replace.h"
#include<QString>

OperatorReplaceGeneric::OperatorReplaceGeneric(){
    this->structurePlug().addPlugIn(DataString::KEY,"text.str");
    this->structurePlug().addPlugIn(DataString::KEY,"before.str");
    this->structurePlug().addPlugIn(DataString::KEY,"after.str");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");
    this->path().push_back("String");
    this->setKey("stringreplacestring");
    this->setName("ReplaceString");
    this->setInformation("Replaces every occurrence of the string before with the string after in the string text");
}

void OperatorReplaceGeneric::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string v2 = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();
    string v3 = *dynamic_cast<DataString *>(this->plugIn()[2]->getData())->getData();


    QString str1(v1.c_str());
    QString str2(v2.c_str());
    QString str3(v3.c_str());
    str1.replace(str2,str3);
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(str1.toStdString());
}

COperator * OperatorReplaceGeneric::clone(){
    return new OperatorReplaceGeneric();
}
