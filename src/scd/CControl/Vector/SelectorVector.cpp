#include "SelectorVector.h"

#include "ViewVector.h"

#include "DataVector.h"
#include <CData.h>
#include<CUtilitySTL.h>
#include<CGlossary.h>
SelectorVector::SelectorVector(QWidget * parent)
    :CControl(parent),_index(0)
{
    this->path().push_back("Vector");
    this->setName("ListBox");
    this->setKey("SelectorVector");
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"out.gen");

    box = new QComboBox;
    box->addItem("empty");
    box->setCurrentIndex(0);

    if(!QObject::connect(box, SIGNAL(currentIndexChanged(int)),this, SLOT(geInformation(int)),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect SelectorVector and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void SelectorVector::updatePlugInControl(int , CData* data){

    box->clear();
    _v_key.clear();
    DataVector * num = dynamic_cast<DataVector *>(data);
    DataVector::DATAVECTOR v = num->getData();
    for ( int i =0 ; i <(int) v->size(); i++ ){
        box->addItem((*v)[i]->toString().c_str(),(*v)[i]->getKey().c_str());
        _v_key.push_back((*v)[i]->getKey());
    }
    if(_index>=0&&_index<(int)v->size()){
        box->setCurrentIndex ( _index);
        test = true;
    }

    this->update();

}

void SelectorVector::geInformation(int index){
    _index = index;
    test = true;
    this->apply();
}

CControl * SelectorVector::clone(){
    return new SelectorVector();
}

string SelectorVector::toString(){
    return SCD::UtilityString::Any2String(_index);
}

void SelectorVector::fromString(string str){
    SCD::UtilityString::String2Any(str,_index);
}

void SelectorVector::apply(){
    if(test==true)
    {
        if(_index>=0&&_index<(int)_v_key.size()){
            CData * data =  CGlossarySingletonClient::getInstance()->createData(_v_key[_index]);
            data->fromString(box->currentText().toStdString());
            this->sendPlugOutControl(0,data,CPlug::NEW);
        }
    }
}
