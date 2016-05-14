#include "getcolumnname.h"
#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>
getTableHeaderCol::getTableHeaderCol(){
    this->structurePlug().addPlugIn(DataTable::KEY,"tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY,"col.num");
    this->structurePlug().addPlugOut(DataString::KEY,"elt.str");
    this->path().push_back("Table");
    this->setKey("getTableHeaderCol");
    this->setName("GetHeaderCol");
}

void getTableHeaderCol::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int col = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();


    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(tab->getHeader(col));
}

COperator * getTableHeaderCol::clone(){
    return new getTableHeaderCol();
}
