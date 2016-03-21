#include "resize.h"
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
resize::resize(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataNumber::KEY,"index.num");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("vectorresizevector");
    this->setName("ResizeVector");
    this->setInformation("Set the data element of the vector at the given index");
}

void resize::exec(){
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    int index1 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    if(index1>=0){
        if(index1<(int)vec->size())
            vec->resize(index1);
        else
        {
            int diff = index1-(int)vec->size();
            for(int i=0;i<diff;i++){
                shared_ptr<CData> d(new DataGeneric);
                vec->push_back(d);
            }
        }
    }
    else
        this->error("index1>=0 ");

    this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(vec);
}

COperator * resize::clone(){
    return new resize();
}
