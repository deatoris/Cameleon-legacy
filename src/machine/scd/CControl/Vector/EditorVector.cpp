#include "EditorVector.h"

#include "ViewVector.h"

#include "DataVector.h"
#include <CData.h>
#include "DataString.h"

EditorVector::EditorVector(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Vector");
    this->setName("EditorVector");
    this->setKey("EditorVector");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");

    box = new QTableWidget;
    box->setRowCount(0);
    box->setColumnCount(1);

    button = new QPushButton("add element");
    buttonrm = new QPushButton("rm element");
    save = new QPushButton("save");

    if(!QObject::connect(button, SIGNAL(clicked()),this, SLOT(addelement()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorVector and box" ;
    }
    if(!QObject::connect(buttonrm, SIGNAL(clicked()),this, SLOT(rmelement()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorVector and box" ;
    }
    if(!QObject::connect(save, SIGNAL(clicked()),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect EditorVector and box" ;
    }
//    QHBoxLayout * layh =  new QHBoxLayout;

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(button);
    lay->addWidget(buttonrm);
    lay->addWidget(box);
    lay->addWidget(save);

    test = false;
    this->setLayout(lay);

}

void EditorVector::geInformation(){
    test = true;
    this->apply();
}

void EditorVector::addelement(){
    int count = box->rowCount();
    box->setRowCount(count+1);
    QTableWidgetItem* item = new QTableWidgetItem("");
    box->setItem(count,0,item);
    this->update();
}
void EditorVector::rmelement(){
    int count = box->rowCount();
    if(count>0){
        box->setRowCount(count-1);
        this->update();
    }
}

CControl * EditorVector::clone(){
    return new EditorVector();
}

string EditorVector::toString(){
    vector<shared_ptr<CData> > v;
    int count = box->rowCount();
    for(int i=0;i<count;i++){
        QTableWidgetItem* item = box->item(i,0);
        DataString* str = new DataString;
        str->setValue(item->text().toStdString());
        shared_ptr<CData> elt(str);
        v.push_back(elt);
    }
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

void EditorVector::fromString(string str){
    if(str.compare("")!=0){
        vector<shared_ptr<CData> > v;
        std::istringstream iss(str);
        iss >> v;
        for ( int i =0 ; i < (int)v.size(); i++ ){
            int count = box->rowCount();
            box->setRowCount(count+1);
            QTableWidgetItem* item = new QTableWidgetItem((v)[i]->toString().c_str());
            box->setItem(count,0,item);
        }
    }
    test = true;
}

void EditorVector::apply(){
    if(test==true&&this->isPlugOutConnected(0)==true)
    {
        DataVector * vec = new DataVector;
        DataVector::DATAVECTOR v(new vector<shared_ptr<CData> > );
        int count = box->rowCount();
        for(int i=0;i<count;i++){
            QTableWidgetItem* item = box->item(i,0);
            DataString* str = new DataString;
            str->setValue(item->text().toStdString());
            shared_ptr<CData> elt(str);
            v->push_back(elt);
        }
        vec->setData(v);
        this->sendPlugOutControl(0,vec,CPlug::NEW);
    }
}

