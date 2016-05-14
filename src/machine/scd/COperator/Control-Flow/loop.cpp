#include "loop.h"
#include "CConnectorDirect.h"
#include "DataNumber.h"
#include "DataBoolean.h"
loop::loop(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"max.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"it.num");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"end.bool");
    this->path().push_back("Control-Flow");
    this->setKey("loop");
    this->setName("Loop");
    this->setInformation("for(int it=0;it<max;it++), max.num is OLD and end = true when it==max");
    count = 0;
}

void loop::exec(){
    double max = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    if(count>=max-1){
        dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(count);
        dynamic_cast<DataBoolean *>(this->plugOut()[1]->getData())->setValue(true);
    }else{
        dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(count);
        dynamic_cast<DataBoolean *>(this->plugOut()[1]->getData())->setValue(false);
    }
    count++;
}

void loop::updateMarkingAfterExecution(){
    double max = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();

    if(this->plugIn()[0]->getState()==CPlug::NEW && count>=max){
        this->plugIn()[0]->setState(CPlug::OLD);
        this->plugIn()[0]->send();
        count = 0;
    }else{
        this->plugIn()[0]->setState(CPlug::NEW);
        this->plugIn()[0]->send();
    }

    this->plugOut()[0]->setState(CPlug::NEW);
    this->plugOut()[0]->send();
    this->plugOut()[1]->setState(CPlug::NEW);
    this->plugOut()[1]->send();

    this->updateState();
}

COperator * loop::clone(){
    return new loop();
}
