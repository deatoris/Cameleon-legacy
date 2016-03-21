#include "Copy.h"
#include "CLogger.h"
#include<CMachine.h>
#include<CUtilitySTL.h>
Copy::Copy(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"copy0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"copy1.gen");
//    this->path().push_back("Generic");
    this->setKey("Copy");
    this->setName("Copy");
    this->structurePlug().addAction("AddPlug");
   this->setInformation("Copy the single input data to the output datas");
}
void Copy::action(string ){
   this->structurePlug().addPlugOut(DataGeneric::KEY,"output"+SCD::UtilityString::Any2String(this->structurePlug().plugOut().size()) +".gen");
    this->_v_plug_out.push_back(new CPlugOut(this,DataGeneric::KEY,this->_v_plug_out.size()));
   CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
//    if(this->plugIn()[0]->getState()==CPlug::OLD){
//        this->plugOut()[(int)this->plugOut().size()-1]->getData()->dumpReception(this->plugIn()[0]->getData());
//        this->plugOut()[(int)this->plugOut().size()-1]->setState(CPlug::NEW);
//    }

}
void Copy::exec(){
    for(int i =0;i<(int)this->plugOut().size();i++)
        this->plugOut()[i]->getData()->dumpReception(this->plugIn()[0]->getData());
}

COperator * Copy::clone(){
    return new Copy();
}
