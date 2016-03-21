#include "and.h"

And::And(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"in1.bool");
    this->structurePlug().addPlugIn(DataBoolean::KEY,"in2.bool");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Boolean");
    this->setKey("booleanand");
    this->setName("AndBoolean");
    this->setInformation("out= true for in1=true and in2=true, false otherwise ");
}

void And::exec(){
    DataBoolean * d1 = dynamic_cast<DataBoolean *>(this->plugIn()[0]->getData());
    DataBoolean * d2 = dynamic_cast<DataBoolean *>(this->plugIn()[1]->getData());
    DataBoolean * dret = dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData());

    bool b1 = d1->getValue();
    bool b2 = d2->getValue();
    if(b1 && b2){
        dret->setValue(true);
    }else{
        dret->setValue(false);
    }
}

COperator * And::clone(){
    return new And();
}
