#include "if.h"
#include "DataBoolean.h"
#include "CConnectorDirect.h"
If::If(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"boolean.bool");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"if.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"else.gen");

    this->path().push_back("Control-Flow");
    this->setKey("If");
    this->setName("If");
    this->setInformation("Copy input to if for boolean=true or Copy input to else for boolean=false");
}

void If::exec(){
    DataBoolean * b = dynamic_cast<DataBoolean *>(this->plugIn()[0]->getData());

    ifvalue = b->getValue();
    if(b->getValue())
        this->plugOut()[0]->getData()->dumpReception(this->plugIn()[1]->getData());
    else
        this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
}

COperator * If::clone(){
    return new If();
}


void If::updateMarkingAfterExecution(){
    if(ifvalue){
        this->plugOut()[0]->setState(CPlug::NEW);
        this->plugOut()[0]->send();
    }else{
        this->plugOut()[1]->setState(CPlug::NEW);
        this->plugOut()[1]->send();
    }

    vector<CPlugIn* >::iterator itin;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        (*itin)->setState(CPlug::OLD);
        (*itin)->send();
    }
    this->updateState();
}
