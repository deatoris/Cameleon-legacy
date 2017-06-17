#include "OperatorSortVector.h"
#include <algorithm>
class SortData {
public:
  bool operator() (shared_ptr<CData> data1,shared_ptr<CData> data2) { return (data1->toString()<data2->toString());}
};


OperatorSortVector::OperatorSortVector(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("OperatorSortVector");
    this->setName("SortVector");
    this->setInformation("Sort the input vector following the member toString of the data");
}

void OperatorSortVector::exec(){
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();

    sort(vec->begin(),vec->end(),SortData());
    this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(vec);

}

COperator * OperatorSortVector::clone(){
    return new OperatorSortVector();
}
