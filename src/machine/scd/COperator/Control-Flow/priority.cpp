#include "priority.h"

priority::priority(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input1.gen");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input2.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output1.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output2.gen");
    this->path().push_back("Control-Flow");
    this->setKey("priority");
    this->setName("priority");
    this->setInformation("copy input1 to output1 in prior to copy input2 to output2");
}

void priority::exec(){
    this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
    this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
}

COperator * priority::clone(){
    return new priority();
}
