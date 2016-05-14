#include "OperatorTableToCSV.h"

#include<DataTable.h>
#include<DataString.h>
#include<DataBoolean.h>
OperatorTableToCSV::OperatorTableToCSV(){
    this->structurePlug().addPlugIn(DataTable::KEY,"in.tab");
    this->structurePlug().addPlugOut(DataString::KEY,"csv.str");
    this->path().push_back("Table");
    this->setKey("OperatorTableToCSV");
    this->setName("TableToCSV");
    this->setInformation("Return a string representing the full text CSV file");
}

void OperatorTableToCSV::exec(){
    shared_ptr<Table>  inTable = dynamic_cast<DataTable *>(this->plugIn()[0]->getData())->getData();
    QString  file = "";
    int sizeCol = inTable->sizeCol();
    int sizeRow = inTable->sizeRow();
    for(int i=0;sizeRow>i;i++){
        for(int j=0;sizeCol>j;j++){
            string str = inTable->operator ()(j,i);
            if(j!=0) file = file+";"+str.c_str();
            if(j==0&&i!=0) file = file+"\n";
            if(j==0) file = file+str.c_str();
        }
    }
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setData(&file.toStdString());
}

COperator * OperatorTableToCSV::clone(){
    return new OperatorTableToCSV;
}
