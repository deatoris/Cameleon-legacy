/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CLauncher.h"
#include<CSingleton.h>
#include<CInterfaceClient.h>
#include <CGlossary.h>
#include <CProcessor.h>
#include <DataNumber.h>
#include <CClient.h>
#include <CMachine.h>
#include "CGInstance.h"
#include <memory>
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include <DataMap.h>
#include <DataString.h>

#include <CUtilitySTL.h>
#include "CGLicence.h"
#include "CGProjectSerialization.h"
CLauncher::CLauncher()
{
    progress = new CGProgressBar();
    connect(this, SIGNAL(update(int,QString,QString)),
            progress, SLOT(update(int,QString,QString)));
    CLoggerInstance::getInstance()->log("Init instance ...");
    CGInstance::getInstance()->init();
    CLoggerInstance::getInstance()->log("Instance Initialized.");
    CGProjectSerialization* py = CGInstance::getInstance()->getSerial();
    if(!connect(py, SIGNAL(progressSerial(int)), this, SLOT(loadProgress(int)))){
    }
    first = true;
}

void CLauncher::setDesktopSize(QSize size){
    this->desktopSize = size;
    this->progress->getWidget()->setGeometry(size.width()/2-250,size.height()/2-250,500,500);
}

void CLauncher::wait(){
    //    for(int i=0; i<25;i++){
    //        for(int l=0; l<10000000;l++){
    //            ///hey
    //            int j = 10;
    //            int k = 100*j;
    //            k++;
    //        }
    //    }
}

void CLauncher::loadProgress(int percent){
    //    if(first)progress->start(100);
    //    first = false;
    emit update(percent,"PROJECT", "load");
}

void CLauncher::launchCli(QList<argument> appArgs){
    //CHECK ARGUMENTS
    //        compArgs = CArguments::getArgumentsFromComposition(compositionPath);

    //        if(CArguments::isCompositionHelp(appArgs)){
    //            CArguments::printCompositionHelp(compositionPath,compArgs);
    //            exit(0);
    //        }

    //        if(!CArguments::checkArguments(appArgs, compArgs)){
    //            CArguments::printCompositionHelp(compositionPath,compArgs);
    //            exit(0);
    //        }

    //        //NOTHING TO SHOW, JUST WATCH COMMAND LINE PROMPT
    //        this->startIDE();

    //        CGCompositionManager::getInstance()->getComposer()->openCompositionByFile(compositionPath);
    //        //SET ARGUMENTS
    //        CGCompositionManager::getInstance()->setArguments(appArgs);
    //        CGCompositionManager::getInstance()->getComposer()->play();
    this->startWithoutProgress();
    QString compositionPath = CArguments::getCompositionFromAppArgs(appArgs);

    if(compositionPath.compare("")==0)compositionPath = CGProject::getInstance()->getPath();

    //LAUNCH CHECKS
    if(CArguments::isCameleonHelp(appArgs)){
        CArguments::printCameleonHelp();
        exit(0);
    }

    //You have to force the composition & not to be in IDE mode to set compmosition arguments
    //ANALYSE ARGUMENTS
    if(!CArguments::isFileExist(compositionPath)){
        CArguments::printCameleonHelp();
        exit(0);
    }

    QList<argument> compArgs = CArguments::getArgumentsFromComposition(compositionPath);

    if(CArguments::isCompositionHelp(appArgs)){
        CArguments::printCompositionHelp(compositionPath,compArgs);
        exit(0);
    }

    if(!CArguments::checkArguments(appArgs, compArgs)){
        CArguments::printCompositionHelp(compositionPath,compArgs);
        exit(0);
    }

    //SET ARGUMENTS
    CGCompositionManager::getInstance()->setArguments(appArgs);
    //if(compositionPath.compare("")!=0)CGCompositionManager::getInstance()->getComposer()->openCompositionByFile(compositionPath);
    CGCompositionManager::getInstance()->getComposer()->play();
   // CClientSingleton::getInstance()->getCInterfaceClient2Server()->startSend();

}

