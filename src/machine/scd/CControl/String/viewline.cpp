#include "viewline.h"

#include "DataString.h"
ViewLine::ViewLine(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("String");
    this->setName("ViewStringLine");
    this->setKey("StringView");
    this->structurePlug().addPlugIn(DataString::KEY,"in.str");

    label = new QLineEdit;
    label->setText("");
    label->setEnabled(false);
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(label);
    this->setLayout(lay);
}

void ViewLine::geInformation(){
    this->apply();
}

CControl * ViewLine::clone(){
    return new ViewLine();
}

void ViewLine::updatePlugInControl(int , CData* data){

        DataString * num = dynamic_cast<DataString *>(data);
        label->setText(num->getValue().c_str());
        this->update();

}
