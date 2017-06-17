#include "EditorDate.h"

#include "DataBoolean.h"
#include "DataDate.h"
EditorDate::EditorDate(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("DateTime");
    this->setName("EditorDate");
    this->setKey("EditorDate");
    this->structurePlug().addPlugOut(DataDate::KEY,"out.date");

    box = new QDateEdit;

    if(!QObject::connect(box, SIGNAL(currentIndexChanged(QString)),this, SLOT(geInformation()),Qt::DirectConnection)){
//        //qDebug << "[WARN] Can't connect EditorDate and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void EditorDate::geInformation(){
    test = true;
    this->apply();
}

CControl * EditorDate::clone(){
    return new EditorDate();
}

string EditorDate::toString(){
    return "";// box->currentText().toStdString();
}

void EditorDate::fromString(string ){
//    if(str.compare("TRUE") == 0){
//        box->setCurrentIndex(0);
//    }else{
//        box->setCurrentIndex(1);
//    }
    test = true;
}

void EditorDate::apply(){
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
