#include "controlprogressbar.h"

#include "DataNumber.h"
ControlProgressBar::ControlProgressBar(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Number");
    this->setName("ProgressBar");
    this->setKey("ControlProgressBar");
    this->structurePlug().addPlugIn(DataNumber::KEY,"min.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"max.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"current.num");
    this->setInformation("Progress bar defined by a range [min,max] and a current value");
    label = new QProgressBar;
    label->setMinimum(0);
    label->setMaximum(10);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(label);
    this->setLayout(lay);
}



CControl * ControlProgressBar::clone(){
    return new ControlProgressBar();
}

void ControlProgressBar::updatePlugInControl(int indexplugin, CData* data){
    if(indexplugin==0){
        int value = dynamic_cast<DataNumber*>(data)->getValue();
        label->setMinimum(value);
        update();
    }
    if(indexplugin==1){
        int value = dynamic_cast<DataNumber*>(data)->getValue();
        label->setMaximum(value);
        update();
    }
    if(indexplugin==2){
        int value = dynamic_cast<DataNumber*>(data)->getValue();
        label->setValue(value);
        update();
    }
}