void CLauncher::launchGui(QList<argument> ){
    //Standard instance.conf usage

    this->startIDE();

    CGLayoutManager::getInstance()->getController()->setStandaloneMode(true);
    CGCompositionManager::getInstance()->getComposer()->play();
    CGLayoutManager::getInstance()->getController()->show();


}
#include "CGDoc.h"
void CLauncher::launch(QList<argument> appArgs){
    CLoggerInstance::getInstance()->log("Launch cameleon ...");
    QString compositionPath = CArguments::getCompositionFromAppArgs(appArgs);
    QList<argument> compArgs;
    //LAUNCH CHECKS
    if(CArguments::isCameleonHelp(appArgs)){
        CArguments::printCameleonHelp();
        exit(0);
    }

    //You have to force the composition & not to be in IDE mode to set compmosition arguments
    if(CArguments::isCompositionForced(appArgs)){
        //ANALYSE ARGUMENTS
        if(compositionPath.compare("") == 0 || !CArguments::isFileExist(compositionPath)){
            CArguments::printCameleonHelp();
            exit(0);
        }
    }
    //LAUNCH CHECKS

    //START MACHINE & SHOW COMPOSER and/or CONTROLLER
    /*
        IDE, integrated development environment (controller + composer)
        GUI, Graphical user interface (only controller)
        CLI command line interface
    */
    if(CArguments::mode(appArgs).compare("DOC") == 0){//if .cm file is not setted, then launch standard IDE mode & ignore the rest

        CLoggerInstance::getInstance()->log("===========================");
        CLoggerInstance::getInstance()->log("Generate documentation process");
        this->launchCli(appArgs);
        CLoggerInstance::getInstance()->log("Cameleon launched.");
        CGDoc* doc = new CGDoc();
        doc->generateDoc();

        CLoggerInstance::getInstance()->log("Exit Cameleon instance.");
        CLoggerInstance::getInstance()->log("===========================");
        exit(0);
    }else
        if(CArguments::mode(appArgs).compare("IDE") == 0){//if .cm file is not setted, then launch standard IDE mode & ignore the rest
            CLoggerInstance::getInstance()->log("===========================");
            CLoggerInstance::getInstance()->log("Start IDE Process");
            this->startIDE();
            //        if(compositionPath.compare("")!=0)CGCompositionManager::getInstance()->getComposer()->openCompositionByFile(compositionPath);
            if(CGProject::getInstance()->getShowController()){
                CGLayoutManager::getInstance()->getController()->show();
            }else{
                CGLayoutManager::getInstance()->getController()->hide();
            }
            QString compositionPath = CArguments::getCompositionFromAppArgs(appArgs);

            if(compositionPath.compare("")!=0)CGCompositionManager::getInstance()->getComposer()->openCompositionByFile(compositionPath);

            CGCompositionManager::getInstance()->getComposer()->show();
            CLoggerInstance::getInstance()->log("Cameleon launched.");
        }else if(CArguments::mode(appArgs).compare("GUI") == 0){
            CLoggerInstance::getInstance()->log("===========================");
            CLoggerInstance::getInstance()->log("Start GUI Process");
            this->startIDE();
            CLoggerInstance::getInstance()->log("Open composition ...");
            if(compositionPath.compare("")!=0)CGCompositionManager::getInstance()->getComposer()->openCompositionByFile(compositionPath);
            CLoggerInstance::getInstance()->log("Composition opened.");
            CGLayoutManager::getInstance()->getController()->setStandaloneMode(true);
            CGLayoutManager::getInstance()->getController()->show();
            CGCompositionManager::getInstance()->getComposer()->play();
            CLoggerInstance::getInstance()->log("Cameleon launched.");
        }else if(CArguments::mode(appArgs).compare("CLI") == 0){
            CLoggerInstance::getInstance()->log("===========================");
            CLoggerInstance::getInstance()->log("Start CLI Process");
            this->launchCli(appArgs);
            CLoggerInstance::getInstance()->log("Cameleon launched.");
        }else{
            CLoggerInstance::getInstance()->log("===========================");
            CLoggerInstance::getInstance()->log("Start IDE(default) Process");
            this->startIDE();
            CGLayoutManager::getInstance()->getController()->show();
            CLoggerInstance::getInstance()->log("Cameleon launched.");
        }

}



