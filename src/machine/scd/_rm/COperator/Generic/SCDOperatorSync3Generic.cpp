#include "SCDOperatorSync3Generic.h"

SCDOperatorSync3Generic::SCDOperatorSync3Generic(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input1.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input2.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input3.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output1.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output2.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output3.gen");
    this->path().push_back("Generic");
    this->setKey("SCDOperatorSync3Generic");
    this->setName("Sync3");
}
void SCDOperatorSync3Generic::exec(){
        this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
        this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
    this->plugOut()[2]->getData()->dumpReception(this->plugIn()[2]->getData());
}

bool SCDOperatorSync3Generic::executionCondition(){
    if((this->plugOut()[0]->getState()==CPlug::NEW
            && this->plugOut()[0]->isConnected()==true)
            || (this->plugOut()[1]->getState()==CPlug::NEW
                && this->plugOut()[1]->isConnected()==true)
                || (this->plugOut()[2]->getState()==CPlug::NEW
                    && this->plugOut()[2]->isConnected()==true)

                )
    {
        return false;
    }
    if(this->plugIn()[0]->getState()!=CPlug::NEW || this->plugIn()[1]->getState()!=CPlug::NEW || (this->plugIn()[2]->getState()!=CPlug::NEW&& this->plugIn()[2]->isConnected()==true) ){
        return false;
    }else{
        return true;
    }
}

COperator * SCDOperatorSync3Generic::clone(){
    return new SCDOperatorSync3Generic();
}
