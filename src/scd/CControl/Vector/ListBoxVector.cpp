#include "ListBoxVector.h"

#include "DataVector.h"
#include "DataString.h"

ListBoxVector::ListBoxVector(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Vector");
    this->setName("ListBox2");
    this->setKey("ListBoxVector");
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugOut(DataString::KEY,"data.str");
    this->structurePlug().addPlugOut(DataString::KEY,"type.str");

    box = new QComboBox;
//    box->addItem("empty");
//    box->setCurrentIndex(0);
    if(!QObject::connect(box, SIGNAL(activated(int)),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect ListBoxVector and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void ListBoxVector::updatePlugInControl(int , CData* data){

    box->clear();

    DataVector * num = dynamic_cast<DataVector *>(data);
    DataVector::DATAVECTOR v = num->getData();


    for ( int i =0 ; i < (int)v->size(); i++ ){
        box->addItem((*v)[i]->toString().c_str(),(*v)[i]->getKey().c_str());
    }
    if(box->count()!=0)
        box->setCurrentIndex(0);
    this->update();
}

void ListBoxVector::geInformation(){
    test = true;
    this->apply();
}

CControl * ListBoxVector::clone(){
    return new ListBoxVector();
}

string ListBoxVector::toString(){
    return QString::number(box->currentIndex()).toStdString();
}

void ListBoxVector::fromString(string s){
    QString s2(s.c_str());
    box->setCurrentIndex(s2.toInt());
    test = true;
}

void ListBoxVector::apply(){
    if(test==true)
    {
        int index = box->currentIndex();
        if(index!=-1){
            QString stype = box->itemData(index).toString();
            QString sdata = box->currentText();
            DataString *data = new DataString();
            DataString *type = new DataString();
            data->setValue(sdata.toStdString());
            type->setValue(stype.toStdString());
            this->sendPlugOutControl(0,data,CPlug::NEW);
            this->sendPlugOutControl(1,type,CPlug::NEW);

        }
    }
}
