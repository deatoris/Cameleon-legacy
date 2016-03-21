#include "Curves.h"
#include "DataTable.h"
Curves::Curves(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Table");
    this->setName("Curve");
    this->setKey("Curves");
    this->structurePlug().addPlugIn(DataTable::KEY,"in.table");

    box = new QTableWidget;
    box->setRowCount(0);
    box->setColumnCount(2);
    QTableWidgetItem* item2 = new QTableWidgetItem("key");
    box->setHorizontalHeaderItem(0,item2);

    QTableWidgetItem* item3 = new QTableWidgetItem("value");
    box->setHorizontalHeaderItem(1,item3);

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
    this->setMinimumWidth(300);
}

void Curves::geInformation(){
    test = true;
    this->apply();
}

CControl * Curves::clone(){
    return new Curves();
}

string Curves::toString(){
    //    DataVector * vec = new DataVector;
    //    DataVector::DATAVECTOR v = vec->getData();
    //    int count = box->rowCount();
    //    for(int i=0;i<count;i++){
    //        QTableWidgetItem* item = box->item(i,0);
    //        DataString* str = new DataString;
    //        str->setValue(item->text().toStdString());
    //        shared_ptr<CData> elt(str);
    //        vector<shared_ptr<CData> >* vb = v.get();
    //        v->push_back(elt);
    //    }
    return "";//vec->toString();//box->currentText().toStdString();
}

void Curves::fromString(string ){
    //    if(str.compare("")!=0){
    //        DataVector * vec = new DataVector;
    //        vec->fromString(str);
    //        DataVector::DATAVECTOR v = vec->getData();
    //        for ( int i =0 ; i < v->size(); i++ ){
    //            int count = box->rowCount();
    //            box->setRowCount(count+1);
    //            QTableWidgetItem* item = new QTableWidgetItem((*v)[i]->toString().c_str());
    //            box->setItem(count,0,item);
    //        }
    //    }
    test = true;
}

void Curves::updatePlugInControl(int , CData*  ){
//    box->clear();
//    if(DataMap * dmap = dynamic_cast<DataMap *>(data)){
//        shared_ptr<map<string,shared_ptr<CData>  > > v = dmap->getData();
//        map<string,shared_ptr<CData>  >::iterator it;
//        int i = 0;
//        for ( it=v->begin() ; it != v->end(); it++ ){
//            QTableWidgetItem* item = new QTableWidgetItem((*it).first.c_str());
//            string file = (*it).second->toString();
//            QTableWidgetItem* item2 = new QTableWidgetItem(file.c_str());
//            box->setRowCount(i+1);
//            box->setItem(i,0,item);
//            box->setItem(i,1,item2);
//            i++;
//        }
//    }
//    this->update();
}
