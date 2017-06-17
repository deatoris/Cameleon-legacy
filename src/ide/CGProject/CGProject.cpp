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
#include "CGCompositionItem.h"
#include "CGComposition.h"
#include "CGCompositionManager.h"
#include "CGLayout.h"
#include "CGLayoutManager.h"
#include "CGLayoutFactory.h"
#include "CGProjectTree.h"
#include "CGChangeView.h"
#include "CClient.h"
#include "CGInstance.h"
#include "CGProjectSerialization.h"
CGProject* CGProject::projectInstance = NULL;

CGProject::CGProject()
{
    this->logPath = "";
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

void CGProject::setShowController(bool sh){
    this->showCler=sh;
}

bool CGProject::getShowController(){
    return this->showCler;
}

CGProject* CGProject::getInstance(){
    if(NULL != projectInstance){
        return projectInstance;
    }else{
        projectInstance = new CGProject();
        return projectInstance;
    }
}

bool CGProject::removeDir(const QString &dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                //                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        //        result = dir.rmdir(dirName);
    }

    return result;
}

void CGProject::emptyTmpFiles(){
    if(this->removeDir(this->tmpPath)){
        //        //qDebug<< "yes";
    }
}

int CGProject::getNumberThread(){
    return this->nbThread;
}

void CGProject::setNumberThread(int nb){
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->setNbrThreadSend(nb);
    this->nbThread = nb;
}

void CGProject::setReadMe(QString readme){
    this->readme = readme;
}

QString CGProject::getReadMe(){
    return readme;
}

void CGProject::createProject(QString path){
    CGProjectSerialization::createProject(path);
}

void CGProject::switchToModel(CGCompositionModel* model){
    foreach(CGProjectTree* tree, treeList){
        if(tree->getLock()){
            QTreeWidgetItem*item = tree->topLevelItem(0);
            QString data = "composition:"+QString::number(model->getId())+":"+model->getName();
            QTreeWidgetItem* itemToRemove = tree->search(data, item);
            if(itemToRemove!=NULL){
                foreach(QTreeWidgetItem* it, tree->selectedItems()){
                    it->setSelected(false);
                }
                itemToRemove->setSelected(true);
                tree->expandItem(itemToRemove->parent());
                tree->expandItem(itemToRemove);
            }else{
                tree->collapseAll();
            }
        }
    }
}

void CGProject::switchToLayout(CGLayout* layout){
    foreach(CGProjectTree* tree, treeList){
        if(tree->getLock()){
            QTreeWidgetItem*item = tree->topLevelItem(1);
            QString data = "layout:"+QString::number(layout->getId())+":"+layout->getName();
            QTreeWidgetItem* itemToRemove = tree->search(data, item);
            if(itemToRemove!=NULL){
                foreach(QTreeWidgetItem* it, tree->selectedItems()){
                    it->setSelected(false);
                }
                //                itemToRemove->setSelected(true);
                tree->expandItem(itemToRemove->parent());
                tree->expandItem(itemToRemove);
            }else{
                tree->collapseAll();
            }
        }
    }
}

void CGProject::addLayout(){
    CGLayoutManager::getInstance()->getController()->newLayout();
}

void CGProject::addComposition(){
    CGCompositionManager::getInstance()->getComposer()->createMulti();
}

void CGProject::rename(QString name, QString data){
    foreach(CGProjectTree* tree, treeList){
        QList<QString > sL = data.split(":");
        int id = sL[1].toInt();
        if(sL[0].compare("composition") == 0){
            QTreeWidgetItem*t = tree->topLevelItem(0);
            QTreeWidgetItem* item = tree->search(data, t);
            item->setText(0,name);
            CGCompositionModel* model = CGCompositionManager::getInstance()->getCompositionById(id);
            model->setName(name);
        }else if(sL[0].compare("layout") == 0){
            QTreeWidgetItem*t = tree->topLevelItem(1);
            QTreeWidgetItem* item = tree->search(data, t);
            item->setText(0,name);
            CGLayout* layout = CGLayoutManager::getInstance()->getLayout(id);
            layout->setName(name);
        }

    }
}

