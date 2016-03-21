#include "value.h"

value::value(){
    this->structurePlug().addPlugIn(DataMap::KEY,"in1.map");
    this->structurePlug().addPlugIn(DataString::KEY,"key.str");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"value.gen");
    this->path().push_back("Map");
    this->setKey("mapvaluemap");
    this->setName("GetDataMap");
    this->setInformation("Get the data given by the key");
}

void value::exec(){
    DataMap::DATAMAP inmap = dynamic_cast<DataMap *>(this->plugIn()[0]->getData())->getData();
    string key = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();
    CData* outvalue = this->plugOut()[0]->getData();
    if(inmap->operator [](key).get() != 0){
        outvalue->dumpReception(inmap->operator [](key).get());
    }else{
        this->error("key doesn't exist !");
    }
}

COperator * value::clone(){
    return new value();
}
