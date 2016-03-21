#include "EditorDateTime.h"

#include "DataBoolean.h"
#include "DataDateTime.h"
EditorDateTime::EditorDateTime(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("DateTime");
    this->setName("EditorDateTime");
    this->setKey("EditorDateTime");
    this->structurePlug().addPlugOut(DataDateTime::KEY,"out.datetime");

    box = new QDateTimeEdit;

    if(!QObject::connect(box, SIGNAL(editingFinished()),this, SLOT(geInformation()),Qt::DirectConnection)){
//        //qDebug << "[WARN] Can't connect EditorDateTime and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void EditorDateTime::geInformation(){
    test = true;
    this->apply();
}

CControl * EditorDateTime::clone(){
    return new EditorDateTime();
}

string EditorDateTime::toString(){
    return box->dateTime().toString("dd.MM.yyyy hh:mm:ss").toStdString();//box->currentText().toStdString();
}

void EditorDateTime::fromString(string datetime){

    test = true;
    box->setDateTime(QDateTime::fromString(datetime.c_str(),"dd.MM.yyyy hh:mm:ss"));
}

void EditorDateTime::apply(){
    if(test==true)
    {
        DataDateTime * b = new DataDateTime;
        this->sendPlugOutControl(0,b,CPlug::NEW);
    }
}
