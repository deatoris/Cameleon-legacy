#include "exitloop.h"
#include "CConnectorDirect.h"

exitloop::exitloop(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input1.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output1.gen");
    this->path().push_back("Control-Flow");
    this->setKey("execloop");
    this->setName("Erase");
    this->setInformation("input1 to output1 if input1 == NEW & whatever the state of the output plug.");
}

void exitloop::exec(){
    this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
}

bool exitloop::executionCondition(){
    if(this->plugIn()[0]->getState()==CPlug::NEW){
        return true;
    }else{
        return false;
    }
}

COperator * exitloop::clone(){
    return new exitloop();
}