void CGProject::save(){
    this->saveAs(this->path, CGCompositionManager::getInstance()->getCurrentCompositionModel(), CGLayoutManager::getInstance()->getLayoutL());
}

QString CGProject::makeAbsolute(QString it){
    QFileInfo info2(this->getPath());
    QString instancePath = info2.absolutePath();
    return instancePath+"/"+it;
}

QString CGProject::makeRelative(QString it){
    QFileInfo info(this->getPath());
    QFileInfo itInfo(it);
    if(itInfo.isDir()){
        QDir d = info.absoluteDir();
        QString ds = d.absolutePath();
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

void CGProject::saveAs(QString path, CGCompositionModel* topModel, QList<CGLayout*> layoutL, bool patterned, bool isInterface){
    CGProjectSerialization::saveAs(path,topModel,layoutL,patterned,isInterface);
}

CGCompositionModel* CGProject::compositionFromFile(QString path, CGCompositionModel* parentModel){
    return CGProjectSerialization::compositionFromFile(path,parentModel);
}

void CGProject::load(){
    CGInstance::getInstance()->getSerial()->load();
}

void CGProject::clearProcesses(){
    foreach(CGProjectTree* tree, treeList){
            tree->topLevelItem(0)->takeChildren();
    }
}


void CGProject::loadRessources(){
    CLogger::getInstance()->log("SERIALIZE",CLogger::INFO,"Ressources not supported by this version!");

//    foreach(CGProjectTree* tree, treeList){
//            tree->getItemRessourceTop()->takeChildren();
//    }

//    QStringList listFilter;
//    listFilter << "*";
//    //GET FILES
//    //On déclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un rpertoire et ses sous-rpertoires.
//    //De plus, on spcifie le filtre qui nous permettra de rcuprer uniquement les fichiers du type souhait.
//    QDirIterator fileIterator(this->ressourcePath, listFilter ,QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

//    //Variable qui contiendra tous les fichiers correspondant notre recherche ...
//    QStringList fileList;

//    //Tant qu'on n'est pas arrivé à la fin de l'arborescence...
//    while(fileIterator.hasNext())
//    {
//        //...on va au prochain fichier correspondant  notre filtre
//        fileList << fileIterator.next();
//    }
//    foreach(CGProjectTree* tree, treeList){
//        foreach(QString f, fileList){
//            tree->addRessource(f);
//        }
//        tree->getItemRessourceTop()->sortChildren(1,Qt::AscendingOrder);
//    }
}

bool CGProject::cpDir(QString srcPath, QString dstPath)
{
    if(QFileInfo(dstPath).exists()){
        CLogger::getInstance()->log("SCD",CLogger::ERROR,"dir already exist "+dstPath);
        return false;
    }
    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName())){
        CLogger::getInstance()->log("SCD",CLogger::ERROR,"can't mkdir "+QFileInfo(dstPath).fileName());
        return false;
    }

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                CLogger::getInstance()->log("SCD",CLogger::ERROR,"can't copy file"+srcItemPath+"to "+dstItemPath);
                return false;
            }
        } else {
            CLogger::getInstance()->log("SCD",CLogger::INFO,"Unhandled item" + info.filePath() + "in cpDir");
        }
    }
    return true;
}

