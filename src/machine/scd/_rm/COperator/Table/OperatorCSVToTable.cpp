#include "OperatorCSVToTable.h"

#include<DataTable.h>
#include<DataString.h>
OperatorCSVToTable::OperatorCSVToTable(){
    this->structurePlug().addPlugIn(DataString::KEY,"csv.str");
    this->structurePlug().addPlugOut(DataTable::KEY,"m.Table");
    this->path().push_back("Table");
    this->setKey("OperatorCSVToTable");
    this->setName("CSVToTable");
    this->setInformation("Load the Table from the given csv format string");
}

void OperatorCSVToTable::exec(){
    string  file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    Table   * m = new Table;
    QString sFile = file.c_str();
    QStringList sline = sFile.split("\n");

    //process size
    int i = 0;
    foreach(QString line, sline){
        int j = 0;
        QStringList elements = line.split(";");
        if(elements.size()>m->sizeCol()){;
            m->resize(elements.size(),sline.size());
        }
        foreach(QString element, elements){
            m->operator ()(j,i) = element.toStdString();
            j++;
        }
        i++;
    }
    dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData((m));


}

COperator * OperatorCSVToTable::clone(){
    return new OperatorCSVToTable;
}
