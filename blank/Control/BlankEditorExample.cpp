/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "BlankEditorExample.h"
#include "BlankDataExample.h"
BlankEditorExample::BlankEditorExample(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("example");
    this->setName("BlankEditorExample");
    this->setKey("BlankEditorExample");
    this->structurePlug().addPlugOut(BlankDataExample::KEY,"out.blank");

    box = new QComboBox;
    box->addItem("TRUE");
    box->addItem("FALSE");
    box->setCurrentIndex(1);

    if(!QObject::connect(box, SIGNAL(currentIndexChanged(QString)),this, SLOT(geInformation()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect BlankEditorExample and box" ;
    }

    QVBoxLayout *lay = new QVBoxLayout;
    lay->addWidget(box);
    test = false;
    this->setLayout(lay);
}

void BlankEditorExample::geInformation(){
    test = true;
    this->apply();
}

CControl * BlankEditorExample::clone(){
    return new BlankEditorExample();
}

string BlankEditorExample::toString(){
    return box->currentText().toStdString();
}

void BlankEditorExample::fromString(string str){
    if(str.compare("TRUE") == 0){
        box->setCurrentIndex(0);
    }else{
        box->setCurrentIndex(1);
    }
    test = true;
}

void BlankEditorExample::apply(){
    if(test==true&&this->isPlugOutConnected(0)==true)
    {
        BlankDataExample * b = new BlankDataExample;
        if(box->currentText().compare("TRUE") == 0){
            b->setValue(true);
        }else{
            b->setValue(false);
        }
        this->sendPlugOutControl(0,b,CPlug::NEW);
    }
}