void CGProject::addRessource(QString sourceRessourcePath){
//    QFileInfo info(sourceRessourcePath);
//    QString dstPath = "";
//    //cp ressource tout project ressource
//    if(info.isFile()){
//        QFile file(sourceRessourcePath);
//        dstPath = this->ressourcePath+"/"+info.fileName();
//        if(!file.copy(dstPath)){
//            return;
//        }

//        //add to trees
//        foreach(CGProjectTree* tree, treeList){
//            QTreeWidgetItem*layoutDir = tree->topLevelItem(2);
//            QTreeWidgetItem* itemToAdd = new QTreeWidgetItem(layoutDir);
//            //        QString data = "ressource:"+info.fileName();
//            itemToAdd->setText(0, info.fileName());
//            itemToAdd->setText(1, "file");
//            itemToAdd->setText(2, dstPath);
//            itemToAdd->setData(0,Qt::UserRole,"ressource");
//            itemToAdd->setIcon(0,QIcon(":/icons/page_red.png"));
//        }
//    }
//    if(info.isDir()){
//        dstPath = this->ressourcePath+"/"+info.fileName();
//        if(cpDir(sourceRessourcePath,dstPath)){
//            return;
//        }

//        //add to trees
//        foreach(CGProjectTree* tree, treeList){
//            QTreeWidgetItem*layoutDir = tree->topLevelItem(2);
//            QTreeWidgetItem* itemToAdd = new QTreeWidgetItem(layoutDir);
//            //        QString data = "ressource:"+info.fileName();
//            itemToAdd->setText(0, info.fileName());
//            itemToAdd->setText(1, "dir");
//            itemToAdd->setText(2, dstPath);
//            itemToAdd->setData(0,Qt::UserRole,"ressource");
//            itemToAdd->setIcon(0,QIcon(":/icons/box.png"));
//        }
//    }
}

void CGProject::close(){

}

void CGProject::addLayout(CGLayout* layout){
    foreach(CGProjectTree* tree, treeList){
        if(layout->getType().compare("interface") == 0){
            QTreeWidgetItem*layoutDir = tree->topLevelItem(1);
            QTreeWidgetItem* itemToAdd = new QTreeWidgetItem(layoutDir);
            QString data = "layout:"+QString::number(layout->getId())+":"+layout->getName();
            QString sid = QString::number(layout->getId());
            itemToAdd->setText(0,"["+sid+"]"+layout->getName());
            itemToAdd->setData(0,Qt::UserRole,data);
            itemToAdd->setIcon(0,QIcon(":/icons/application.png"));
        }else{
            QTreeWidgetItem*item = tree->topLevelItem(1);
            QString data = "layout:configuration";
            QTreeWidgetItem* topItem = tree->search(data, item);
            QTreeWidgetItem* itemToAdd = new QTreeWidgetItem(topItem);
            QString data2 = "layout:"+QString::number(layout->getId())+":"+layout->getName();
            QString sid = QString::number(layout->getId());
            itemToAdd->setText(0,"["+sid+"]"+layout->getName());
            itemToAdd->setData(0,Qt::UserRole,data2);
            itemToAdd->setIcon(0,QIcon(":/icons/application.png"));
        }
    }
}

CGProjectTree* CGProject::getComposerTree(){
    return treeList[0];
}

CGProjectTree* CGProject::getControllerTree(){
    return treeList[1];
}


void CGProject::addComposition(CGCompositionModel* model){
    foreach(CGProjectTree* tree, treeList){
        QTreeWidgetItem* itemToAdd = NULL;
        QTreeWidgetItem*item = tree->topLevelItem(0);
        if(model->getParentCompositionModel()->getId() == 0){
            itemToAdd = new QTreeWidgetItem(item);
            QString data = "composition:"+QString::number(model->getId())+":"+model->getName();
            itemToAdd->setIcon(0,QIcon(":/icons/clogo.png"));
            QString sid = QString::number(model->getId());
            itemToAdd->setText(0,"["+sid+"]"+model->getName());
            itemToAdd->setData(0,Qt::UserRole,data);
        }else{
            QString parentData = "composition:"+QString::number(model->getParentCompositionModel()->getId())+":"+model->getParentCompositionModel()->getName();
            QTreeWidgetItem* parentItem = tree->search(parentData, item);
            itemToAdd = new QTreeWidgetItem(parentItem);
            QString data = "composition:"+QString::number(model->getId())+":"+model->getName();
            QString sid = QString::number(model->getId());
            itemToAdd->setText(0,"["+sid+"]"+model->getName());
            itemToAdd->setData(0,Qt::UserRole,data);
            itemToAdd->setIcon(0,QIcon(":/icons/clogo.png"));
        }
        //        if(itemToAdd!=NULL){
        //            itemToAdd->setSelected(true);
        //            tree->expandToDepth();
        //        }
    }
    foreach(CGCompositionModel* m, model->getCompositionModelList()){
        this->addComposition(m);
    }
}

