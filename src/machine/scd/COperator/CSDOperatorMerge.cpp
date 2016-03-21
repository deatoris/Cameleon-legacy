#include "CSDOperatorMerge.h"

CSDOperatorMerge::CSDOperatorMerge(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in1.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in2.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"out.gen");
    this->path().push_back("Generic");
    this->setKey("CSDOperatorMerge");
    this->setName("Merge");
}
void CSDOperatorMerge::exec(){
    if(this->plugIn()[0]->getState()==CPlug::NEW)
        this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
    else
        this->plugOut()[0]->getData()->dumpReception(this->plugIn()[1]->getData());
}
bool CSDOperatorMerge::executionCondition(){
    if(this->plugOut()[0]->getState()==CPlug::NEW&&this->plugOut()[0]->isConnected()==true){
        return false;
    }
    else if(this->plugIn()[0]->getState()!=CPlug::NEW && this->plugIn()[1]->getState()!=CPlug::NEW)
    {
        return false;
    }
    else
        return true;
}

COperator * CSDOperatorMerge::clone(){
    return new CSDOperatorMerge();
}

