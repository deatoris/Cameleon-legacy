#include "counter.h"
#include "DataNumber.h"

counter::counter(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"input.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output.gen");
    this->structurePlug().addPlugOut(DataNumber::KEY,"count.num");
    this->path().push_back("Control-Flow");
    this->setKey("counter");
    this->setName("Counter");
    this->setInformation("Count the number of execution of this operator for the beginning of play mode");
    count = 0;
}

void counter::exec(){
    count++;
    this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());
    dynamic_cast<DataNumber *>(this->plugOut()[1]->getData())->setValue(count);

}

COperator * counter::clone(){
    return new counter();
}

void counter::initState(){
    COperator::initState();
    count = 0;
}
