#include "OperatorPlay.h"

#include "CLogger.h"
#include "CGCompositionManager.h"
OperatorPlay::OperatorPlay(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output0.gen");
    this->path().push_back("Introspection");
    this->path().push_back("Instance");
    this->setKey("OperatorPlay");
    this->setName("Stop");
    this->setInformation("pause & next the current Caméléon instance");
    this->setExecutedThread(COperator::CLIENT);
}

void OperatorPlay::exec(){
    for(int i =0;i<(int)this->plugOut().size();i++)
        this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i]->getData());

    CGCompositionManager::getInstance()->getComposer()->stop();
}

COperator * OperatorPlay::clone(){
    return new OperatorPlay();
}
