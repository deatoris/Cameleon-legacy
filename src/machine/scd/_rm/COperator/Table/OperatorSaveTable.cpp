#include "OperatorSaveTable.h"

#include<DataTable.h>
#include<DataString.h>
#include<DataBoolean.h>
OperatorSaveTable::OperatorSaveTable(){
    this->structurePlug().addPlugIn(DataTable::KEY,"in.tab");
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Table");
    this->setKey("OperatorSaveTable");
    this->setName("SaveTable");
    this->setInformation("Save the Table value in the given file, out= false for bad writing, true otherwise");
}

void OperatorSaveTable::exec(){
    shared_ptr<Table> inTable = dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    string  file = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();

    ofstream  out(file.c_str());
    if(out.is_open()){
        out<<*(inTable);
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);


}

COperator * OperatorSaveTable::clone(){
    return new OperatorSaveTable;
}
