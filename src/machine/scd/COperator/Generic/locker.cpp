#include "locker.h"
#include<CConnectorDirect.h>
#include<CUtilitySTL.h>
#include<CMachine.h>
locker::locker(){

    this->structurePlug().addPlugIn(DataGeneric::KEY,"lockin.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"lockout.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"out0.gen");
    this->path().push_back("Control-Flow");
    this->setKey("OperatorLocker");
    this->setName("Locker");
        this->structurePlug().addAction("AddPlug");
    this->setInformation("Can be executed if  lock state=NEW\n the exex is just the copy of the data and the state from in_i to out_i");
}
void locker::action(string key){
    QString s = key.c_str();
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in"+SCD::UtilityString::Any2String(this->structurePlug().plugIn().size()-1) +".gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"out"+SCD::UtilityString::Any2String(this->structurePlug().plugOut().size()-1) +".gen");
    this->_v_plug_in.push_back(new CPlugIn(this,DataGeneric::KEY,this->_v_plug_in.size()));
    this->_v_plug_out.push_back(new CPlugOut(this,DataGeneric::KEY,this->_v_plug_out.size()));
    CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
}
void locker::exec(){
    for(int i =0;i<(int)this->plugOut().size();i++)
        if(this->plugIn()[i]->isDataAvailable()==true)
            this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i]->getData());
}

bool locker::executionCondition(){
    if(this->plugIn()[0]->getState()!=CPlug::NEW)
        return false;
    for(int i =0;i<(int)this->plugOut().size();i++){
        if(this->plugOut()[i]->getState()==CPlug::NEW && this->plugOut()[i]->isConnected()==true)
            return false;
    }
    return true;
}
void locker::updateMarkingAfterExecution(){
    vector<CPlugOut* >::iterator itout;
    vector<CPlugIn* >::iterator itin;
    //All ouput plug becomes NEW
    for(itout =this->_v_plug_out.begin(),  itin =_v_plug_in.begin() ;itout!=this->_v_plug_out.end();itout++,itin++){
        (*itout)->setState((*itin)->getState());
        try{
            if((*itin)->getState()!=CPlug::EMPTY)
                (*itout)->send();
        }
        catch(std::string msg){
            this->error(msg);
        }
    }
    //All input plug becomes (NEW,OLD)->OLD
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        if((*itin)->getState()==CPlug::NEW)
            (*itin)->setState(CPlug::OLD);
        (*itin)->send();
    }
    this->updateState();
}

COperator * locker::clone(){
    return new locker();
}
