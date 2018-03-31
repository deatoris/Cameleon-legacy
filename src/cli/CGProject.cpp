/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGProject.h"
#include "CClient.h"
#include "CGInstance.h"
#include "CGProjectSerialization.h"
CGProject* CGProject::projectInstance = NULL;

CGProject::CGProject()
{
    this->logPath = "";
    this->readme = "";
    this->name = "";
    this->version = "";
    this->path = "";
    this->tmpPath = "";
    this->ressourcePath = "";
    this->targetPath = "";
    this->connectorOrderMode = "FREE";
    this->nbThread = 1;
    this->showCler=false;

}

CGProject* CGProject::getInstance(){
    if(NULL != projectInstance){
        return projectInstance;
    }else{
        projectInstance = new CGProject();
        return projectInstance;
    }
}

void CGProject::setReadme(QString readme){
    this->readme=readme;
}


QString CGProject::getReadme(){
    return this->readme;
}


int CGProject::getNumberThread(){
    return this->nbThread;
}

void CGProject::setNumberThread(int nb){
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->setNbrThreadSend(nb);
    this->nbThread = nb;
}

void CGProject::createProject(QString path){
    //CGProjectSerialization::createProject(path);
}

void CGProject::setName(QString name){
    this->name = name;
}

void CGProject::setPath(QString path){
    this->path = path;
}

void CGProject::setLogPath(QString logPath){
    this->logPath = logPath;
}

void CGProject::setTargetPath(QString targetPath){
    this->targetPath = targetPath;
}

void CGProject::setTmpPath(QString tmpPath){
    this->tmpPath = tmpPath;
}

void CGProject::setRessourcePath(QString libPath){
    this->ressourcePath = libPath;
}

void CGProject::setVersion(QString version){
    this->version = version;
}

void CGProject::setConnectorOrderMode(QString ){
    this->connectorOrderMode = "HARD";
}

QString CGProject::getConnectorOrderMode(){
    return connectorOrderMode;
}

QString CGProject::getName(){
    return name;
}

QString CGProject::getTargetPath(){
    return targetPath;
}

QString CGProject::getPath(){
    return path;
}

QString CGProject::getLogPath(){
    return logPath;
}

QString CGProject::getVersion(){
    return version;
}

QString CGProject::getTmpPath(){
    return tmpPath;
}

QString CGProject::getRessourcePath(){
    return this->ressourcePath;
}
