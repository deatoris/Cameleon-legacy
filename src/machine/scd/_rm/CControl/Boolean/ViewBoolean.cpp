#include "ViewBoolean.h"
#include "DataBoolean.h"
ViewBoolean::ViewBoolean(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Boolean");
    this->setName("ViewBoolean");
    this->setKey("BooleanView");
    this->structurePlug().addPlugIn(DataBoolean::KEY,"in.bool");



    label = new QLabel;
    label->setText("FALSE");

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(label);
    this->setLayout(lay);
//    this->rect()
}

void ViewBoolean::geInformation(){
    this->apply();
}

CControl * ViewBoolean::clone(){
    return new ViewBoolean();
}

void ViewBoolean::updatePlugInControl(int ,CData* data){
    if(DataBoolean * b = dynamic_cast<DataBoolean *>(data))
    {
        if(b->getValue()){
            label->setText("TRUE");
        }else{
            label->setText("FALSE");
        }
        this->update();
    }
}

