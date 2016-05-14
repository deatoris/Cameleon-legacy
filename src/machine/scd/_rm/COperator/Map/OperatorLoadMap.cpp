#include "OperatorLoadMap.h"

#include<DataMap.h>
#include<DataString.h>

OperatorLoadMap::OperatorLoadMap(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataMap::KEY,"m.map");
    this->path().push_back("Map");
    this->setKey("OperatorLoadMap");
    this->setName("LoadMap");
    this->setInformation("Load the map in the given file");
}

void OperatorLoadMap::exec(){
    string  file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    map<string,shared_ptr<CData> >   * m = new map<string,shared_ptr<CData> >;
    ifstream  out(file.c_str());
    if(out.is_open()){
        out>>* m;
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataMap *>(this->plugOut()[0]->getData())->setData(DataMap::DATAMAP(m));


}

COperator * OperatorLoadMap::clone(){
    return new OperatorLoadMap;
}
