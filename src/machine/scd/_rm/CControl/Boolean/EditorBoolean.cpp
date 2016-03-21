#include "EditorBoolean.h"
#include "DataBoolean.h"
EditorBoolean::EditorBoolean(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Boolean");
    this->setName("EditorBoolean");
    this->setKey("editorboolean");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");

    box = new QComboBox;
    box->addItem("TRUE");
    box->addItem("FALSE");
    box->setCurrentIndex(1);

    if(!QObject::connect(box, SIGNAL(currentIndexChanged(QString)),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorBoolean and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void EditorBoolean::geInformation(){
    test = true;
    this->apply();
}

CControl * EditorBoolean::clone(){
    return new EditorBoolean();
}

string EditorBoolean::toString(){
    return box->currentText().toStdString();
}

void EditorBoolean::fromString(string str){
    if(str.compare("TRUE") == 0){
        box->setCurrentIndex(0);
    }else{
        box->setCurrentIndex(1);
    }
    test = true;
}

void EditorBoolean::apply(){
    if(test==true&&this->isPlugOutConnected(0)==true)
    {
        DataBoolean * b = new DataBoolean;
        if(box->currentText().compare("TRUE") == 0){
            b->setValue(true);
        }else{
            b->setValue(false);
        }
        this->sendPlugOutControl(0,b,CPlug::NEW);
    }
}
