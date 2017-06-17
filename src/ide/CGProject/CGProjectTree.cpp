/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGProjectTree.h"
#include "CGCompositionManager.h"
#include "CGLayoutManager.h"
#include "CGDeleteOperator.h"
#include "CGDeleteLayout.h"
#include "CGProject.h"
CGProjectTree::CGProjectTree(QWidget * parent)
    : QTreeWidget(parent)
{
    renameAction= new QAction(QIcon(":/icons/page.png"),
                              tr("rename"), this);
    renameAction->setStatusTip(tr("rename"));
    connect(renameAction, SIGNAL(triggered()),
            this, SLOT(rename()));

    deleteAction= new QAction(QIcon(":/icons/delete.png"),
                              tr("delete"), this);
    deleteAction->setStatusTip(tr("delete"));
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteLayout()));

    projectMenu = new QMenu("project");
    projectMenu->addAction(renameAction);
    //    projectMenu->addAction(deleteAction);
    locked = false;


    //CREATE MENUS
    createRessourceAction= new QAction(QIcon(":/icons/add.png"),
                                       tr("import file"), this);
    createRessourceAction->setStatusTip(tr("import file"));
    connect(createRessourceAction, SIGNAL(triggered()),
            this, SLOT(createRessource()));

    deleteRessourceAction= new QAction(QIcon(":/icons/delete.png"),
                                       tr("delete ressource"), this);
    deleteRessourceAction->setStatusTip(tr("delete ressource"));
    connect(deleteRessourceAction, SIGNAL(triggered()),
            this, SLOT(deleteRessource()));

    renameRessourceAction= new QAction(QIcon(":/icons/pencil.png"),
                                       tr("rename ressource"), this);
    renameRessourceAction->setStatusTip(tr("rename ressource"));
    connect(renameRessourceAction, SIGNAL(triggered()),
            this, SLOT(renameRessource()));

    createDirRessourceAction= new QAction(QIcon(":/icons/add.png"),
                                          tr("import directory"), this);
    createDirRessourceAction->setStatusTip(tr("import directory"));
    connect(createDirRessourceAction, SIGNAL(triggered()),
            this, SLOT(createDirRessource()));

    deleteDirRessourceAction= new QAction(QIcon(":/icons/delete.png"),
                                          tr("delete directory"), this);
    deleteDirRessourceAction->setStatusTip(tr("delete directory"));
    connect(deleteDirRessourceAction, SIGNAL(triggered()),
            this, SLOT(deleteDirRessource()));

    renameDirRessourceAction= new QAction(QIcon(":/icons/pencil.png"),
                                          tr("rename directory"), this);
    renameDirRessourceAction->setStatusTip(tr("rename directory"));
    connect(renameDirRessourceAction, SIGNAL(triggered()),
            this, SLOT(renameRessource()));

    ressourceMenu = new QMenu("Ressource");
    ressourceMenu->addAction(deleteRessourceAction);
    ressourceMenu->addAction(renameRessourceAction);

    ressourceDirMenu = new QMenu("Ressource directory");
    ressourceDirMenu->addAction(createRessourceAction);
    ressourceDirMenu->addAction(createDirRessourceAction);
    ressourceDirMenu->addAction(renameDirRessourceAction);
    ressourceDirMenu->addAction(deleteDirRessourceAction);

}

QTreeWidgetItem* CGProjectTree::exists(QTreeWidgetItem* parent, QString toAdd){
    QList<QTreeWidgetItem*> l = parent->takeChildren();
    foreach(QTreeWidgetItem* i, l){
        if(i->text(0).compare(toAdd) == 0){
            parent->addChildren(l);
            return i;
        }
    }
    parent->addChildren(l);
    return NULL;
}

void CGProjectTree::setItemRessourceTop(QTreeWidgetItem* topitem){
    itemRessourceTop = topitem;
}

QTreeWidgetItem* CGProjectTree::getItemRessourceTop(){
    return itemRessourceTop;
}