void CLauncher::startGui(){
    //ANALYSE ARGUMENTS
    QList<argument> appArgs = CArguments::getArgumentsFromApplication();
    if(!CArguments::checkSurface(appArgs)){
        CArguments::printCameleonHelp();
        exit(0);
    }

    this->launchGui(appArgs);
}

void CLauncher::startCli(){
    //ANALYSE ARGUMENTS
    QList<argument> appArgs = CArguments::getArgumentsFromApplication();
    if(!CArguments::checkSurface(appArgs)){
        CArguments::printCameleonHelp();
        exit(0);
    }

    this->launchCli(appArgs);
}

void CLauncher::start(){
    //ANALYSE ARGUMENTS
    QList<argument> appArgs = CArguments::getArgumentsFromApplication();
    if(!CArguments::checkSurface(appArgs)){
        CArguments::printCameleonHelp();
        exit(0);
    }

    this->launch(appArgs);
}

void CLauncher::startWithoutProgress(){

    CInterfaceClient * intefacelocalclient = new  CInterfaceClient(false);
    CClientSingleton::getInstance()->setCInterfaceClient2Server(intefacelocalclient);

    composer = new CGComposer();
    controller = new CGController();
    composer->hide();
    controller->hide();
    /*QSize size = desktopSize;
    size.width();
    composer->setGeometry(50,60,size.width()-100,size.height()/2);
    controller->setGeometry(50,105+size.height()/2,size.width()-100,-100+size.height()/2);*/

    QObject::connect(composer, SIGNAL(destroyed()),
                     controller, SLOT(close()));
    QObject::connect(controller, SIGNAL(destroyed()),
                     composer, SLOT(close()));

    this->wait();
    //    CProcessorObject * processor = new CProcessorObject;
    //    processor->moveToThread(CMachineSingleton::getInstance());
    //    CMachineSingleton::getInstance()->setProcessor(processor);
    intefacelocalclient->connectWithServer2(CMachineSingleton::getInstance()->getProcessor());

    CMachineSingleton::getInstance()->setMode(IDE);
    CClientSingleton::getInstance()->setMode(IDE);
    CMachineSingleton::getInstance()->start();
    CGInstance::getInstance()->setMode(CGInstance::CMD);
    CGInstance::getInstance()->start();

}

