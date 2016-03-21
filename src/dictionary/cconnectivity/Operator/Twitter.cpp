/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "Twitter.h"
#include "DataString.h"

Twitter::Twitter(){
    this->structurePlug().addPlugIn(DataString::KEY,"input.gen");
    this->structurePlug().addPlugOut(DataString::KEY,"output.gen");
    this->path().push_back("Twitter");
    this->setKey("ConnectorTwitter");
    this->setName("connector");
    this->setInformation("Get file Twitter from a web file, specified by an url string");
    if(!QObject::connect(this, SIGNAL(urlToLoad(QString)),this, SLOT(loadUrl(QString)),Qt::QueuedConnection)){
        //qDebug << "ouch ca marche po";
    }
    loaded = false;
    htmlout = "";
}

void Twitter::exec(){
    DataString* s = dynamic_cast<DataString *>(this->plugIn()[0]->getData());

    QString surl = s->getValue().c_str();
    loaded = false;
    emit urlToLoad(surl);

    while(!loaded){
        //wait
    }

    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(htmlout.toStdString());

}

COperator * Twitter::clone(){
    return new Twitter();
}

void Twitter::progress(int prog){
    QString s = QString::number(prog);
}

void Twitter::loadUrl(QString surl){
    QUrl url = QUrl(surl);
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpend()));
}

void Twitter::httpend(){
    QString st = reply->readAll();
    htmlout = st;
    loaded = true;

}
