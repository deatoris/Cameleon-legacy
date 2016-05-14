#include "EditorMap.h"

#include <CData.h>
#include "DataString.h"

EditorMap::EditorMap(QWidget * parent)
    :CControl(parent),_data(new map<string,shared_ptr<CData>  >)
{
    this->path().push_back("Map");
    this->setName("EditorMap");
    this->setKey("EditorMap");
    this->structurePlug().addPlugOut(DataMap::KEY,"out.map");

    box = new QTableWidget;
    box->setRowCount(0);
    box->setColumnCount(2);
    QTableWidgetItem* item2 = new QTableWidgetItem("key");
    box->setHorizontalHeaderItem(0,item2);

    QTableWidgetItem* item3 = new QTableWidgetItem("value");
    box->setHorizontalHeaderItem(1,item3);

    button = new QPushButton("add element");
    save = new QPushButton("save");

    if(!QObject::connect(button, SIGNAL(clicked()),this, SLOT(addelement()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorMap and box" ;
    }

    if(!QObject::connect(save, SIGNAL(clicked()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorMap and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(button);
    lay->addWidget(box);
    lay->addWidget(save);
    test = false;
    this->setLayout(lay);
    this->setMinimumWidth(300);
}

void EditorMap::geInformation(){
    test = true;
    this->apply();
}

void EditorMap::addelement(){
    int count = box->rowCount();
    box->setRowCount(count+1);
    QTableWidgetItem* item = new QTableWidgetItem("key");
    QTableWidgetItem* item2 = new QTableWidgetItem("");
    box->setItem(count,0,item);
    box->setItem(count,1,item2);
    this->update();
}

CControl * EditorMap::clone(){
    return new EditorMap();
}

string EditorMap::toString(){
    std::ostringstream oss;
    oss << *(_data);
    return oss.str();
}

void EditorMap::fromString(string str){
    if(str.compare("")!=0){
        _data->clear();
        std::istringstream iss(str);
        iss >> *(_data);
        map<string,shared_ptr<CData>  >::iterator it;
        for ( it=_data->begin() ; it != _data->end(); it++ ){
            int count = box->rowCount();
            box->setRowCount(count+1);
            QTableWidgetItem* item = new QTableWidgetItem((*it).first.c_str());
            QTableWidgetItem* item2 = new QTableWidgetItem((*it).second->toString().c_str());
            box->setItem(count,0,item);
            box->setItem(count,1,item2);
        }
    }
    test = true;
}

void EditorMap::apply(){
    if(test==true)
    {
        _data->clear();
        int count = box->rowCount();
        for(int i=0;i<count;i++){
            QTableWidgetItem* item = box->item(i,0);
            QTableWidgetItem* item2 = box->item(i,1);

            DataString* str2 = new DataString;
            str2->setValue(item2->text().toStdString());
            shared_ptr<CData> elt(str2);
            _data->operator [](item->text().toStdString())=elt;
        }
        DataMap * map = new DataMap;
        map->setData(_data);
        this->sendPlugOutControl(0,map,CPlug::NEW);

    }
}
