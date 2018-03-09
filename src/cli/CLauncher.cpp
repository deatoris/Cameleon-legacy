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
    CLoggerInstance::getInstance()->log("Init instance ...");
    CGInstance::getInstance()->init();
    CLoggerInstance::getInstance()->log("Instance Initialized.");
    CGProjectSerialization* py = CGInstance::getInstance()->getSerial();
    if(!connect(py, SIGNAL(progressSerial(int)), this, SLOT(loadProgress(int)))){
    }
    first = true;
}

void CLauncher::launchCli(QList<argument> appArgs){
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

    //TODO is file a correct composition?

    CGInstance::getInstance()->setCurrentProjectPath(compositionPath);
    CGProject::getInstance()->setPath(compositionPath);

    compArgs = CArguments::getArgumentsFromComposition(compositionPath);

    if(CArguments::isCompositionHelp(appArgs)){
        CArguments::printCompositionHelp(compositionPath,compArgs);
        exit(0);
    }

    if(!CArguments::checkArguments(appArgs, compArgs)){
        CArguments::printCompositionHelp(compositionPath,compArgs);
        exit(0);
    }

    this->startWithoutProgress();

    //SET ARGUMENTS
   // CGCompositionManager::getInstance()->setArguments(appArgs);
    //if(compositionPath.compare("")!=0)CGCompositionManager::getInstance()->getComposer()->openCompositionByFile(compositionPath);
    //CGCompositionManager::getInstance()->getComposer()->play();

}

void CLauncher::startCli(){
    //ANALYSE ARGUMENTS
    appArgs = CArguments::getArgumentsFromApplication();
    if(!CArguments::checkSurface(appArgs)){
        CArguments::printCameleonHelp();
        exit(0);
    }

    this->launchCli(appArgs);
}

void CLauncher::startWithoutProgress(){

    CInterfaceClient * intefacelocalclient = new  CInterfaceClient(false);
    CClientSingleton::getInstance()->setCInterfaceClient2Server(intefacelocalclient);

    intefacelocalclient->connectWithServer2(CMachineSingleton::getInstance()->getProcessor());

    CMachineSingleton::getInstance()->setMode(IDE);
    CClientSingleton::getInstance()->setMode(IDE);
    CMachineSingleton::getInstance()->start();
    CGInstance::getInstance()->setMode(CGInstance::CMD);
    CGInstance::getInstance()->start();

}



