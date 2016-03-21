#include "settableelement.h"
#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>
setTableElement::setTableElement(){
    this->structurePlug().addPlugIn(DataTable::KEY,"tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY,"col.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"row.num");
    this->structurePlug().addPlugIn(DataString::KEY,"elt.str");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("setTableElement");
    this->setName("SetValueTable");
    this->setInformation("set the value at the given position row col");
}

void setTableElement::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int col = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int row = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();

    string str = dynamic_cast<DataString *>(this->plugIn()[3]->getData())->getValue();

    try{
        tab->operator ()(col,row)= str;
        this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(tab);
    }
    catch(std::string msg){
        this->error(msg);
    }
}

COperator * setTableElement::clone(){
    return new setTableElement();
}

