#include "OperatorRmColTable.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataTable.h>

OperatorRmColTable::OperatorRmColTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,  "tablein.tab");
    this->structurePlug().addPlugIn(DataNumber::KEY, "Colmin.num");
    this->structurePlug().addPlugIn(DataNumber::KEY, "Colmax.num");
    this->structurePlug().addPlugOut(DataTable::KEY,"tableout.tab");
    this->path().push_back("Table");
    this->setKey("OperatorRmColTable");
    this->setName("RmColTable");
    this->setInformation("Remove the columns of the input table the between Colmin and Colmax with Colmax=Colmin as default value");
}
void OperatorRmColTable::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugIn()[1]->setState(CPlug::EMPTY);
    if(this->plugIn()[2]->isConnected()==false)
        this->plugIn()[2]->setState(CPlug::OLD);
    else
        this->plugIn()[2]->setState(CPlug::EMPTY);

    this->plugOut()[0]->setState(CPlug::EMPTY);
}

void OperatorRmColTable::exec(){
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    int Colmin = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    int Colmax = Colmin ;
    if(this->plugIn()[2]->isDataAvailable()==true)
        Colmax = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();

    try{
        tab->deleteCol(Colmin,Colmax);
        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(tab));
    }
    catch(std::string msg){
        this->error(msg);
    }


}

COperator * OperatorRmColTable::clone(){
    return new OperatorRmColTable();
}
