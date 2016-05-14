#include "uid.h"
#include "quuid.h"
#include<DataString.h>
uid::uid(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"a.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"a.gen");
    this->structurePlug().addPlugOut(DataString::KEY,"uid.str");
    this->path().push_back("Number");
    this->setKey("NumberuidNumber");
    this->setName("uidNumber");
    this->setInformation("");
}
void uid::exec(){
    QUuid id = QUuid::createUuid ();
    this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(id.toString().toStdString());
}

COperator * uid::clone(){
    return new uid();
}
