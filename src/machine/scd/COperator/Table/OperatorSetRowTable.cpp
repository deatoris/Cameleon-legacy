#include "OperatorSetRowTable.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>

OperatorSetRowTable::OperatorSetRowTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,  "tablein.tab");
    this->structurePlug().addPlugIn(DataTable::KEY, "tableinsert.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY, "row.num");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("OperatorSetRowTable");
    this->setName("SetRowTable");
    this->setInformation("Insert tableinsert in tablein  at the given row");
}

void OperatorSetRowTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    shared_ptr<Table> tabin= dynamic_cast<DataTable *>(this->plugIn()[1]->getData())->getData();
    int row= dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();

    try{
        Table * t = tabin.get();
        tab->setRow(row,*t);
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(tab));
    }
    catch(std::string msg){
        this->error(msg);
    }


}

COperator * OperatorSetRowTable::clone(){
    return new OperatorSetRowTable();
}
