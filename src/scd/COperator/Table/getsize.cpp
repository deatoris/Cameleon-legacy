#include "getsize.h"
#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>
getSizeTable::getSizeTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,"table.tab");
        this->structurePlug().addPlugOut(DataNumber::KEY,"colsize.num");
    this->structurePlug().addPlugOut(DataNumber::KEY,"rowsize.num");

    this->path().push_back("Table");
    this->setKey("getSizeTable");
    this->setName("SizeTable");
    this->setInformation("Get the size of the input table");
}

void getSizeTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(tab->sizeCol());
    dynamic_cast<DataNumber *>(this->plugOut()[1]->getData())->setValue(tab->sizeRow());

}

COperator * getSizeTable::clone(){
    return new getSizeTable();
}
