#include "OperatorCSVToTable.h"

#include<DataTable.h>
#include<DataString.h>
OperatorCSVToTable::OperatorCSVToTable(){
    this->structurePlug().addPlugIn(DataString::KEY,"csv.str");
    this->structurePlug().addPlugIn(DataString::KEY,"separator.str");
    this->structurePlug().addPlugIn(DataString::KEY,"returnline.str");
    this->structurePlug().addPlugOut(DataTable::KEY,"m.Table");
    this->path().push_back("Table");
    this->setKey("OperatorCSVToTable");
    this->setName("CSVToTable");
    this->setInformation("Load the Table from the given csv format string");
}

void OperatorCSVToTable::exec(){
    string  file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string  separator = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();
    string  returnline = dynamic_cast<DataString *>(this->plugIn()[2]->getData())->getValue();
    Table   * m = new Table;
    QString sFile = file.c_str();
    QStringList sline = sFile.split(returnline.c_str());

    //process size
    int i = 0;
    foreach(QString line, sline){
        int j = 0;
        QStringList elements = line.split(separator.c_str());
        if(elements.size()>m->sizeCol()){;
            m->resize(elements.size(),sline.size());
        }
        foreach(QString element, elements){
            m->operator ()(j,i) = element.toStdString();
            j++;
        }
        i++;
    }
    dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(shared_ptr<Table>(m));


}

COperator * OperatorCSVToTable::clone(){
    return new OperatorCSVToTable;
}
