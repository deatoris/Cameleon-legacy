/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGInstance.h"
#include "CGProject.h"
#include "CLogger.h"
#include "CGlossary.h"
#include "CGInstanceSerialization.h"
#include <CClient.h>
#include "CGProjectSerialization.h"

CGInstance* CGInstance::workspaceInstance = NULL;

void CGInstance::setName(QString name){
    this->name = name;
}

void CGInstance::setPath(QString path){
    this->path = path;
}

void CGInstance::setPluginPath(QString pluginPath){
    this->pluginPath = pluginPath;
}

void CGInstance::setLibPath(QString libPath){
    this->libPath = libPath;
}

void CGInstance::setCurrentProjectPath(QString currentPath){
    this->currentProjectPath = currentPath;
}

void CGInstance::setRessourcePath(QString ressourcePath){
    this->ressourcePath = ressourcePath;
}

void CGInstance::addRecentProject(QString composition){
    this->currentProjectPath = composition;
    if(recentProjects.contains(composition)) this->recentProjects.removeOne(composition);
    this->recentProjects.append(composition);
}

void CGInstance::addInstalledLib(QString name, QString version){
    this->installedLibs.insert(name,version);
}

void CGInstance::removeInstalledLib(QString name, QString ){
    this->installedLibs.remove(name);
}

CGProjectSerialization* CGInstance::getSerial(){
    return serial;
}

QString CGInstance::getName(){
    return name;
}

QString CGInstance::getPath(){
    return path;
}

QString CGInstance::getPluginPath(){
    return pluginPath;
}

QString CGInstance::getLibPath(){
    return libPath;
}

QString CGInstance::getCurrentProjectPath(){
    return currentProjectPath;
}

QString CGInstance::getRessourcePath(){
    return ressourcePath;
}

QList<QString> CGInstance::getRecentProjects(){
    return recentProjects;
}

void CGInstance::activeLib(QString name, QString version, bool activated){
    if(activated && !activatedLibs.contains(name)){
        activatedLibs.insert(name,version);
    }
    if(!activated && activatedLibs.contains(name)){
        activatedLibs.remove(name);
    }
}

void CGInstance::loadRessources(){
    QStringList list = getRessourcesFromDir(this->ressourcePath);
    list.sort();
    QStringList fileList;
    QStringList dirList;
    foreach(QString s, list){
        if(!s.contains(QRegExp(".svn"))){
            ressources.insert(s,s);
        }
    }
}

void CGInstance::loadPatterns(){
    QStringList list = getPatternsFromDir(this->patternPath);
    list.sort();
    foreach(QString s, list){
        if(!s.contains(".svn")){
            patterns.insert(s,s);
        }
    }
}

#include<CDictionnary.h>
#include "ICDictionary.h"
#include <QtPlugin>
void CGInstance::loadInstalledLibInfos(){
    QStringList list = getSharedLibrariesFromDir(this->libPath);
    list.sort();
    foreach(QString s, list){
        //try load lib
        QPluginLoader loader(s);
        if(loader.load()){
            QObject *plugin = loader.instance();
            if(plugin){
                ICDictionary *iDictionary = qobject_cast<ICDictionary *>(plugin);
                if(iDictionary!=0){
                    CDictionnary* dictionary = iDictionary->getDictionary();
                    installedLibs.insert(CGInstance::getInstance()->makeRelative(s),dictionary->getVersion().c_str());
                }
            }else{
                installedLibsErrors.insert(CGInstance::getInstance()->makeRelative(s), "Not a Caméléon dictionary. Clean your instance !"+loader.errorString());
            }
            loader.unload();
        }else{
            installedLibsErrors.insert(CGInstance::getInstance()->makeRelative(s), loader.errorString());
        }
    }
}

QMap<QString, QString> CGInstance::getInstalledLibsError(){
    return installedLibsErrors;
}

QString CGInstance::makeAbsolute(QString it){
    QFileInfo info2(this->getPath());
    QFileInfo infoIt(it);
//    if(infoIt.isAbsolute()) return it;
    QString instancePath = info2.absolutePath();
    return instancePath+"/"+it;
}

