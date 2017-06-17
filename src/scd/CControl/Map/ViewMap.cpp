#include "ViewMap.h"
#include "DataMap.h"
#include <CData.h>
#include "DataString.h"

ViewMap::ViewMap(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Map");
    this->setName("ViewMap");
    this->setKey("ViewMap");
    this->structurePlug().addPlugIn(DataMap::KEY,"in.map");

    box = new QTableWidget;
    box->setRowCount(0);
    box->setColumnCount(2);
    QTableWidgetItem* item2 = new QTableWidgetItem("key");
    box->setHorizontalHeaderItem(0,item2);

    QTableWidgetItem* item3 = new QTableWidgetItem("value");
    box->setHorizontalHeaderItem(1,item3);

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    this->setLayout(lay);
    this->setMinimumWidth(300);
}

void ViewMap::geInformation(){
    this->apply();
}

CControl * ViewMap::clone(){
    return new ViewMap();
}


void ViewMap::updatePlugInControl(int , CData* data ){
    box->clear();
    if(DataMap * dmap = dynamic_cast<DataMap *>(data)){
        shared_ptr<map<string,shared_ptr<CData>  > > v = dmap->getData();
        map<string,shared_ptr<CData>  >::iterator it;
        int i = 0;
        for ( it=v->begin() ; it != v->end(); it++ ){
            QTableWidgetItem* item = new QTableWidgetItem((*it).first.c_str());
            string file = (*it).second->toString();
            QTableWidgetItem* item2 = new QTableWidgetItem(file.c_str());
            box->setRowCount(i+1);
            box->setItem(i,0,item);
            box->setItem(i,1,item2);
            i++;
        }
    }
    this->update();
}
