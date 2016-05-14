#include "EditorString.h"
#include "DataString.h"
EditorString::EditorString(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("String");
    this->setName("EditorStringLine");
    this->setKey("LineEditorString");
    this->structurePlug().addPlugOut(DataString::KEY,"out.str");

    lineEdit = new QLineEdit;
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setText("");

    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(lineEdit);
    test = false;
    this->setLayout(buttonLayout);
}
void EditorString::keyPressEvent ( QKeyEvent * keyEvent ){
    test=true;
    CControl::keyPressEvent(keyEvent);
}
CControl * EditorString::clone(){
    return new EditorString();
}

string EditorString::toString(){
    return lineEdit->text().toStdString();
}
void EditorString::fromString(string str){
    lineEdit->setText(str.c_str());
    test = true;
}
void EditorString::apply(){
    if(test==true&&this->isPlugOutConnected(0)==true)
    {
        DataString * number = new DataString;
        number->setValue(lineEdit->text().toStdString());
        this->sendPlugOutControl(0,number,CPlug::NEW);
    }
}
