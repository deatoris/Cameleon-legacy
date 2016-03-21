#include "ViewTable.h"
#include "DataTable.h"
ViewTable::ViewTable(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Table");
    this->setName("ViewTable");
    this->setKey("ViewTable");
    this->structurePlug().addPlugIn(DataTable::KEY,"in.table");

    box = new QTableWidget;
    box->setRowCount(0);
    box->setColumnCount(0);

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    this->setLayout(lay);
    this->setMinimumWidth(300);
}


CControl * ViewTable::clone(){
    return new ViewTable();
}



void ViewTable::updatePlugInControl(int, CData* data){

        box->clear();

        if(DataTable * table = dynamic_cast<DataTable *>(data)){
            shared_ptr<Table> st = table->getData();
            box->setRowCount(st->sizeRow());
            box->setColumnCount(st->sizeCol());
            for(int i=0;i<st->sizeRow();i++){
                for(int j=0;j<st->sizeCol();j++){
                    QTableWidgetItem* item = new QTableWidgetItem(st->operator ()(j,i).c_str());
                    box->setItem(i,j,item);
                }
            }
            for(int j=0;j<st->sizeCol();j++){
                string header = st->getHeader(j);
                QTableWidgetItem* itemName = new QTableWidgetItem(header.c_str());
                box->setHorizontalHeaderItem(j,itemName);
            }
        }
        this->update();

}
