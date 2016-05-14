#include "OperatorSetColTable.h"
#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>

OperatorSetColTable::OperatorSetColTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,  "tablein.tab");
    this->structurePlug().addPlugIn(DataTable::KEY, "tableinsert.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY, "Col.num");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("OperatorSetColTable");
    this->setName("SetColTable");
    this->setInformation("Insert tableinsert in tablein  at the given Col");
}


void OperatorSetColTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    shared_ptr<Table> tabin= dynamic_cast<DataTable *>(this->plugIn()[1]->getData())->getData();
    int Col= dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();


    try{
        Table * t = tabin.get();
        tab->setCol(Col,*t);
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(tab));
    }
    catch(std::string msg){
        this->error(msg);
    }


}

COperator * OperatorSetColTable::clone(){
    return new OperatorSetColTable();
}
