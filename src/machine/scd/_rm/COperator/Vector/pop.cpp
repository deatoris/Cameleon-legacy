#include "pop.h"


pop::pop(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugOut(CDataGeneric::KEY,"element.gen");
    this->structurePlug().addPlugOut(DataNumber::KEY,"index.num");
    this->path().push_back("Vector");
    this->setKey("vectorpopvector");
    this->setName("pop");
}

void pop::exec(){
    vector<CData*> vec = static_cast<DataVector *>(this->plugIn()[0]->getData())->getValue();

    CData* element = vec[vec.size()-1];
    vec.pop_back();

    CDataGeneric* gen = static_cast<CDataGeneric *>(this->plugOut()[0]->getData());
    DataNumber* indexout = static_cast<DataNumber *>(this->plugOut()[1]->getData());
    indexout->setValue(vec.size());

    gen->dumpReception(element);
}

COperator * pop::clone(){
    return new pop();
}
