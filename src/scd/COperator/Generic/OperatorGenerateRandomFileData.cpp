#include "OperatorGenerateRandomFileData.h"
#include<CDataByFile.h>
#include "CGProject.h"
OperatorGenerateRandomFileData::OperatorGenerateRandomFileData(){
    this->structurePlug().addPlugIn(DataGeneric::KEY,"in.gen");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"out.gen");

    this->path().push_back("Data");
    this->setKey("OperatorGenerateRandomFileData");
    this->setName("CopyInRandomFile");
    this->setInformation("Copy the output to the input such that the output file is randomly thrown\n");

}

void OperatorGenerateRandomFileData::exec(){
    CData *d  =this->plugIn()[0]->getData();

    string file = CGProject::getInstance()->getTmpPath().toStdString();
    int i = rand();
    std::ostringstream oss;
    oss << i;
    file += oss.str();
    file +=d->getExtension();

    CData *dd= d->morpherMode( CData::BYCOPY,file );
    CData *ddd= dd->morpherMode( CData::BYFILE,file );

    this->plugOut()[0]->getData()->dumpReception(ddd);
    delete dd;
    delete ddd;
}
COperator * OperatorGenerateRandomFileData::clone(){
    return new OperatorGenerateRandomFileData();
}
