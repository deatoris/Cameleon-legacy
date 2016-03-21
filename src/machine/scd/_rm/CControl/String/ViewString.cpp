#include "ViewString.h"
#include "DataString.h"
ViewString::ViewString(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("String");
    this->setName("ViewStringText");
    this->setKey("StringViewText");
    this->structurePlug().addPlugIn(DataString::KEY,"in.str");

    label = new QTextBrowser;
    label->setPlainText("");
    label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(label);
    this->setLayout(lay);
}

void ViewString::geInformation(){
    this->apply();
}

CControl * ViewString::clone(){
    return new ViewString();
}

void ViewString::updatePlugInControl(int , CData* data){

    DataString * num = dynamic_cast<DataString *>(data);
    label->setPlainText(num->getValue().c_str());
    this->update();

}


