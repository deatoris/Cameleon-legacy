#include "OperatorGetMode.h"

#include "CLogger.h"
#include "DataString.h"
#include "CGInstance.h"
GetMode::GetMode(){
    this->structurePlug().addPlugOut(DataString::KEY,"mode.str");
    this->path().push_back("Introspection");
    this->path().push_back("Instance");
    this->setKey("GetMode");
    this->setName("GetMode");
   this->setInformation("get instance mode : \n MNG full ide, \n GUI standalone with gui, \n CMD standalone command line.");
}

void GetMode::exec(){
    CGInstance::Mode m = CGInstance::getInstance()->getMode();
    QString mode = "";
    if(CGInstance::CMD == m){
        mode ="CMD";
    }else if(CGInstance::GUI == m){
        mode ="GUI";
    }else if(CGInstance::MNG == m){
        mode ="MNG";
    }else{
        this->error("UNKNOWN MODE");
    }
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(mode.toStdString());
}

COperator * GetMode::clone(){
    return new GetMode();
}
