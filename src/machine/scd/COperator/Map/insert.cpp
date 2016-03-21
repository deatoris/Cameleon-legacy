#include "insert.h"
#include<CGlossary.h>

insert::insert(){
    this->structurePlug().addPlugIn(DataMap::KEY,"min.map");
    this->structurePlug().addPlugIn(DataString::KEY,"key.str");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"dataelt.gen");
    this->structurePlug().addPlugOut(DataMap::KEY,"mout.map");
    this->path().push_back("Map");
    this->setKey("mapinsertmap");
    this->setName("InsertMap");
    this->setInformation("Insert the element data in the map with the given key");
}

void insert::exec(){
    DataMap::DATAMAP inmap = dynamic_cast<DataMap *>(this->plugIn()[0]->getData())->getData();
    string key = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();
    CData* genvalue = this->plugIn()[2]->getData();

    CData* genvalue2 = genvalue->clone();
    genvalue2->dumpReception(genvalue);

    inmap->operator [](key)=shared_ptr<CData>(genvalue2);

    this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
    dynamic_cast<DataMap *>(this->plugOut()[0]->getData())->setData(inmap);
}

COperator * insert::clone(){
    return new insert();
}