QString CGInstance::makeRelative(QString it){
    QFileInfo info(this->getPath());
    QFileInfo itInfo(it);
    if(itInfo.isDir()){
        QDir d = info.absoluteDir();
//        QString ds = d.absolutePath();
        QString s = it+"/s.pgm";
        QString toReturn  = d.relativeFilePath(s);
        QFileInfo i(toReturn);
        return i.path();
    }else{
        QString toDir = info.absolutePath();
        QDir d(toDir);
        return d.relativeFilePath(it);
    }
}


void CGInstance::setMode(CGInstance::Mode mode){
    this->mode = mode;
}

CGInstance::Mode CGInstance::getMode(){
    return this->mode;
}

void CGInstance::start(){
    //position dock

    //    CGCompositionManager::getInstance()->getComposer()->addDockWidget(machine, CGCompositionManager::getInstance()->getComposer()->getMachine());
    //    CGCompositionManager::getInstance()->getComposer()->addDockWidget(composerglossary, CGCompositionManager::getInstance()->getComposer()->getGlossary());
    //    CGCompositionManager::getInstance()->getComposer()->addDockWidget(composerproject, CGCompositionManager::getInstance()->getComposer()->getProject());
    //    CGLayoutManager::getInstance()->getController()->addDockWidget(controllerproject,CGLayoutManager::getInstance()->getController()->getProject());
    //    CGLayoutManager::getInstance()->getController()->addDockWidget(controllerglossary,CGLayoutManager::getInstance()->getController()->getGlossary());

    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"=======================================================");
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Start cameleon engine version "+this->instanceVersion);
    //try to load activated libs

    QMap<QString, QString>::iterator j = activatedLibs.begin();
    while (j != activatedLibs.end()) {
        CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load lib "+j.value()+" "+j.key()+" ...");
        QPluginLoader loader(makeAbsolute(j.key()));
        QObject *plugin = loader.instance();
        QString path = makeAbsolute(j.key());
        QString version = j.value();
        if (plugin){
            ICDictionary *iDictionary = qobject_cast<ICDictionary *>(plugin);
            if(iDictionary!=0){
                CDictionnary* dictionary = iDictionary->getDictionary();
                CGlossarySingletonServer::getInstance()->registerDictionnay(dictionary);
                CGlossarySingletonClient::getInstance()->registerDictionnay(dictionary);
            }
        }else{
            qDebug()<<"[ERROR] Can't load activated lib "<<path <<" "<<version;
        }
        j++;
    }
    if(mode == CGInstance::CMD){
        if(currentProjectPath.compare("")!=0){
            QFile file(currentProjectPath);
            if (!file.open(QFile::ReadOnly | QFile::Text)) {
                //            //qDebug << "ERROR: can't load previous project from " << currentProjectPath;
            }else{
                CGProject::getInstance()->setPath(currentProjectPath);
                serial->loadWithoutGui();
            }
        }
    }
}

void CGInstance::load(){
    CGInstanceSerialization::load();
}

QMap<QString, QString> CGInstance::getInstalledLibs(){
    return installedLibs;
}

void CGInstance::clearActivated(){
    activatedLibs.clear();
}

QMap<QString, QString> CGInstance::getActivatedLibs(){
    return activatedLibs;
}

QString CGInstance::areaToString(Qt::DockWidgetArea area){
    if(Qt::LeftDockWidgetArea == area){
        return "LeftDockWidgetArea";
    }
    if(Qt::RightDockWidgetArea == area){
        return "RightDockWidgetArea";
    }
    if(Qt::TopDockWidgetArea == area){
        return "TopDockWidgetArea";
    }
    if(Qt::BottomDockWidgetArea == area){
        return "BottomDockWidgetArea";
    }
    if(Qt::AllDockWidgetAreas == area){
        return "AllDockWidgetAreas";
    }
    if(Qt::NoDockWidgetArea == area){
        return "NoDockWidgetArea";
    }
    return "LeftDockWidgetArea";
}

Qt::DockWidgetArea CGInstance::stringToArea(QString s){
    if(s.compare("LeftDockWidgetArea") == 0){
        return Qt::LeftDockWidgetArea;
    }
    if(s.compare("RightDockWidgetArea") == 0){
        return Qt::RightDockWidgetArea;
    }
    if(s.compare("TopDockWidgetArea") == 0){
        return Qt::TopDockWidgetArea;
    }
    if(s.compare("BottomDockWidgetArea") == 0){
        return Qt::BottomDockWidgetArea;
    }
    if(s.compare("AllDockWidgetAreas") == 0){
        return Qt::AllDockWidgetAreas;
    }
    if(s.compare("NoDockWidgetArea") == 0){
        return Qt::NoDockWidgetArea;
    }
    return Qt::LeftDockWidgetArea;
}

