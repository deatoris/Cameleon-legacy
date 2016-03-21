#include "CSDOperatorProjectPath.h"
#include "CMachine.h"
#include "DataBoolean.h"
CSDOperatorProjectPath::CSDOperatorProjectPath(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"in.bool");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");
    this->path().push_back("Introspection");
    this->path().push_back("Project");
    this->setKey("stringCSDOperatorProjectPathstring");
    this->setName("ProjectPath");
}

void CSDOperatorProjectPath::exec(){
    QString path = CMachineSingleton::getInstance()->getTmpDir().c_str();
    path.replace("/tmp","/");
    path.replace("/tmp/","/");
    path.replace("/tmp","/");
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(path.toStdString());
}

COperator * CSDOperatorProjectPath::clone(){
    return new CSDOperatorProjectPath();
}
