#include "OperatorLoadVector.h"

#include<DataVector.h>
#include<DataString.h>

OperatorLoadVector::OperatorLoadVector(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataVector::KEY,"m.vec");
    this->path().push_back("Vector");
    this->setKey("OperatorLoadVector");
    this->setName("LoadVector");
    this->setInformation("Load the Vector in the given file");
}

void OperatorLoadVector::exec(){
    string  file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    vector<shared_ptr<CData> >   * m = new vector<shared_ptr<CData> >;
    ifstream  out(file.c_str());
    if(out.is_open()){
        out>>* m;
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(DataVector::DATAVECTOR(m));


}

COperator * OperatorLoadVector::clone(){
    return new OperatorLoadVector;
}
