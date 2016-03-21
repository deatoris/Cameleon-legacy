#include "gettableelement.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>

getTableElement::getTableElement(){
    this->structurePlug().addPlugIn(DataTable::KEY,"tablein.tab");
        this->structurePlug().addPlugIn(DataNumber::KEY,"col.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"row.num");

    this->structurePlug().addPlugOut(DataString::KEY,"elt.str");
    this->path().push_back("Table");
    this->setKey("getTableElement");
    this->setName("GetValueTable");
    this->setInformation("get the element at the given position row and col ");
}

void getTableElement::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();

    int col = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int row = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();

    try{
        string str = tab->operator ()(col,row);
        dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(str);
    }
    catch(std::string msg){
        this->error(msg);
    }


}

COperator * getTableElement::clone(){
    return new getTableElement();
}
