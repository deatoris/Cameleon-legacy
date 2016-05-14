#include "BackgroundControl.h"
#include "DataBoolean.h"

string BackgroundControl::KEY = "BackgroundControl";

BackgroundControl::BackgroundControl(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Introspection");
    this->setName("Background");
    this->setKey(BackgroundControl::KEY);


    QVBoxLayout *lay = new QVBoxLayout;
    this->setLayout(lay);
}

CControl * BackgroundControl::clone(){
    return new BackgroundControl();
}

