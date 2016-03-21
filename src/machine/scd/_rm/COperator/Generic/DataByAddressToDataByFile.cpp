#include "DataByAddressToDataByFile.h"
#include<CDataByPointer.h>
DataByAddressToDataByFile::DataByAddressToDataByFile(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"out.gen");
    this->path().push_back("Generic");
    this->setKey("DataByAddressToDataByFile");
    this->setName("DataByAddressToDataByFile");
    this->setInformation("In the flow, the data will be sent by file");

}

void DataByAddressToDataByFile::exec(){
    CData *d  =this->plugIn()[0]->getData()->morpherMode(CData::BYFILE);
    this->plugOut()[0]->getData()->dumpReception(d);
}
COperator * DataByAddressToDataByFile::clone(){
    return new DataByAddressToDataByFile();
}
