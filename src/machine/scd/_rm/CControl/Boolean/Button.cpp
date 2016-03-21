#include "Button.h"

#include "DataBoolean.h"
#include "DataString.h"
EditorButton::EditorButton(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Boolean");
    this->setName("Button");
    this->setKey("EditorButton");
    this->structurePlug().addPlugIn(DataString::KEY,"name.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"clicked.bool");

    button = new QPushButton(tr("Apply"));

    if(!QObject::connect(button, SIGNAL(clicked()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //        //qDebug << "[WARN] Can't connect EditorButton and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(button);
    test = false;
    this->setLayout(lay);
}

void EditorButton::geInformation(){
    test = true;
    this->apply();
}

CControl * EditorButton::clone(){
    return new EditorButton();
}

string EditorButton::toString(){
    return button->text().toStdString();
}

void EditorButton::fromString(string str){
    if(str!=""){
        button->setText(str.c_str());
    }
}

void EditorButton::updatePlugInControl(int ,CData* data){

    if(DataString * b = dynamic_cast<DataString *>(data))
    {
        button->setText(b->getValue().c_str());
        this->update();
    }

}

void EditorButton::apply(){
    if(test==true)
    {
        test = false;
        DataBoolean * b = new DataBoolean;
        b->setValue(true);
        this->sendPlugOutControl(0,b,CPlug::NEW);
    }
}
