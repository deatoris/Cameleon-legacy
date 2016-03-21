#include "OperatorLoadTable.h"

#include<DataTable.h>
#include<DataString.h>

OperatorLoadTable::OperatorLoadTable(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataTable::KEY,"m.Table");
    this->path().push_back("Table");
    this->setKey("OperatorLoadTable");
    this->setName("LoadTable");
    this->setInformation("Load the Table in the given file");
}

void OperatorLoadTable::exec(){
    string  file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    Table   * m = new Table;
    ifstream  out(file.c_str());
    if(out.is_open()){
        out>>* m;
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(m));


}

COperator * OperatorLoadTable::clone(){
    return new OperatorLoadTable;
}