QMap<QString, QString> CGInstance::getRessources(){
    return ressources;
}

QStringList CGInstance::getSharedLibrariesFromDir(QString dir){
    QStringList listFilter;
    listFilter << "*.dll";
    listFilter << "*.so";
    //GET FILES
    //On déclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un rpertoire et ses sous-rpertoires.
    //De plus, on spcifie le filtre qui nous permettra de rcuprer uniquement les fichiers du type souhait.
    QDirIterator fileIterator(dir, listFilter ,QDir::AllEntries);

    //Variable qui contiendra tous les fichiers correspondant notre recherche ...
    QStringList fileList;

    //Tant qu'on n'est pas arrivé à la fin de l'arborescence...
    while(fileIterator.hasNext())
    {
        //...on va au prochain fichier correspondant  notre filtre
        fileList << fileIterator.next();
    }
    return fileList;
}

QMap<QString, QString> CGInstance::getPatterns(){
    return patterns;
}

QStringList CGInstance::getRessourcesFromDir(QString dir){
    QStringList listFilter;
    listFilter << "*";
    //GET FILES
    //On déclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un rpertoire et ses sous-rpertoires.
    //De plus, on spcifie le filtre qui nous permettra de rcuprer uniquement les fichiers du type souhait.
    QDirIterator fileIterator(dir, listFilter ,QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    //Variable qui contiendra tous les fichiers correspondant notre recherche ...
    QStringList fileList;

    //Tant qu'on n'est pas arrivé à la fin de l'arborescence...
    while(fileIterator.hasNext())
    {
        //...on va au prochain fichier correspondant  notre filtre
        fileList << fileIterator.next();
    }
    return fileList;
}

QStringList CGInstance::getPatternsFromDir(QString dir){
    QStringList listFilter;
    listFilter << "*";
    //GET FILES
    //On dclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un rpertoire et ses sous-rpertoires.
    //De plus, on spcifie le filtre qui nous permettra de rcuprer uniquement les fichiers du type souhait.
    QDirIterator fileIterator(dir, listFilter ,QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    //Variable qui contiendra tous les fichiers correspondant notre recherche ...
    QStringList fileList;

    //Tant qu'on n'est pas arrivé à la fin de l'arborescence...
    while(fileIterator.hasNext())
    {
        //...on va au prochain fichier correspondant  notre filtre
        fileList << fileIterator.next();
    }

    QStringList listFilter2;
    listFilter2 << "*.pa";
    //GET FILES
    //On dclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un rpertoire et ses sous-rpertoires.
    //De plus, on spcifie le filtre qui nous permettra de rcuprer uniquement les fichiers du type souhait.
    QDirIterator fileIterator2(dir, listFilter2 ,QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

    //Tant qu'on n'est pas arrivé à la fin de l'arborescence...
    while(fileIterator2.hasNext())
    {
        //...on va au prochain fichier correspondant  notre filtre
        fileList << fileIterator2.next();
    }

    return fileList;
}

QString CGInstance::getInstanceVersion(){
    return instanceVersion;
}
QString CGInstance::getPatternPath(){
    return patternPath;
}

QString CGInstance::getProjectPath(){
    return projectPath;
}

CGInstance::CGInstance()
{
    instanceVersion = "1.0.0";
    currentProjectPath = "";
    serial = new CGProjectSerialization();
    this->mode = MNG;

}

void CGInstance::init(){
    this->load();
}
void CGInstance::setProjectPath(QString projectPath){
    this->projectPath = projectPath;
}

void CGInstance::setPatternPath(QString patternPath){
    this->patternPath = patternPath;
}

void CGInstance::setFirstLaunch(bool firstLaunch){
    firstLaunch = firstLaunch;
}

bool CGInstance::getFirstLaunch(){
    return firstLaunch;
}

CGInstance* CGInstance::getInstance(){
    if(NULL != workspaceInstance){
        return workspaceInstance;
    }else{
        workspaceInstance = new CGInstance();
        return workspaceInstance;
    }
}