void CGProjectTree::createRessource(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import ressource"), "", tr("Any Files (*.*)"));
    if(fileName.compare("")!=0){
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("createRessource"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        QFileInfo infos(fileName);
        QString pathToParent = getPathToParent(item, infos.fileName());

        QFile fileToCopy(CGProject::getInstance()->getRessourcePath()+"/"+pathToParent);
        if (!fileToCopy.exists()){
            QTreeWidgetItem* itemPatternTop = new QTreeWidgetItem(item);
            itemPatternTop->setText(0, infos.fileName());
            itemPatternTop->setText(1, "file");
            itemPatternTop->setText(2, CGProject::getInstance()->getRessourcePath()+"/"+pathToParent);
            itemPatternTop->setData(0,Qt::UserRole,"ressource");
            itemPatternTop->setIcon(0,QIcon(":/icons/page_red.png"));
            file.copy(CGProject::getInstance()->getRessourcePath()+"/"+pathToParent);
            item->sortChildren(1,Qt::AscendingOrder);
        }
    }
}

QString CGProjectTree::getPathToParent(QTreeWidgetItem*item, QString from){
    if(item == this->topLevelItem(2)){
        return from;
    }else{
        return this->getPathToParent(item->parent(),item->text(0)+"/"+from);
    }
}

void CGProjectTree::deleteRessource(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        QFile file(item->text(2));
        file.remove();
        item->parent()->removeChild(item);
        //delete file on disk
    }
}

void CGProjectTree::renameRessource(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        bool ok;
        QString text = QInputDialog::getText(0, QObject::tr("Rename ressource"),
                                             QObject::tr("Ressource name:"), QLineEdit::Normal,
                                             item->text(0), &ok);
        if (ok && !text.isEmpty()){
            QFile file(item->text(2));
            file.rename(text);
            QFileInfo infos(item->text(2));
            QString dstPath = infos.absolutePath()+"/"+text;
            if(!file.copy(dstPath)){
                return;
            }
            file.remove();
            item->setText(0,text);
            item->setText(2,dstPath);
        }
    }
}

void CGProjectTree::createDirRessource(){
    //    QList<QTreeWidgetItem*> list = this->selectedItems();
    //    QTreeWidgetItem* item = list.first();
    //    bool ok;
    //    QString text = QInputDialog::getText(0, QObject::tr("Create directory"),
    //                                         QObject::tr("Directory name:"), QLineEdit::Normal,
    //                                         item->text(0), &ok);

    //    QString pathToParent = getPathToParent(item, text);

    //    QDir dir(CGProject::getInstance()->getRessourcePath()+"/"+pathToParent);

    //    if (ok && !text.isEmpty() && !dir.exists()){
    //        QTreeWidgetItem* itemRessourceTop = new QTreeWidgetItem(item);
    //        itemRessourceTop->setText(0, text);
    //        itemRessourceTop->setText(1, "dir");
    //        itemRessourceTop->setText(2, CGProject::getInstance()->getRessourcePath()+"/"+pathToParent);
    //        itemRessourceTop->setData(0,Qt::UserRole,"ressource");
    //        itemRessourceTop->setIcon(0,QIcon(":/icons/box.png"));
    //        item->sortChildren(1,Qt::AscendingOrder);
    //        dir.mkpath(CGProject::getInstance()->getRessourcePath()+"/"+pathToParent);
    //    }

    QString dirName =QFileDialog::getExistingDirectory(this, tr("Open Dir"),CGProject::getInstance()->getPath(),QFileDialog::ShowDirsOnly);
    if(dirName.compare("")!=0){
        QFileInfo info(dirName);

        QString targetDir = CGProject::getInstance()->getRessourcePath()+"/"+info.baseName();
        cpDir(dirName,targetDir);
        CGProject::getInstance()->getComposerTree()->addRessource(targetDir);
        CGProject::getInstance()->getControllerTree()->addRessource(targetDir);


        QStringList listFilter;
        listFilter << "*";
        //GET FILES
        //On déclare un QDirIterator dans lequel on indique que l'on souhaite parcourir un rpertoire et ses sous-rpertoires.
        //De plus, on spcifie le filtre qui nous permettra de rcuprer uniquement les fichiers du type souhait.
        QDirIterator fileIterator(targetDir, listFilter ,QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);

        //Variable qui contiendra tous les fichiers correspondant notre recherche ...
        QStringList fileList;

        //Tant qu'on n'est pas arrivé à la fin de l'arborescence...
        while(fileIterator.hasNext())
        {
            //...on va au prochain fichier correspondant  notre filtre
            fileList << fileIterator.next();
        }
        foreach(QString f, fileList){
            CGProject::getInstance()->getComposerTree()->addRessource(f);
            CGProject::getInstance()->getControllerTree()->addRessource(f);
        }
    }
}

