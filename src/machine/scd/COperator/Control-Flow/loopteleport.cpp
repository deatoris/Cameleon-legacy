#include "loopteleport.h"
#include<CUtilitySTL.h>
#include<CMachine.h>
#include "CConnectorDirect.h"
#include "DataNumber.h"
#include "DataBoolean.h"
forteleport::forteleport(){
    _firstiteration = true;
    _callbyexitloop = false;
    this->structurePlug().addPlugIn(DataNumber::KEY,"nbrloop.num");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"data_0_iteration_0.gen");

    this->structurePlug().addPlugOut(DataNumber::KEY,"connectionwithforloopend.num");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"data_0_iteration_i.gen");

    this->path().push_back("Control-Flow");
    this->setKey("forteleport");
    this->setName("forteleport");
    this->structurePlug().addAction("AddPlug");
    this->setInformation("");
}
void forteleport::action(string key){
    QString s = key.c_str();
    this->structurePlug().addPlugIn(DataGeneric::KEY,"data_"+SCD::UtilityString::Any2String(this->structurePlug().plugIn().size()-1) +"_iteration_0.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"data_"+SCD::UtilityString::Any2String(this->structurePlug().plugOut().size()-1) +"_iteration_i.gen");
    this->_v_plug_in.push_back(new CPlugIn(this,DataGeneric::KEY,this->_v_plug_in.size()));
    this->_v_plug_out.push_back(new CPlugOut(this,DataGeneric::KEY,this->_v_plug_out.size()));
    CMachineSingleton::getInstance()->getProcessor()->updateOperatorStructure(this->getId(),this->structurePlug());
}

bool forteleport::sendData(vector<CData*> v_c){
    if((int)v_c.size()!=(int)this->plugOut().size()-1){
        this->error("The number of outplug plug of forteleport must be equal to the number of outplug plug of endforteleport");
        return false;
    }
    else{
        if(_nbr_iteration>0){
            _callbyexitloop = true;
            for(int i=1;i<(int)this->plugOut().size();i++)
                this->plugOut()[i]->getData()->dumpReception(v_c[i-1]);
            this->updateState();
            return true;
        }else{
            _firstiteration=true;
            return false;
        }
    }
}

void forteleport::exec(){
    if(_firstiteration == true){
        dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(this->getId());
        _nbr_iteration  = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
        _nbr_iteration--;
        for(int i=1;i<(int)this->plugOut().size();i++)
            this->plugOut()[i]->getData()->dumpReception(this->plugIn()[i]->getData());
        _firstiteration = false;
        _callbyexitloop = false;
    }else{
        _callbyexitloop = false;
        dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(this->getId());
        _nbr_iteration--;
    }
}

bool forteleport::executionCondition(){
    vector<CPlugOut* >::iterator itout;
    bool allempty=true;
    for(itout =_v_plug_out.begin();itout!=_v_plug_out.end();itout++){
        if((*itout)->getState()!=CPlug::EMPTY){
            allempty = false;
        }
    }
    if( allempty==true)
        _firstiteration =true;
    if(_firstiteration ==true){
        if(this->plugOut()[0]->getState()==CPlug::NEW)
            this->plugOut()[0]->setState(CPlug::OLD);
        return COperator::executionCondition();
    }else if(_callbyexitloop == true){

        return true;
    }
    return false;
}

COperator * forteleport::clone(){
    return new forteleport();
}