void CLauncher::startIDE(){
    progress->start(10);
    emit update(1,"INSTANCE", "init");

    emit update(2,"MACHINE", "init client");
    CLoggerInstance::getInstance()->log("Init client ...");
    CProcessorObject * processor = new CProcessorObject;
    processor->moveToThread(CMachineSingleton::getInstance());
    CMachineSingleton::getInstance()->setProcessor(processor);

    CInterfaceClient * intefacelocalclient = new  CInterfaceClient(false);
    CClientSingleton::getInstance()->setCInterfaceClient2Server(intefacelocalclient);
    CLoggerInstance::getInstance()->log("Client initialized.");

    emit update(3,"INSTANCE", "init IDE");
    composer = new CGComposer();
    controller = new CGController();
    composer->hide();
    controller->hide();
    QSize size = desktopSize;
    size.width();
    composer->setGeometry(50,60,size.width()-100,size.height()/2);
    controller->setGeometry(50,105+size.height()/2,size.width()-100,-100+size.height()/2);

    QObject::connect(composer, SIGNAL(destroyed()),
                     controller, SLOT(close()));
    QObject::connect(controller, SIGNAL(destroyed()),
                     composer, SLOT(close()));

    this->wait();
    emit update(4,"INSTANCE", "init machine");
    //    CProcessorObject * processor = new CProcessorObject;
    //    processor->moveToThread(CMachineSingleton::getInstance());
    //    CMachineSingleton::getInstance()->setProcessor(processor);
    CLoggerInstance::getInstance()->log("Connect to server ...");
    intefacelocalclient->connectWithServer2(CMachineSingleton::getInstance()->getProcessor());
    CLoggerInstance::getInstance()->log("Server connected.");

    CMachineSingleton::getInstance()->setMode(IDE);
    CClientSingleton::getInstance()->setMode(IDE);
    emit update(6,"INSTANCE", "start instance");

    emit update(7,"MACHINE", "start machine");
    CLoggerInstance::getInstance()->log("Start machine ...");
    CMachineSingleton::getInstance()->start();
    CLoggerInstance::getInstance()->log("Machine started.");

    emit update(8,"INSTANCE", "init composer & controller");
    emit update(9,"INSTANCE", "build composer & controller");
    this->wait();
    emit update(10,"INSTANCE", "end load");
    this->wait();
    CLoggerInstance::getInstance()->log("Start instance ...");
    CGInstance::getInstance()->start();
    CLoggerInstance::getInstance()->log("Instance started.");
    emit update(100,"INSTANCE", "end load");
    progress->getWidget()->close();
    progress->getWidget()->hide();
    progress->getWidget()->setEnabled(false);

}

//void CLauncher::startGUI(){
//    progress->start(10);
//    emit update(1,"INSTANCE", "init");

//    emit update(2,"MACHINE", "init client");
//    CProcessorObject * processor = new CProcessorObject;
//    processor->moveToThread(CMachineSingleton::getInstance());
//    CMachineSingleton::getInstance()->setProcessor(processor);

//    CInterfaceClient * intefacelocalclient = new  CInterfaceClient(false);
//    CClientSingleton::getInstance()->setCInterfaceClient2Server(intefacelocalclient);

//    emit update(3,"INSTANCE", "init IDE");
//    composer = new CGComposer();
//    controller = new CGController();
//    composer->hide();
//    controller->hide();
//    QSize size = desktopSize;
//    size.width();
//    composer->setGeometry(50,60,size.width()-100,size.height()/2);
//    controller->setGeometry(50,105+size.height()/2,size.width()-100,-100+size.height()/2);

//    QObject::connect(composer, SIGNAL(destroyed()),
//                     controller, SLOT(close()));
//    QObject::connect(controller, SIGNAL(destroyed()),
//                     composer, SLOT(close()));


//    this->wait();
//    emit update(4,"INSTANCE", "init machine");
//    //    CProcessorObject * processor = new CProcessorObject;
//    //    processor->moveToThread(CMachineSingleton::getInstance());
//    //    CMachineSingleton::getInstance()->setProcessor(processor);
//    intefacelocalclient->connectWithServer2(CMachineSingleton::getInstance()->getProcessor());
//    CMachineSingleton::getInstance()->setMode(GUI);
//    CClientSingleton::getInstance()->setMode(GUI);

//    emit update(6,"INSTANCE", "start instance");

//    emit update(7,"MACHINE", "start machine");
//    CMachineSingleton::getInstance()->start();

//    emit update(8,"INSTANCE", "init composer & controller");
//    emit update(9,"INSTANCE", "build composer & controller");
//    this->wait();
//    emit update(10,"INSTANCE", "end load");
//    this->wait();
//    CGInstance::getInstance()->start();
//    emit update(100,"INSTANCE", "end load");
//    progress->getWidget()->hide();
//    composer->hide();
//    controller->hide();
//}



