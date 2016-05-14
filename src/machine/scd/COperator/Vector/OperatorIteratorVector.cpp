#include "OperatorIteratorVector.h"

#include<CData.h>
#include "CConnectorDirect.h"
#include<DataBoolean.h>
OperatorIteratorVector::OperatorIteratorVector()
    :_stilldata(false),_index(0),_lastshoot(false)
{
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"data.out");
    this->structurePlug().addPlugOut(DataNumber::KEY,"index.num");



    this->path().push_back("Vector");
    this->setKey("OperatorIteratorVector");
    this->setName("IteratorVectorIn");
    this->setInformation("Iterate over the vector from begin to end every time a NEW . During this itration, boolean state is EMPTY and at the end is NEW with a true value");
}

bool OperatorIteratorVector::executionCondition(){
//    vector<CPlugOut* >::iterator itout;

//    //All ouput plug not NEW
//    for(itout =this->_v_plug_out.begin();itout!=_v_plug_out.end();itout++){
//        if((*itout)->getState()==CPlug::NEW&&(*itout)->isConnected()==true){
//            return false;
//        }
//    }
//    if(_stilldata==true){
//        if(this->plugIn()[1]->getState()==CPlug::NEW)
//            return true;
//        else
//            return false;
//    }
//    else if(_lastshoot==true){
//        if(this->plugIn()[1]->getState()==CPlug::NEW){
//            _lastshoot = false;
//            return true;
//        }
//        else
//            return false;
//    }

//    if(this->plugIn()[0]->getState()==CPlug::NEW)
//        return true;
//    else
//        return false;
    return false;
}

void OperatorIteratorVector::updateMarkingAfterExecution(){

//    this->plugOut()[2]->setState(CPlug::NEW);
//    this->plugOut()[2]->getSenderPlugToPlug()->sendData(this->plugOut()[2]->getData(),this->plugOut()[2]->getState());

//    if(this->plugIn()[1]->getState()==CPlug::NEW){
//        this->plugIn()[1]->setState(CPlug::OLD);
//        this->plugIn()[1]->getSenderPlugToPlug()->sendData(this->plugIn()[1]->getData(),this->plugIn()[1]->getState());
//    }
//    if(_stilldata==true){
//        this->plugOut()[0]->setState(CPlug::NEW);
//        this->plugOut()[0]->getSenderPlugToPlug()->sendData(this->plugOut()[0]->getData(),this->plugOut()[0]->getState());

//    }else{
//        this->plugOut()[1]->setState(CPlug::NEW);
//        this->plugOut()[1]->getSenderPlugToPlug()->sendData(this->plugOut()[1]->getData(),this->plugOut()[1]->getState());
//        this->plugIn()[0]->setState(CPlug::OLD);
//        this->plugIn()[0]->getSenderPlugToPlug()->sendData(this->plugIn()[0]->getData(),this->plugIn()[0]->getState());
//    }
//    this->updateState();
}


void OperatorIteratorVector::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugOut()[1]->setState(CPlug::EMPTY);
    this->plugOut()[2]->setState(CPlug::EMPTY);
//    _stilldata= false;
//    _index= 0;
//    _lastshoot = false ;
}

void OperatorIteratorVector::exec(){

    if(_stilldata==true){
        if(_index<(int)_vec->size()){
            CData * data = _vec->operator [](_index).get();
            this->plugOut()[0]->getData()->dumpReception(data);
            _index++;
        }
        else{
            _stilldata = false;
            _lastshoot = true ;
            this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
        }

    }
    else{

        _vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
                    _index=0;
        _stilldata=true;
        this->exec();
    }
    dynamic_cast<DataNumber *>(this->plugOut()[2]->getData())->setValue(_index);
}

COperator * OperatorIteratorVector::clone(){
    return new OperatorIteratorVector();
}
