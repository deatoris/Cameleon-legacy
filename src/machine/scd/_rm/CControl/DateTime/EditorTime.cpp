#include "EditorTime.h"

#include "DataBoolean.h"
#include "DataDate.h"
EditorTime::EditorTime(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("DateTime");
    this->setName("EditorTime");
    this->setKey("EditorTime");
    this->structurePlug().addPlugOut(DataDate::KEY,"out.time");

    box = new QTimeEdit;

    if(!QObject::connect(box, SIGNAL(currentIndexChanged(QString)),this, SLOT(geInformation()),Qt::DirectConnection)){
//        //qDebug << "[WARN] Can't connect EditorTime and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void EditorTime::geInformation(){
    test = true;
    this->apply();
}

CControl * EditorTime::clone(){
    return new EditorTime();
}

string EditorTime::toString(){
    return "";//box->currentText().toStdString();
}

void EditorTime::fromString(string ){
//    if(str.compare("TRUE") == 0){
//        box->setCurrentIndex(0);
//    }else{
//        box->setCurrentIndex(1);
//    }
    test = true;
}

void EditorTime::apply(){
//    if(test==true)
//    {
//        DataBoolean * b = new DataBoolean;
//        if(box->currentText().compare("TRUE") == 0){
//            b->setValue(true);
//        }else{
//            b->setValue(false);
//        }
//        this->sendPlugOutControl(0,b,CPlug::NEW);
//    }
}
