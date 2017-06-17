#include "EditorNumber.h"
#include "DataNumber.h"
EditorNumber::EditorNumber(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Number");
    this->setName("EditorNumber");
    this->setKey("EditorNumber");
    this->structurePlug().addPlugOut(DataNumber::KEY,"out.num");
    this->setInformation("Press enter to set the value");
    lineEdit = new QLineEdit;
    lineEdit->setAlignment(Qt::AlignRight);
    lineEdit->setText("0");
    QDoubleValidator *FloatVal = new QDoubleValidator( this );
    lineEdit->setValidator( FloatVal );



    QVBoxLayout *buttonLayout = new QVBoxLayout;
    buttonLayout->addWidget(lineEdit);
    test = false;
    this->setLayout(buttonLayout);
    //    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void EditorNumber::keyPressEvent ( QKeyEvent * keyEvent ){
    test=true;
    CControl::keyPressEvent(keyEvent);
}

CControl * EditorNumber::clone(){
    return new EditorNumber();
}

string EditorNumber::toString(){
    return lineEdit->text().toStdString();
}
void EditorNumber::fromString(string str){
    lineEdit->setText(str.c_str());
    test = true;
}
void EditorNumber::apply(){
    if(test==true&&this->isPlugOutConnected(0)==true)
    {
        DataNumber * number = new DataNumber;
        if(lineEdit->text()!=""){
            number->setValue(lineEdit->text().toDouble());
            this->sendPlugOutControl(0,number,CPlug::NEW);
        }
    }
}
