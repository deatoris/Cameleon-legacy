#include "async.h"
#include<CUtilitySTL.h>
#include<CMachine.h>
#include<CConnectorDirect.h>
async::async(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input1.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input2.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output1.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output2.gen");
    this->path().push_back("Control-Flow");
    this->setKey("async");
    this->setName("Asynchrone");
    this->setInformation("Exec if at least one input plug state is NEW");
    this->structurePlug().addAction("AddPlug");
}
void async::action(string key){
    QString s = key.c_str();
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input"+SCD::ConvertString::Any2String(this->structurePlug().plugIn().size()) +".gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output"+SCD::ConvertString::Any2String(this->structurePlug().plugOut().size()) +".gen");
    this->_v_plug_in.push_back(new CPlugIn(this,DataGeneric::KEY,this->_v_plug_in.size()));
    this->_v_plug_out.push_back(new CPlugOut(this,DataGeneric::KEY,this->_v_plug_out.size()));
    CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
}
void async::exec(){
    for(int i =0;i<(int)this->plugOut().size();i++){
        if(this->plugIn()[i]->isDataAvailable()==true)
            this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i]->getData());
    }
}
bool async::executionCondition(){
    for(int i =0;i<(int)this->plugOut().size();i++){
        if(this->plugOut()[i]->getState()==CPlug::NEW && this->plugOut()[i]->isConnected()==true)
            return false;
    }
    for(int i =0;i<(int)this->plugIn().size();i++){
        if(this->plugIn()[i]->getState()==CPlug::NEW)
            return true;
    }
    return false;
}

void async::updateMarkingAfterExecution(){

    for(int i=0;i<(int)this->plugOut().size();i++){
        if(this->plugIn()[i]->getState()!=CPlug::EMPTY){
            this->plugOut()[i]->setState(CPlug::NEW);
            try{
                this->plugOut()[i]->send();
            }
            catch(std::string msg){
                this->error(msg);
            }
        }
    }
    //All input plug becomes (NEW,OLD)->OLD
    vector<CPlugIn* >::iterator itin;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        if((*itin)->getState()!=CPlug::EMPTY){
        (*itin)->setState(CPlug::OLD);
        (*itin)->send();
        }
    }
    this->updateState();
}
COperator * async::clone(){
    return new async();
}
