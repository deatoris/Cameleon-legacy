#include "OperatorSaveMap.h"

#include<DataMap.h>
#include<DataString.h>
#include<DataBoolean.h>
OperatorSaveMap::OperatorSaveMap(){
    this->structurePlug().addPlugIn(DataMap::KEY,"in.bool");
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Map");
    this->setKey("OperatorSaveMap");
    this->setName("SaveMap");
    this->setInformation("Save the Map value in the given file, out= false for bad writing, true otherwise");
}

void OperatorSaveMap::exec(){
    DataMap::DATAMAP inmap = dynamic_cast<DataMap *>(this->plugIn()[0]->getData())->getData();
    string  file = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();

    ofstream  out(file.c_str());
    if(out.is_open()){
        out<<*(inmap);
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);


}

COperator * OperatorSaveMap::clone(){
    return new OperatorSaveMap;
}
