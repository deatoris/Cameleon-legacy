/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "DownloadFile.h"

#include "DataString.h"
#include "DataBoolean.h"
#include "CGProject.h"
#include "CGCompositionManager.h"
#include "CLogger.h"
#include "CClient.h"
DownloadFile::DownloadFile(){
    this->structurePlug().addPlugIn(DataString::KEY,"url.str");
    this->structurePlug().addPlugOut(DataString::KEY,"filepath.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"ok.bool");
    this->path().push_back("Http");
    this->setKey("DownloadFile");
    this->setName("get");
    this->setInformation("Get html DownloadFile from a web page, specified by an url string");
    this->setExecutedThread(COperator::CLIENT);
    loaded = false;
    launched = true;
    htmlout = "";
}

QString DownloadFile::saveFileName(const QUrl &url)
{
    QString path = url.path();
    QString basename = QFileInfo(path).fileName();

    if (basename.isEmpty())
        basename = "download";

    if (QFile::exists(basename)) {
        // already exists, don't overwrite
        int i = 0;
        basename += '.';
        while (QFile::exists(basename + QString::number(i)))
            ++i;

        basename += QString::number(i);
    }

    return basename;
}

void DownloadFile::exec(){
    request = new QNetworkRequest();
    qnam = new QNetworkAccessManager(this);

    DataString* s = dynamic_cast<DataString *>(this->plugIn()[0]->getData());

    QString surl = s->getValue().c_str();
    loaded = false;
    QUrl url = QUrl(surl);

    nameFile = saveFileName(url);
    request->setUrl(url);

    reply = qnam->get(*request);
//    if(!connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()))){
//        this->error("Can connect operator DownloadFile & QNetworkReply.");
//    }

//    if(!connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
//             this, SLOT(errorReceive(QNetworkReply::NetworkError)))){
//        this->error("Can connect operator DownloadFile & QNetworkReply.");
//    }





    //(nearly-)synchronous networkmanager calls: http://rohieb.wordpress.com/2010/07/08/qt-nearly-synchronous-qnetworkaccessmanager-calls/


    reply = qnam->get(*request);

    // execute an event loop to process the request (nearly-synchronous)
    QEventLoop eventLoop;
    // also dispose the event loop after the reply has arrived
    connect(reply, SIGNAL(finished()), &eventLoop, SLOT(quit()));
    eventLoop.exec();



    QFile f(CGProject::getInstance()->getTmpPath()+"/"+nameFile); //On ouvre le fichier

    if ( f.open(QIODevice::WriteOnly) )
    {
        f.write(reply->readAll()); ////On lit la rponse du serveur que l'on met dans un fichier
        f.close(); //On ferme le fichier

        QString qs = CGProject::getInstance()->getTmpPath()+"/"+nameFile;
        string ss = qs.toStdString();
        dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(ss);
        DataBoolean * dret = dynamic_cast<DataBoolean *>(this->plugOut()[1]->getData());
        dret->setValue(true);

//        bool t = CMachineSingleton::getInstance()->getProcessor()->getProcessor()->isStartSleeping();
        COperator::updateMarkingAfterExecution();
        CGCompositionManager::getInstance()->getComposer()->play();
    }else{
        this->error("file already opened");
    }

    launched = false;
//    CGCompositionManager::getInstance()->getComposer()->pause();
}

void DownloadFile::updateMarkingAfterExecution(){

}

COperator * DownloadFile::clone(){
    return new DownloadFile();
}

void DownloadFile::progress(int prog){
    QString s = QString::number(prog);
}

void DownloadFile::errorReceive(QNetworkReply::NetworkError error){
    DataBoolean * dret = dynamic_cast<DataBoolean *>(this->plugOut()[1]->getData());
    dret->setValue(false);
    CLogger::getInstance()->log("CONNECTIVITY",CLogger::ERROR,"NetworkError: "+error);
//    bool t = CMachineSingleton::getInstance()->getProcessor()->getProcessor()->isStartSleeping();
    COperator::updateMarkingAfterExecution();
    CGCompositionManager::getInstance()->getComposer()->play();
}

void DownloadFile::replyFinished(){
    loaded = true;

    QFile f(CGProject::getInstance()->getTmpPath()+"/"+nameFile); //On ouvre le fichier

    if ( f.open(QIODevice::WriteOnly) )
    {
        f.write(reply->readAll()); ////On lit la rponse du serveur que l'on met dans un fichier
        f.close(); //On ferme le fichier

        QString qs = CGProject::getInstance()->getTmpPath()+"/"+nameFile;
        string ss = qs.toStdString();
        dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setData(&ss);
        DataBoolean * dret = dynamic_cast<DataBoolean *>(this->plugOut()[1]->getData());
        dret->setValue(true);

//        bool t = CMachineSingleton::getInstance()->getProcessor()->getProcessor()->isStartSleeping();
        COperator::updateMarkingAfterExecution();
        CGCompositionManager::getInstance()->getComposer()->play();
    }else{
        this->error("file already opened");
    }
}
