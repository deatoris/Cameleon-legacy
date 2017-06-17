#include "setcolumnname.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>
setTableHeaderCol::setTableHeaderCol(){
    this->structurePlug().addPlugIn(DataTable::KEY,"tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY,"col.num");
    this->structurePlug().addPlugIn(DataString::KEY,"elt.str");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("setTableHeaderCol");
    this->setName("setHeaderCol");
}

void setTableHeaderCol::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int col = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    string str = dynamic_cast<DataString *>(this->plugIn()[2]->getData())->getValue();
    tab->setHeader(col,str);
    dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(tab);
}

COperator * setTableHeaderCol::clone(){
    return new setTableHeaderCol();
}
