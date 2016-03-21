#include "OperatorGetRowTable.h"

#include "OperatorGetRowTable.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>

OperatorGetRowTable::OperatorGetRowTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,  "tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY, "Rowmin.num");
    this->structurePlug().addPlugIn(DataNumber::KEY, "Rowmax.num");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("OperatorGetRowTable");
    this->setName("GetRowTable");
    this->setInformation("Extract the table between Rowmin and Rowmax with Rowmax=Rowmin as default value");
}
void OperatorGetRowTable::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugIn()[1]->setState(CPlug::EMPTY);
    if(this->plugIn()[2]->isConnected()==false)
        this->plugIn()[2]->setState(CPlug::OLD);
    else
        this->plugIn()[2]->setState(CPlug::EMPTY);

    this->plugOut()[0]->setState(CPlug::EMPTY);
}

void OperatorGetRowTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int Rowmin = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int Rowmax = Rowmin ;
    if(this->plugIn()[2]->isDataAvailable()==true)
        Rowmax = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();

    try{
        Table * tout= new Table;
        *tout = tab->getRow(Rowmin,Rowmax);
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(tout));
    }
    catch(std::string msg){
        this->error(msg);
    }


}

COperator * OperatorGetRowTable::clone(){
    return new OperatorGetRowTable();
}
