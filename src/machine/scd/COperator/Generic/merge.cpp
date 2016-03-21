#include "merge.h"
#include<CPlug.h>
#include<CConnectorDirect.h>
#include<CUtilitySTL.h>
#include<CMachine.h>
#include<CLogger.h>

OperatorMergeGeneric::OperatorMergeGeneric(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input0.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input1.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output.gen");
    this->path().push_back("Control-Flow");
    this->setKey("merge");
    this->setName("Merge");
    this->setInformation("Copy input data at new to output data.\n<b>Execution Condition:</b>\n one input at new,\n output to OLD.\nAction:\ncan add input connector.");
        this->structurePlug().addAction("AddPlug");
}
void OperatorMergeGeneric::action(string key){
    QString s = key.c_str();
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input"+SCD::UtilityString::Any2String(this->structurePlug().plugIn().size())+".gen");
    this->_v_plug_in.push_back(new CPlugIn(this,DataGeneric::KEY,this->_v_plug_in.size()));
    CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
}

void OperatorMergeGeneric::exec(){

    for(int i =0;i<(int)this->plugIn().size();i++){
        if(this->plugIn()[i]->getState()==CPlug::NEW)
        {
            try{
                this->plugOut()[0]->getData()->dumpReception(this->plugIn()[i]->getData());
            }
            catch(std::string msg){
                this->error("The generic plug is cast at the first execution. Then, the specefic data type must be the same. Here; you don't satisfy this condition since we have this message :"+ msg);
            }
        }
    }
}
bool OperatorMergeGeneric::executionCondition(){



    if(this->plugOut()[0]->getState()==CPlug::NEW&&this->plugOut()[0]->isConnected()==true){
        return false;
    }
    for(int i =0;i<(int)this->plugIn().size();i++){
        if(this->plugIn()[i]->getState()==CPlug::NEW)
            return true;
    }
    return false;
}

COperator * OperatorMergeGeneric::clone(){
    return new OperatorMergeGeneric();
}
void OperatorMergeGeneric::updateMarkingAfterExecution(){
    vector<CPlugOut* >::iterator itout;
    //All ouput plug becomes NEW
    for(itout =this->_v_plug_out.begin();itout!=this->_v_plug_out.end();itout++){
        (*itout)->setState(CPlug::NEW);
        try{
            (*itout)->send();
        }
        catch(std::string msg){
            this->error(msg);
        }
    }
    //All input plug becomes (NEW,OLD)->OLD
    vector<CPlugIn* >::iterator itin;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        if((*itin)->getState()==CPlug::NEW){
            (*itin)->setState(CPlug::OLD);
            (*itin)->send();
            itin=_v_plug_in.end()-1;
        }
    }
    this->updateState();
}

