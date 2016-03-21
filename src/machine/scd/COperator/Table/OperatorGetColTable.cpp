#include "OperatorGetColTable.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>

OperatorGetColTable::OperatorGetColTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,  "tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY, "colmin.num");
    this->structurePlug().addPlugIn(DataNumber::KEY, "colmax.num");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("OperatorGetColTable");
    this->setName("GetColTable");
    this->setInformation("Extract the table between colmin and colmax with colmax=colmin as default value");
}
void OperatorGetColTable::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugIn()[1]->setState(CPlug::EMPTY);
    if(this->plugIn()[2]->isConnected()==false)
        this->plugIn()[2]->setState(CPlug::OLD);
    else
        this->plugIn()[2]->setState(CPlug::EMPTY);

    this->plugOut()[0]->setState(CPlug::EMPTY);
}

void OperatorGetColTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int colmin = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int colmax = colmin ;
    if(this->plugIn()[2]->isDataAvailable()==true)
        colmax = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();

    try{
        Table * tout= new Table;
        *tout = tab->getCol(colmin,colmax);
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(tout));
    }
    catch(std::string msg){
        this->error(msg);
    }


}

COperator * OperatorGetColTable::clone(){
    return new OperatorGetColTable();
}
