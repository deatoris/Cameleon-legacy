#include "OperatorIteratorOutVector.h"

#include<CData.h>
#include "CConnectorDirect.h"
#include<DataBoolean.h>
OperatorIteratorOutVector::OperatorIteratorOutVector()
    :_stilldata(false),_index(0),_lastshoot(false)
{
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataNumber::KEY,"index.num");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"data.out");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");



    this->path().push_back("Vector");
    this->setKey("OperatorIteratorOutVector");
    this->setName("IteratorVectorOut");
    this->setInformation("Iterate over the vector from begin to end every time a NEW . During this itration, boolean state is EMPTY and at the end is NEW with a true value");
}

bool OperatorIteratorOutVector::executionCondition(){
//    for(int i =0;i<(int)this->plugOut().size();i++){
//        if(this->plugOut()[i]->getState()==CPlug::NEW && this->plugOut()[i]->isConnected()==true)
//            return false;
//    }
//    for(int i =0;i<(int)this->plugIn().size();i++){
//        if(this->plugIn()[i]->getState()!=CPlug::NEW)
//            return false;
//    }
    return false;
}
void OperatorIteratorOutVector::updateMarkingAfterExecution(){

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


void OperatorIteratorOutVector::initState(){
//    this->plugIn()[0]->setState(CPlug::EMPTY);
//    this->plugIn()[1]->setState(CPlug::EMPTY);
//    this->plugOut()[0]->setState(CPlug::EMPTY);
//    this->plugOut()[1]->setState(CPlug::EMPTY);
//        this->plugOut()[2]->setState(CPlug::EMPTY);
//    _stilldata= false;
//    _index= 0;
//    _lastshoot = false ;
}

void OperatorIteratorOutVector::exec(){

//    if(_stilldata==true){
//        if(_index<(int)_vec->size()){
//            CData * data = _vec->operator [](_index).get();
//            this->plugOut()[0]->getData()->dumpReception(data);
//            _index++;
//        }
//        else{
//            _stilldata = false;
//            _lastshoot = true ;
//            this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
//        }

//    }
//    else{

//        _vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
//                    _index=0;
//        _stilldata=true;
//        this->exec();
//    }
//    dynamic_cast<DataNumber *>(this->plugOut()[2]->getData())->setValue(_index);
}

COperator * OperatorIteratorOutVector::clone(){
    return new OperatorIteratorOutVector();
}

