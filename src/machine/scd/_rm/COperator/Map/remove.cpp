#include "remove.h"

removemapelement::removemapelement(){
    this->structurePlug().addPlugIn(DataMap::KEY,"min.map");
    this->structurePlug().addPlugIn(DataString::KEY,"key.str");
    this->structurePlug().addPlugOut(DataMap::KEY,"mout.map");
    this->path().push_back("Map");
    this->setKey("mapremovemap");
    this->setName("RemoveMap");
    this->setInformation("Remove the element with the given key");
}

void removemapelement::exec(){
    DataMap::DATAMAP inmap = dynamic_cast<DataMap *>(this->plugIn()[0]->getData())->getData();
    string key = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();
    inmap->erase(key);
    this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
    dynamic_cast<DataMap *>(this->plugOut()[0]->getData())->setData(inmap);
}

COperator * removemapelement::clone(){
    return new removemapelement();
}
