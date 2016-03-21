#include "sync.h"
#include<CUtilitySTL.h>
#include<CMachine.h>
syncGen::syncGen(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input0.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input1.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output1.gen");
    this->path().push_back("Control-Flow");
    this->setKey("sync");
    this->setName("Synchrone");
    this->structurePlug().addAction("AddPlug");
    this->setInformation("Exec if all input plug states are NEW");

}
void syncGen::action(string key){
    QString s = key.c_str();
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input"+SCD::ConvertString::Any2String(this->structurePlug().plugIn().size()) +".gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output"+SCD::ConvertString::Any2String(this->structurePlug().plugOut().size()) +".gen");
    this->_v_plug_in.push_back(new CPlugIn(this,DataGeneric::KEY,this->_v_plug_in.size()));
    this->_v_plug_out.push_back(new CPlugOut(this,DataGeneric::KEY,this->_v_plug_out.size()));
    CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
}

void syncGen::exec(){
    for(int i =0;i<(int)this->plugOut().size();i++)
        this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i]->getData());
}

bool syncGen::executionCondition(){
    for(int i =0;i<(int)this->plugOut().size();i++){
        if(this->plugOut()[i]->getState()==CPlug::NEW && this->plugOut()[i]->isConnected()==true)
            return false;
    }
    for(int i =0;i<(int)this->plugIn().size();i++){
        if(this->plugIn()[i]->getState()!=CPlug::NEW)
            return false;
    }
    return true;
}

COperator * syncGen::clone(){
    return new syncGen();
}
