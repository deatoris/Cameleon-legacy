#include "resizetable.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>
ResizeTable::ResizeTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,"tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY,"colsize.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"rowsize.num");

    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("ResizeTable");
    this->setName("ResizeTable");
    this->setInformation("Resize the input table");
}
void ResizeTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int sizecol = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int sizeraw = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();
    try{
        tab->resize(sizecol,sizeraw);
        this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(tab);
    }
    catch(std::string msg){
        this->error(msg);
    }
}

COperator * ResizeTable::clone(){
    return new ResizeTable();
}