void CGProject::deleteLayout(CGLayout* layout){
    foreach(CGProjectTree* tree, treeList){
        QTreeWidgetItem*item = tree->topLevelItem(1);
        QString data = "layout:"+QString::number(layout->getId())+":"+layout->getName();
        QTreeWidgetItem* itemToRemove = tree->search(data, item);
        tree->removeItemWidget(itemToRemove,0);
        delete itemToRemove;
    }
}

void CGProject::deleteComposition(CGCompositionModel* model){
    foreach(CGProjectTree* tree, treeList){
        QTreeWidgetItem*item = tree->topLevelItem(0);
        QString data = "composition:"+QString::number(model->getId())+":"+model->getName();
        QTreeWidgetItem* itemToRemove = tree->search(data, item);
        tree->removeItemWidget(itemToRemove,0);
        delete itemToRemove;
    }
}

void CGProject::deleteLayout(){

}

void CGProject::deleteComposition(){

}

void CGProject::showElement(QTreeWidgetItem*item,int ){
    QVariant v = item->data(0,Qt::UserRole);
    QString s = v.toString();
    if(s.compare("layouts")!=0 && s.compare("ressource")!=0 ){
        if(s.compare("composition")!=0){
            QList<QString> l = s.split(":");
            if(l[0].compare("composition") == 0){
                QString id = l[1];
                CGCompositionModel* model = CGCompositionManager::getInstance()->getCompositionById(id.toInt());
                CGCommandManager::getInstance()->startAction("CHANGE VIEW");
                CGChangeView* command = new CGChangeView();
                command->setCompositionModel(model);
                command->doCommand();
                CGCommandManager::getInstance()->endAction("CHANGE VIEW");
            }else if(l[0].compare("layout") == 0){
                CGLayout* layout = CGLayoutManager::getInstance()->getLayout(l[1].toInt());
                CGCommandManager::getInstance()->startAction("CHANGE VIEW");
                CGChangeView* command = new CGChangeView();
                command->setLayout(layout);
                command->doCommand();
                CGCommandManager::getInstance()->endAction("CHANGE VIEW");
            }
        }else{
            CGCompositionModel* model = CGCompositionManager::getInstance()->getCompositionById(0);
            CGCommandManager::getInstance()->startAction("CHANGE VIEW");
            CGChangeView* command = new CGChangeView();
            command->setCompositionModel(model);
            command->doCommand();
            CGCommandManager::getInstance()->endAction("CHANGE VIEW");
        }
    }
}

