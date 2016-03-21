#include "CGInNumber.h"
#include<DataNumber.h>
CGInNumber::CGInNumber(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Number");
    this->setName("In");
    this->setKey("InNumber");
    this->structurePlug().addPlugOut(DataNumber::KEY,"out.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"in.num");

    applyButton = new QPushButton(tr("&Apply"));
    applyButton->setAutoDefault(false);

    if(!QObject::connect(applyButton, SIGNAL( released  ()),this, SLOT(geInformation()),Qt::DirectConnection)){
        qDebug() << "[WARN] Can't connect CDatasEditor and button" ;
    }

//    label = new QLabel;
//    label->setText("0");

//    lineEdit = new QLineEdit;
//    lineEdit->setAlignment(Qt::AlignRight);
//    lineEdit->setText("0");
//    QDoubleValidator *FloatVal = new QDoubleValidator( this );
//    lineEdit->setValidator( FloatVal );

//    QVBoxLayout *buttonLayout = new QVBoxLayout;
//    buttonLayout->addWidget(label);
//    buttonLayout->addWidget(applyButton);
//    buttonLayout->addWidget(lineEdit);
//    test = false;
//    this->setLayout(buttonLayout);
}

void CGInNumber::geInformation(){

    test = true;
    this->apply();
}

CControl * CGInNumber::clone(){
    return new CGInNumber();
}

void CGInNumber::updatePlugInControl(int indexplugin,CData* data,CPlug::State state){
    if(DataNumber * number = dynamic_cast<DataNumber *>(data))
    {
        DataNumber::Type v = number->getValue();
        QString c;
        c.setNum(v);
        lineEdit->setText(c);
        this->update();
    }
}
string CGInNumber::toString(){
    return "";//lineEdit->text().toStdString();
}
void CGInNumber::fromString(string str){
//    lineEdit->setText(str.c_str());
    test = true;
}
void CGInNumber::apply(){
    if(test==true)
    {
//        DataNumber * number = new DataNumber;
//        number->setValue(lineEdit->text().toDouble());
//        this->sendPlugOutControl(0,number,CPlug::NEW);
    }
}