bool CGProjectTree::cpDir(QString srcPath, QString dstPath)
{
    if(QFileInfo(dstPath).exists()){
        return false;
    }
    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName())){
        return false;
    }

    QDir srcDir(srcPath);
    if(!QFileInfo(srcPath).exists()){
        return false;
    }

    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
        }
    }
    return true;
}

void CGProjectTree::deleteDirRessource(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        QDir dir(item->parent()->text(2));
        dir.rmdir(item->text(0));
        item->parent()->removeChild(item);
    }
}

void CGProjectTree::renameDirRessource(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        bool ok;
        QString text = QInputDialog::getText(0, QObject::tr("Rename directory"),
                                             QObject::tr("Directory name:"), QLineEdit::Normal,
                                             item->text(0), &ok);
        if (ok && !text.isEmpty()){
            QDir dir(item->parent()->text(2));
            dir.rename(item->text(0),text);
            item->setText(0,text);
        }
    }
}

void CGProjectTree::addRessource(QString ressourcePath){
    QFileInfo infos(ressourcePath);
    if(infos.isDir()){
        ressourcePath.replace(CGProject::getInstance()->getRessourcePath()+"/","");
        QList<QString> pathL = ressourcePath.split("/");
        QTreeWidgetItem* previousItem = NULL;
        foreach(QString dir, pathL){
            if(dir.compare(".")!=0 && dir.compare("..")!=0 ){
                if(previousItem == NULL){
                    QTreeWidgetItem* i = exists(itemRessourceTop, dir);
                    if(i == NULL){
                        previousItem = new QTreeWidgetItem(itemRessourceTop);
                        previousItem->setText(0, dir);
                        previousItem->setText(1, "dir");
                        previousItem->setText(2, CGProject::getInstance()->getRessourcePath()+"/"+ressourcePath);
                        previousItem->setData(0,Qt::UserRole,"ressource");
                        previousItem->setIcon(0,QIcon(":/icons/box.png"));
                    }else{
                        previousItem = i;
                    }
                }else{
                    QTreeWidgetItem* i = exists(previousItem, dir);
                    if(i == NULL){
                        previousItem = new QTreeWidgetItem(previousItem);
                        previousItem->setText(0, dir);
                        previousItem->setText(1, "dir");
                        previousItem->setText(2, CGProject::getInstance()->getRessourcePath()+"/"+ressourcePath);
                        previousItem->setData(0,Qt::UserRole,"ressource");
                        previousItem->setIcon(0,QIcon(":/icons/box.png"));
                    }else{
                        previousItem = i;
                    }
                }
            }
        }
    }
    if(infos.isFile()){
        QString patternFile = infos.fileName();
        QString p = infos.absolutePath()+"/";
        p.replace(CGProject::getInstance()->getRessourcePath(),"");
        QList<QString> pathL = p.split("/");
        QTreeWidgetItem* previousItem = NULL;
        foreach(QString dir, pathL){
            if(dir.compare(".")!=0 && dir.compare("..")!=0 ){
                if(dir.compare("") != 0){
                    if(previousItem == NULL){
                        QTreeWidgetItem* i = exists(itemRessourceTop, dir);
                        if(i == NULL){
                            previousItem= new QTreeWidgetItem(itemRessourceTop);
                            previousItem->setText(0, dir);
                            previousItem->setText(1, "dir");
                            previousItem->setText(2, CGProject::getInstance()->getRessourcePath()+"/"+ressourcePath);
                            previousItem->setData(0,Qt::UserRole,"ressource");
                            previousItem->setIcon(0,QIcon(":/icons/box.png"));
                        }else{
                            previousItem = i;
                        }
                    }else{
                        QTreeWidgetItem* i = exists(previousItem, dir);
                        if(i == NULL){
                            previousItem = new QTreeWidgetItem(previousItem);
                            previousItem->setText(0, dir);
                            previousItem->setText(1, "dir");
                            previousItem->setText(2, CGProject::getInstance()->getRessourcePath()+"/"+ressourcePath);
                            previousItem->setData(0,Qt::UserRole,"ressource");
                            previousItem->setIcon(0,QIcon(":/icons/box.png"));
                        }else{
                            previousItem = i;
                        }
                    }
                }
            }
        }
        if(previousItem == NULL) previousItem = itemRessourceTop;
        previousItem = new QTreeWidgetItem(previousItem);
        previousItem->setText(0, patternFile);
        previousItem->setText(1, "file");
        previousItem->setText(2, ressourcePath);
        previousItem->setData(0,Qt::UserRole,"ressource");
        previousItem->setIcon(0,QIcon(":/icons/page_red.png"));

    }
}
void CGProjectTree::deleteLayout(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        QString datas = item->data(0,Qt::UserRole).toString();
        QList<QString > sL = datas.split(":");
        int id = sL[1].toInt();
        if(sL[0].compare("composition") == 0){
            CGCommandManager::getInstance()->startAction("DELETE COMPOSITION");
            CGCompositionModel* model = CGCompositionManager::getInstance()->getCompositionById(id);
            CGMulti* multi = model->getOuterView()->getParentView();
            multi->disconnect();

            CGDeleteOperator* del= new CGDeleteOperator();
            del->setItem(multi->getOuterView());
            del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
            del->doCommand();
            CGCommandManager::getInstance()->endAction("DELETE COMPOSITION");
        }else if(sL[0].compare("layout") == 0){
            CGCommandManager::getInstance()->startAction("DELETE LAYOUT");
            CGLayout* layout = CGLayoutManager::getInstance()->getLayout(id);
            CGDeleteLayout* del = new CGDeleteLayout();
            del->setLayout(layout);
            del->doCommand();
            CGCommandManager::getInstance()->endAction("DELETE LAYOUT");
        }
    }
}