QDockWidget* CGProject::getWidgetCopy(){
    QDockWidget* dockProject = new QDockWidget(QObject::tr("Project"));
    QWidget* dockWidgetProject = new QWidget();
    CGProjectTree* treeProject = new CGProjectTree();
    treeList.append(treeProject);

    connect(treeProject, SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),
            this, SLOT(showElement(QTreeWidgetItem*,int)));

    //build bar (new) & actions (new)

    QAction* saveAction= new QAction(QIcon(":/icons/disk.png"),
                                     tr("save"), this);
    saveAction->setStatusTip(tr("save"));
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(save()));

    QAction* addLayoutAction= new QAction(QIcon(":/icons/application_add.png"),
                                          tr("add layout ..."), this);
    addLayoutAction->setStatusTip(tr("add layout ..."));
    connect(addLayoutAction, SIGNAL(triggered()),
            this, SLOT(addLayout()));

    QAction* addCompositionAction= new QAction(QIcon(":/icons/chart_line.png"),
                                               tr("add composition ..."), this);
    addCompositionAction->setStatusTip(tr("add composition ..."));
    connect(addCompositionAction, SIGNAL(triggered()),
            this, SLOT(addComposition()));

    lockAction= new QAction(QIcon(":/icons/link.png"),
                            tr("lock"), this);
    lockAction->setCheckable(true);
    lockAction->setChecked(false);
    lockAction->setStatusTip(tr("lock"));
    connect(lockAction, SIGNAL(triggered()),
            this, SLOT(lock()));

    QToolBar* bar = new QToolBar("Player bar", dockWidgetProject);
    //    bar->addAction(saveAction);
    bar->addAction(addLayoutAction);
    bar->addAction(addCompositionAction);
    //    bar->addAction(lockAction);

    //build tree (new)
    QTreeWidgetItem* item1= new QTreeWidgetItem(treeProject);
    item1->setText(0, "process");
    item1->setData(0,Qt::UserRole,"compositions");
    item1->setIcon(0,QIcon(":/icons/clogo.png"));
    QString data1 = "composition:"+QString::number(0);
    item1->setData(0,Qt::UserRole,data1);

    QTreeWidgetItem* layoutDir= new QTreeWidgetItem(treeProject);
    layoutDir->setText(0, "interface");
    layoutDir->setData(0,Qt::UserRole,"layouts");
    layoutDir->setIcon(0,QIcon(":/icons/application.png"));

    QTreeWidgetItem* itemToAdd = new QTreeWidgetItem(layoutDir);
    QString data = "layout:pattern";
    itemToAdd->setText(0, "pattern");
    itemToAdd->setData(0,Qt::UserRole,data);
    itemToAdd->setIcon(0,QIcon(":/icons/box.png"));

    QTreeWidgetItem* patToAdd = new QTreeWidgetItem(itemToAdd);
    QString datapat = "layout:0:interface";
    patToAdd->setText(0, "[0]creation");
    patToAdd->setData(0,Qt::UserRole,datapat);
    patToAdd->setIcon(0,QIcon(":/icons/application.png"));

    //    QTreeWidgetItem* itemToAdd2 = new QTreeWidgetItem(layoutDir);
    //    QString data2 = "layout:configuration";
    //    itemToAdd2->setText(0, "configuration");
    //    itemToAdd2->setData(0,Qt::UserRole,data2);
    //    itemToAdd2->setIcon(0,QIcon(":/icons/box.png"));

    QTreeWidgetItem* item3= new QTreeWidgetItem(treeProject);
    item3->setText(0, "ressource");
    item3->setData(0,Qt::UserRole,"ressource");
    item3->setIcon(0,QIcon(":/icons/box.png"));
    item3->setHidden(true);
    treeProject->setHeaderHidden(true);
    treeProject->setDragEnabled(true);
    treeProject->setDropIndicatorShown(true);
    treeProject->setItemRessourceTop(item3);

    //build dock widget to return (new)
    QVBoxLayout* layoutProject = new QVBoxLayout();
    layoutProject->setSpacing(0);
    layoutProject->setMargin(0);
    layoutProject->addWidget(bar);
    layoutProject->addWidget(treeProject);
    dockProject->setWidget(dockWidgetProject);
    dockWidgetProject->setLayout(layoutProject);
    dockWidgetProject->setParent(dockProject);

    return dockProject;

}

void CGProject::setProjectPath(QString ){

}

void CGProject::deleteItem(){

}

void CGProject::renameItem(){

}

void CGProject::lock(){
    foreach(CGProjectTree* tree, treeList){
        if(tree->getLock()){
            tree->setLock(false);
            this->lockAction->setChecked(false);
        }else{
            this->switchToModel(CGCompositionManager::getInstance()->getCurrentCompositionModel());
            this->switchToLayout(CGLayoutManager::getInstance()->getCurrentLayout());
            tree->setLock(true);
            this->lockAction->setChecked(true);
        }
    }

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
