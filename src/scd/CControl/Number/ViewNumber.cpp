#include "ViewNumber.h"
#include "DataNumber.h"
ViewNumber::ViewNumber(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Number");
    this->setName("ViewNumber");
    this->setKey("NumberView");
    this->structurePlug().addPlugIn(DataNumber::KEY,"in.num");

    label = new QLabel;
    label->setText("0");

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(label);
    this->setLayout(lay);
}

void ViewNumber::geInformation(){
    this->apply();
}

CControl * ViewNumber::clone(){
    return new ViewNumber();
}

void ViewNumber::updatePlugInControl(int , CData* data ){
    DataNumber * num = dynamic_cast<DataNumber *>(data);
    if(num!=0){
        label->setText(QString::number(num->getValue()));
    }else{
        this->error("error in connection data type");
    }
    this->update();
}


