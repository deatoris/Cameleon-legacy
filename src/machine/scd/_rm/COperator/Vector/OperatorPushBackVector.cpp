#include "OperatorPushBackVector.h"
#include<DataVector.h>
OperatorPushBackVector::OperatorPushBackVector(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(CData::KEY,"data.gen");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("OperatorPushBackVector");
    this->setName("PushBackVector");
    this->setInformation("Push back data in the vector");
}

void OperatorPushBackVector::exec(){
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();

    vec->push_back(shared_ptr<CData>(this->plugIn()[1]->getData()->copy()));
    this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(vec);

}

COperator * OperatorPushBackVector::clone(){
    return new OperatorPushBackVector();
}
