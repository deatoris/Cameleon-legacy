#include "releasenewstate.h"

#include "CLogger.h"
releasenewstate::releasenewstate(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output0.gen");
    this->path().push_back("Control-Flow");
    this->setKey("releasenewstate");
    this->setName("releasenewstate");
    this->setInformation("execute the operator if the input plug is new even if the outplug is new");
}


void releasenewstate::exec(){
    this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
}

COperator * releasenewstate::clone(){
    return new releasenewstate();
}
bool releasenewstate::executionCondition(){
    if(this->plugIn()[0]->getState()==CPlug::NEW)
        return true;
    else
        return false;
}
