#include "endloopteleport.h"
#include "loopteleport.h"
#include<CUtilitySTL.h>
#include "CConnectorDirect.h"
#include "DataNumber.h"
#include "DataBoolean.h"
#include "CMachine.h"
endforteleport::endforteleport(){
    _endloop =false;
    this->structurePlug().addPlugIn(DataNumber::KEY,"connectionwithforloop.num");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"data_0_iteration_i.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"data_0_iteration_exitloop.gen");

    this->path().push_back("Control-Flow");
    this->setKey("endforteleport");
    this->setName("endforteleport");
        this->structurePlug().addAction("AddPlug");
    this->setInformation("");
}
void endforteleport::action(string key){
    QString s = key.c_str();
    this->structurePlug().addPlugIn(DataGeneric::KEY,"data_"+SCD::UtilityString::Any2String(this->structurePlug().plugIn().size()-1) +"_iteration_i.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"data_"+SCD::UtilityString::Any2String(this->structurePlug().plugOut().size()) +"_iteration_exitloop.gen");
    this->_v_plug_in.push_back(new CPlugIn(this,DataGeneric::KEY,this->_v_plug_in.size()));
    this->_v_plug_out.push_back(new CPlugOut(this,DataGeneric::KEY,this->_v_plug_out.size()));
    CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
}


bool endforteleport::executionCondition(){
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
void endforteleport::exec(){
    int id = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    vector<CData*> v_c;
    for(int i=1;i<(int)this->plugIn().size();i++)
        v_c.push_back(this->plugIn()[i]->getData());
    COperator * op = CMachineSingleton::getInstance()->getProcessor()->getProcessor()->composition()[id].first;
    if(forteleport * opt = dynamic_cast<forteleport * >(op)){
        if(opt->sendData(v_c)==true){
            _endloop =false;
        }else{
            for(int i =0;i<(int)this->plugOut().size();i++)
                this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i+1]->getData());
            _endloop =true ;
        }
    }
}
void endforteleport::updateMarkingAfterExecution(){
    if(_endloop==false){
        //All input plug becomes (NEW,OLD)->OLD
        vector<CPlugIn* >::iterator itin;
        for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
            (*itin)->setState(CPlug::OLD);
            (*itin)->send();
        }
        this->updateState();
    }else{
        COperator::updateMarkingAfterExecution();
    }
}
COperator * endforteleport::clone(){
    return new endforteleport();
}
