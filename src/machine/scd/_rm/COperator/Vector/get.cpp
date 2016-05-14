#include "get.h"


get::get(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataNumber::KEY,"index.num");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"element.gen");
    //this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("vectorgetvector");
    this->setName("GetElementVector");
    this->setInformation("Get the data given by the index");
}

void get::exec(){
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();

    int ind = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();

    if(ind<(int)vec->size())
    {
        DataGeneric* gen = dynamic_cast<DataGeneric *>(this->plugOut()[0]->getData());
        gen->dumpReception(vec->operator [](ind).get());
    }
    else
    {
        this->error("Out of range");
        return;
    }
}

COperator * get::clone(){
    return new get();
}
