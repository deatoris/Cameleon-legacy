#include "DataByFileToDataByAddress.h"

#include "CData.h"
#include<CDataByFile.h>
DataByFileToDataByAddress::DataByFileToDataByAddress(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"out.gen");
    this->path().push_back("Generic");
    this->setKey("DataByFileToDataByAddress");
    this->setName("DataByFileToDataByAddress");
}

void DataByFileToDataByAddress::exec(){
    CData *d  =this->plugIn()[0]->getData()->morpherMode(CData::BYADDRESS);
    this->plugOut()[0]->getData()->dumpReception(d);
}
COperator * DataByFileToDataByAddress::clone(){
    return new DataByFileToDataByAddress();
}

