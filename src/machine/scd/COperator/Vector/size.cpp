#include "size.h"

size::size(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugOut(DataNumber::KEY,"size.num");
    this->path().push_back("Vector");
    this->setKey("vectorsizevector");
    this->setName("SizeVector");
    this->setInformation("Size of the input vector");
}

void size::exec(){
    DataVector::DATAVECTOR vec= dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    DataNumber* gen = dynamic_cast<DataNumber *>(this->plugOut()[0]->getData());
    gen->setValue(vec->size());
}

COperator * size::clone(){
    return new size();
}
