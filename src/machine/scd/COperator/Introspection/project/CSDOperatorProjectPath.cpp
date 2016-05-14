#include "CSDOperatorProjectPath.h"
#include "CMachine.h"
#include "CLogger.h"
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
    QString path = CMachineSingleton::getInstance()->getTmpPath().c_str();
    path.replace("/tmp","/");
    path.replace("/tmp/","/");
    path.replace("/tmp","/");
    path.replace("///","/");
    path.replace("//","/");
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(path.toStdString());
}

COperator * CSDOperatorProjectPath::clone(){
    return new CSDOperatorProjectPath();
}
CSDOperatorLog::CSDOperatorLog(){
    this->structurePlug().addPlugIn(DataString::KEY,"log.str");
    this->structurePlug().addPlugOut(DataString::KEY,"log.str");
    this->path().push_back("Introspection");
    this->path().push_back("Project");
    this->setKey("log");
    this->setName("log");
}

void CSDOperatorLog::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();

    CLogger::getInstance()->log("FUNCTIONAL",CLogger::DEBUG,QString::fromStdString(v1));
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(v1);
}

COperator * CSDOperatorLog::clone(){
    return new CSDOperatorLog();
}
