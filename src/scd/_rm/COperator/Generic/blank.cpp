#include "blank.h"
#include<CGlossary.h>
#include<CData.h>
#include<DataString.h>
blank::blank(){
    this->structurePlug().addPlugIn(DataString::KEY,"datatype.str");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"blank.gen");
    this->path().push_back("Generic");
    this->setKey("genericblank");
    this->setName("Blank");
    this->setInformation("Create a blank data following the data type");
}

void blank::exec(){
    string datatype = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();

    CData * d = CGlossarySingletonServer::getInstance()->createData(datatype);
    if(d!=NULL){
        d->setPlug(this->plugOut()[0]);
        DataGeneric* gen = dynamic_cast<DataGeneric *>(this->plugOut()[0]->getData());
        gen->dumpReception(d);
    }else{
        this->error("data type doesn't exist"+datatype);
        return;
    }
}

COperator * blank::clone(){
    return new blank();
}
