#include "ViewVector.h"

#include "DataVector.h"
#include <CData.h>

ViewVector::ViewVector(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Vector");
    this->setName("ViewVector");
    this->setKey("ViewVector");
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
//    this->structurePlug().addPlugOut(DataGeneric::KEY,"out.gen");

    box = new QListWidget;
    box->insertItem(0,"empty");

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    this->setLayout(lay);
}

void ViewVector::updatePlugInControl(int , CData* data ){
    box->clear();
    DataVector * num = dynamic_cast<DataVector *>(data);
    DataVector::DATAVECTOR v = num->getData();
    for ( int i =0 ; i <(int) v->size(); i++ ){
        //qDebug<<"insert into view vector";
        QListWidgetItem* item = new QListWidgetItem();
        item->setData(0,(*v)[i]->toString().c_str());
        item->setData(1,(*v)[i]->getKey().c_str());
        box->insertItem(i,item);
        this->update();
    }
}


CControl * ViewVector::clone(){
    return new ViewVector();
}


