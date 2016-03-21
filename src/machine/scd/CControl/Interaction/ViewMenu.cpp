#include "ViewMenu.h"

#include "ViewMenu.h"
#include<DataString.h>
#include<DataTable.h>
#include <CGProject.h>
ViewMenu::ViewMenu(QWidget * parent)
    :CControl(parent),currentActionName("")
{
    this->path().push_back("Interaction");
    this->setName("ViewMenu");
    this->setKey("ViewMenu");
    this->structurePlug().addPlugIn(DataTable::KEY,"description.tab");
    this->structurePlug().addPlugOut(DataString::KEY,"key.str");
    bar = new QMenuBar();

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(bar);
    this->setLayout(lay);

}

void ViewMenu::updatePlugInControl(int , CData* data){
    //build menu

        bar->clear();
        if(DataTable * table = dynamic_cast<DataTable *>(data)){
            QMap<QString, QMenu*> mapMenus;
            shared_ptr<Table> st = table->getData();
            for(int j=0;j<st->sizeCol();j++){
                QString name = st->operator ()(j,0).c_str();
                //Menu name
                QMenu* action = bar->addMenu(name);
                mapMenus.insert(name,action);
            }
            for(int j=0;j<st->sizeCol();j++){
                for(int i=0;i<st->sizeRow();i++){
                    QString name = st->operator ()(j,i).c_str();
                    QString menuname = st->operator ()(j,0).c_str();
                    QMenu* menu = mapMenus.value(menuname);
                    if(i!=0 && name.compare("")!=0) {//Action name
                        QAction* act = menu->addAction(name);
                        if(!QObject::connect(act, SIGNAL(triggered()),this, SLOT(actionClicked()),Qt::DirectConnection)){
                            //qDebug << "[WARN] Can't connect EditorButton and box" ;
                        }

                    }
                }
            }
        }
        this->update();

}

void ViewMenu::actionClicked(){
    QAction* act = qobject_cast<QAction *>(sender());
    currentActionName = act->text();
    this->apply();
}

CControl * ViewMenu::clone(){
    return new ViewMenu();
}

void ViewMenu::apply(){
    if(currentActionName!="")
    {
        DataString * data = new DataString;
        data->setValue(currentActionName.toStdString());
        this->sendPlugOutControl(0,data,CPlug::NEW);
    }
    currentActionName = "";
}
