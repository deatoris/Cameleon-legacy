#include "ChangeLayout.h"

#include "DataNumber.h"
#include "DataImageQt.h"
#include "DataBoolean.h"
#include "CGLayoutManager.h"
#include "CGProject.h"
ChangeLayout::ChangeLayout(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Introspection");
    this->setName("ShowInterface");
    this->setKey("show");
    this->structurePlug().addPlugIn(DataNumber::KEY,"layoutid.num");

    this->setMinimumSize(500,300);
    this->hide();
}

void ChangeLayout::updatePlugInControl(int ,CData*data){
//    QGraphicsView* view = CGLayoutManager::getInstance()->getCurrentLayout()->getView();

        int id = dynamic_cast<DataNumber *>(data)->getValue();
        CGLayoutManager::getInstance()->showLayout(id);

}

CControl * ChangeLayout::clone(){
    return new ChangeLayout();
}


