#include "OperatorBreakPoint.h"

#include "CLogger.h"
#include "CGCompositionManager.h"
OperatorBreakPoint::OperatorBreakPoint(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output0.gen");
    this->path().push_back("Introspection");
    this->setKey("OperatorBreakPoint");
    this->setName("BreakPoint");
    this->setInformation("pause & next the current Caméléon instance");
    this->setExecutedThread(COperator::CLIENT);
}

void OperatorBreakPoint::exec(){
    for(int i =0;i<(int)this->plugOut().size();i++)
        this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i]->getData());

    CGCompositionManager::getInstance()->getComposer()->pause();
//    while(!CGCompositionManager::getInstance()->getComposer()->getIsPaused()){
//        //dummy
//    }
    CGCompositionManager::getInstance()->getComposer()->nextStep();
}

COperator * OperatorBreakPoint::clone(){
    return new OperatorBreakPoint();
}
