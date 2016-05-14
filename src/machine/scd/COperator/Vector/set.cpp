#include "set.h"

set::set(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataNumber::KEY,"index.num");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"element.gen");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("vectorsetvector");
    this->setName("SetDataVector");
    this->setInformation("Set the data element of the vector at the given index");
}

void set::exec(){
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    int ind = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    if(ind<(int)vec->size())
    {
        CData* data = this->plugIn()[2]->getData()->clone();
        data->dumpReception(this->plugIn()[2]->getData());
        vec->operator [](ind) = shared_ptr<CData>(data);
    }
    else{
        this->error("Out of range");
        return;
    }

    this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(vec);

}

COperator * set::clone(){
    return new set();
}
