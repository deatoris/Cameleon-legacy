#include "OperatorExitInstance.h"

#include "CLogger.h"
#include "DataBoolean.h"
OperatorExitInstance::OperatorExitInstance(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"exit.bool");
    this->path().push_back("Introspection");
    this->path().push_back("Instance");
    this->setKey("OperatorExitInstance");
    this->setName("ExitInstance");
   this->setInformation("exit & kill current Camlon instance");
}

void OperatorExitInstance::exec(){
    DataBoolean * d2 = dynamic_cast<DataBoolean *>(this->plugIn()[0]->getData());

    bool b2 = d2->getValue();
    if(b2){
        exit(0);
    }
}

COperator * OperatorExitInstance::clone(){
    return new OperatorExitInstance();
}
