#include "OperatorRmRowTable.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>

OperatorRmRowTable::OperatorRmRowTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,  "tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY, "Rawmin.num");
    this->structurePlug().addPlugIn(DataNumber::KEY, "Rawmax.num");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("OperatorRmRawTable");
    this->setName("RmRowTable");
    this->setInformation("Remove the Rawumns of the input table the between Rawmin and Rawmax with Rawmax=Rawmin as default value");
}
void OperatorRmRowTable::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugIn()[1]->setState(CPlug::EMPTY);
    if(this->plugIn()[2]->isConnected()==false)
        this->plugIn()[2]->setState(CPlug::OLD);
    else
        this->plugIn()[2]->setState(CPlug::EMPTY);

    this->plugOut()[0]->setState(CPlug::EMPTY);
}


void OperatorRmRowTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int Rawmin = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int Rawmax = Rawmin ;
    if(this->plugIn()[2]->isDataAvailable()==true)
        Rawmax = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();

    try{
        tab->deleteRow(Rawmin,Rawmax);
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(tab));
    }
    catch(std::string msg){
        this->error(msg);
    }
}
COperator * OperatorRmRowTable::clone(){
    return new OperatorRmRowTable();
}
