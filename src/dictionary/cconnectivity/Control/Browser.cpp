/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "Browser.h"

#include <DataString.h>
#include <DataBoolean.h>
#include <DataNumber.h>
Browser::Browser(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Http");
    this->setName("Browser");
    this->setKey("Browser");
    this->structurePlug().addPlugIn(DataString::KEY,"url.str");
    this->structurePlug().addPlugOut(DataString::KEY,"current url.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"loaded.bool");
    this->structurePlug().addPlugOut(DataNumber::KEY,"progress.num");

    view = new QWebView();

    if(!QObject::connect(view, SIGNAL( loadFinished(bool)),this, SLOT(loadFinished()),Qt::QueuedConnection)){
        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }

    if(!QObject::connect(view, SIGNAL( urlChanged(QUrl)),this, SLOT(linkCliked(QUrl)),Qt::QueuedConnection)){
        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }

    if(!QObject::connect(view, SIGNAL( loadProgress(int)),this, SLOT(progress(int)),Qt::QueuedConnection)){
        //qDebug << "[WARN] Can't connect CDatasEditor and button" ;
    }

    line = new QLineEdit();
    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(view);
    lay->addWidget(line);
    test = false;

    this->setLayout(lay);
    this->setMinimumWidth(300);
    this->setMinimumHeight(200);
}

void Browser::linkCliked(QUrl url){
    line->setText(url.toString());
}


void Browser::loadFinished(){
    test = true;
    this->apply();
}

void Browser::progress(int p){
    if(test==true)
    {
        DataString * url = new DataString;
        DataBoolean * loaded = new DataBoolean;
        DataNumber * prog = new DataNumber;

        url->setValue(line->text().toStdString());
        loaded->setValue(false);
        prog->setValue(p);

        this->sendPlugOutControl(0,url,CPlug::NEW);
        this->sendPlugOutControl(1,loaded,CPlug::NEW);
        this->sendPlugOutControl(2,prog,CPlug::NEW);
    }
}

void Browser::apply(){
    if(test==true)
    {
        DataString * url = new DataString;
        DataBoolean * loaded = new DataBoolean;
        DataNumber * prog = new DataNumber;

        url->setValue(line->text().toStdString());
        loaded->setValue(true);
        prog->setValue(100);

        this->sendPlugOutControl(0,url,CPlug::NEW);
        this->sendPlugOutControl(1,loaded,CPlug::NEW);
        this->sendPlugOutControl(2,prog,CPlug::NEW);
    }
}

CControl * Browser::clone(){
    return new Browser();
}

string Browser::toString(){
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

void Browser::fromString(string str){
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

void Browser::updatePlugInControl(int , CData* data){

    if(DataString * ds = dynamic_cast<DataString *>(data)){
        string surl = ds->getValue();
        line->setText(surl.c_str());
        QUrl url = QUrl(surl.c_str());
        view->setUrl(url);
        this->update();
    }

}
