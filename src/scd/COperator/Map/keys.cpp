#include "keys.h"

keys::keys(){
    this->structurePlug().addPlugIn(DataMap::KEY,"m.map");
    this->structurePlug().addPlugOut(DataVector::KEY,"keys.vec");
    this->path().push_back("Map");
    this->setKey("mapkeysmap");
    this->setName("KeysMap");
    this->setInformation("Create a vector containing DataString elements with the keys of the input map");
}

void keys::exec(){
    DataMap::DATAMAP inmap = dynamic_cast<DataMap *>(this->plugIn()[0]->getData())->getData();

    vector<shared_ptr<CData> > * v = new vector<shared_ptr<CData> > ;

    map<string,shared_ptr<CData>  >::iterator it;
    for ( it=inmap->begin() ; it != inmap->end(); it++ ){
        DataString* strdata = new DataString(); //ATTENTION
        strdata->setValue((*it).first);
        v->push_back(shared_ptr<CData>(strdata));
    }

   dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(DataVector::DATAVECTOR(v));
}

COperator * keys::clone(){
    return new keys();
}
