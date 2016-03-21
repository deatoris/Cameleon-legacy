#include "id.h"
#include "CLogger.h"
#include<CUtilitySTL.h>
#include<CMachine.h>
id::id(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output0.gen");
    this->path().push_back("Control-Flow");
    this->setKey("id");
    this->setName("Id");
    this->structurePlug().addAction("AddPlug");
    this->setInformation("Copy the input datas of the output datas of the same index starting from the lowest index to the highest");
}

void id::action(string key){
    QString s = key.c_str();
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input"+SCD::ConvertString::Any2String(this->structurePlug().plugIn().size()) +".gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output"+SCD::ConvertString::Any2String(this->structurePlug().plugOut().size()) +".gen");
    this->_v_plug_in.push_back(new CPlugIn(this,DataGeneric::KEY,this->_v_plug_in.size()));
    this->_v_plug_out.push_back(new CPlugOut(this,DataGeneric::KEY,this->_v_plug_out.size()));
    CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
}

void id::exec(){
    for(int i =0;i<(int)this->plugOut().size();i++)
        this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i]->getData());
}

COperator * id::clone(){
    return new id();
}
