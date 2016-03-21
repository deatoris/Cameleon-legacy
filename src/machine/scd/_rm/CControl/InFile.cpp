#include "InFile.h"

#include "InFile.h"
#include<DataString.h>
#include<DataBoolean.h>
#include<DataVector.h>
#include <CGProject.h>
InFile::InFile(QWidget * parent)
    :CControl(parent),_file("")
{
    this->path().push_back("FileSystem");
    this->setName("OpenFile");
    this->setKey("InFile");
    this->structurePlug().addPlugIn(DataVector::KEY,"out.vec");
    this->structurePlug().addPlugIn(DataString::KEY,"name.str");
    this->structurePlug().addPlugIn(DataBoolean::KEY,"record.bool");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");

    applyButton = new QPushButton(tr("Browse"));
    applyButton->setAutoDefault(false);

    if(!QObject::connect(applyButton, SIGNAL( released  ()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(applyButton);
    this->setLayout(buttonLayout);
    filter = "";
    record = true;
}

void InFile::updatePlugInControl(int id, CData* data){
    if(id == 0 ){
        if(DataVector * vec = dynamic_cast<DataVector *>(data)){
            DataVector::DATAVECTOR v = vec->getData();
            filter = "";
            for ( int i =0 ; i <(int) v->size(); i++ ){
                DataString* vv= dynamic_cast<DataString *> (v->operator[](i).get());

                filterList << vv->getValue().c_str();
                filter+= "*"+QString::fromStdString((*v)[i]->toString().c_str())+" ";
            }
        }
    }
    if(id == 1 ){
        if(DataString * b = dynamic_cast<DataString *>(data))
        {
            applyButton->setText(b->getValue().c_str());
            this->update();
        }
    }
    if(id == 2){
        if(DataBoolean * b = dynamic_cast<DataBoolean *>(data)){
            record = b->getValue();
        }
    }
}


void InFile::geInformation(){
    if(filterList.size() == 1 && filterList.value(0).compare(".dir") == 0){
        _file =QFileDialog::getExistingDirectory(this, tr("Open Dir"),CGProject::getInstance()->getPath(),QFileDialog::ShowDirsOnly).toStdString();
    }else{
        _file =QFileDialog::getOpenFileName(this, tr("Open File"),CGProject::getInstance()->getPath(),filter).toStdString();
    }
    this->apply();
}

CControl * InFile::clone(){
    return new InFile();
}

string InFile::toString(){

    if(record)
        return _file;
    else
        return "";
}

void InFile::fromString(string str){
    _file = str;
}

void InFile::apply(){
    if(_file!=""&&this->isPlugOutConnected(0)==true)
    {
        DataString * data = new DataString;
        data->setValue(_file);
        this->sendPlugOutControl(0,data,CPlug::NEW);
    }
}

OutFile::OutFile(QWidget * parent)
    :InFile(parent){
    this->setName("SaveFile");
    this->setKey("OutFile");
}

CControl * OutFile::clone(){
    return new OutFile;
}

void OutFile::geInformation(){
    _file =QFileDialog::getSaveFileName(this, tr("Save File"),CGProject::getInstance()->getPath(),filter).toStdString();
    this->apply();
}
