#include "OperatorProjectTmpPathString.h"

#include "CMachine.h"
#include "DataBoolean.h"
OperatorProjectTmpPath::OperatorProjectTmpPath(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"in.bool");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");
    this->path().push_back("Introspection");
    this->path().push_back("Project");
    this->setKey("stringOperatorProjectTmpPathstring");
    this->setName("ProjectTmpPath");
}

void OperatorProjectTmpPath::exec(){
    QString path = CMachineSingleton::getInstance()->getTmpPath().c_str();
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(path.toStdString());
}

COperator * OperatorProjectTmpPath::clone(){
    return new OperatorProjectTmpPath();
}