void CGProjectTree::rename(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        bool ok;
        QString text = QInputDialog::getText(0, QObject::tr("Rename"),
                                             QObject::tr("Name:"), QLineEdit::Normal,
                                             item->text(0), &ok);
        if (ok && !text.isEmpty()){
            QVariant v = item->data(0,Qt::UserRole);
            QString s = v.toString();
            CGProject::getInstance()->rename(text,s);
        }
    }
}

void CGProjectTree::contextMenuEvent ( QContextMenuEvent * event ){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item == this->topLevelItem(2)){
        deleteDirRessourceAction->setEnabled(false);
        renameDirRessourceAction->setEnabled(false);
    }else{
        deleteDirRessourceAction->setEnabled(true);

        renameDirRessourceAction->setEnabled(true);
    }

    if(item == this->topLevelItem(2)){
        this->ressourceDirMenu->exec(this->mapToGlobal(event->pos()));
    }

    if(this->isRessource(item) && this->isDir(item)){
        this->ressourceDirMenu->exec(this->mapToGlobal(event->pos()));
    }
    if(this->isRessource(item) && this->isElement(item)){
        this->ressourceMenu->exec(this->mapToGlobal(event->pos()));
    }
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1)&& item != this->topLevelItem(2) ){
        this->projectMenu->exec(this->mapToGlobal(event->pos()));
    }
}

bool CGProjectTree::isElement(QTreeWidgetItem* item){
    if(item->text(1).compare("file") == 0){
        return true;
    }
    return false;
}

bool CGProjectTree::isRessource(QTreeWidgetItem* item){
    QString typeTo = qVariantValue<QString>(item->data(0,Qt::UserRole));
    if(typeTo.compare("ressource") == 0){
        return true;
    }
    return false;
}

bool CGProjectTree::isDir(QTreeWidgetItem* item){
    if(item->text(1).compare("dir") == 0){
        return true;
    }
    return false;
}

QTreeWidgetItem* CGProjectTree::search(QString data, QTreeWidgetItem* topItem){
    QString datas = topItem->data(0,Qt::UserRole).toString();
    if(datas.compare(data) == 0) return topItem;
    int count = topItem->childCount();
    for(int i=0;i<count;i++){
        QTreeWidgetItem* it = topItem->child(i);
        QTreeWidgetItem* iTor = this->search(data, it);
        if(iTor!=NULL) return iTor;
    }
    return NULL;
}

void CGProjectTree::highlighItem(QString ){

}

void CGProjectTree::setLock(bool locked){
    this->locked = locked;
}

bool CGProjectTree::getLock(){
    return locked;
}
