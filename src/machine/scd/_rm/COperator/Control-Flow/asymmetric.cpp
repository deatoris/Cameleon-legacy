#include "asymmetric.h"

asymmetric::asymmetric(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input1.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input2.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output1.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output2.gen");
    this->path().push_back("Control-Flow");
    this->setKey("asymmetric");
    this->setName("asymmetric");
    this->setInformation("If input1 == NEW & input2 != EMPTY, then execute.");
}

void asymmetric::exec(){
    this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
    this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
}

bool asymmetric::executionCondition(){
    if((this->plugOut()[0]->getState()==CPlug::NEW
            && this->plugOut()[0]->isConnected()==true)
            || (this->plugOut()[1]->getState()==CPlug::NEW
                && this->plugOut()[1]->isConnected()==true)){
        return false;
    }
    if(this->plugIn()[0]->getState()==CPlug::NEW
            && this->plugIn()[1]->getState()!=CPlug::EMPTY
            &&this->plugIn()[0]->isConnected()==true
            &&this->plugIn()[1]->isConnected()==true){
        return true;
    }else{
        return false;
    }
}

COperator * asymmetric::clone(){
    return new asymmetric();
}
