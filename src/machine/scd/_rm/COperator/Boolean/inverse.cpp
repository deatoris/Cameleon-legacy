#include "inverse.h"

#include "inverse.h"

inverse::inverse(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"in.bool");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Boolean");
    this->setKey("booleaninverse");
    this->setName("InverseBoolean");
    this->setInformation("out= true for in1=false, false otherwise ");
}

void inverse::exec(){
    DataBoolean * d1 = dynamic_cast<DataBoolean *>(this->plugIn()[0]->getData());
    DataBoolean * dret = dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData());

    bool b1 = d1->getValue();
    if(b1){
        dret->setValue(false);
    }else{
        dret->setValue(true);
    }
}

COperator * inverse::clone(){
    return new inverse();
}

