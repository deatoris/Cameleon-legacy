/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGGlossaryTree.h"
#include "CLogger.h"
#include "../CGInstance.h"

CGGlossaryTree::CGGlossaryTree(QWidget * parent)
    : QTreeWidget(parent)
{
    this->createActions();
    this->createMenu();
    this->createDummyTree();

    this->setHeaderHidden(true);
    this->setDragEnabled(true);
    this->setDropIndicatorShown(true);
    this->setDragDropMode(QAbstractItemView::DragDrop);
    //    this->setColumnCount(3);
    //    this->showColumn(0);
    //    this->showColumn(1);
    //    this->showColumn(2);

    hasBeenEdited = false;
}

bool CGGlossaryTree::isChild(QTreeWidgetItem* isParent, QTreeWidgetItem* isc){
    if(isParent == isc) return true;
    if(isc == this->getItemPatternTop() || isc == this->getItemRessourceTop() || isc==0) return false;
    QTreeWidgetItem* p = isc->parent();
    if(p == isParent){
        return true;
    }else if(p == this->getItemPatternTop() || p == this->getItemRessourceTop()){
        return false;
    }else{
        return isChild(isParent,p);
    }
}

void CGGlossaryTree::dropEvent( QDropEvent * event ){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* itemFrom = list.first();
    QTreeWidgetItem* itemTo = this->itemAt(event->pos());
    if(itemTo!=0){
        QString typeFrom = qVariantValue<QString>(itemFrom->data(0,Qt::UserRole));
        QString typeTo = qVariantValue<QString>(itemTo->data(0,Qt::UserRole));

        if(!isChild(itemFrom, itemTo)
                && typeFrom.compare(typeTo) == 0
                && itemTo->text(1).compare("dir") == 0){
            QTreeWidgetItem* itemP = itemFrom->parent();
            itemFrom = itemFrom->parent()->takeChild(itemFrom->parent()->indexOfChild(itemFrom));
            QString f = QFileInfo(itemFrom->text(2)).fileName();
            //HD modification
            if(!this->move(itemFrom->text(2), itemTo->text(2))){
                //return to parent
                itemP->addChild(itemFrom);
            }else{
                itemTo->addChild(itemFrom);
                QString dstPath = itemTo->text(2)+"/"+f;

                if(QFileInfo(itemFrom->text(2)).isDir()){
                    rmDir(itemFrom->text(2));
                }else{
                    QFile::remove(itemFrom->text(2));
                }

                //review path root
                this->reviewPath(itemFrom);
                itemFrom->setText(2, dstPath);
            }
        }
    }
}

bool CGGlossaryTree::reviewPath(QTreeWidgetItem* item){
    QString parentPath = "";
    QString typeTo = qVariantValue<QString>(item->data(0,Qt::UserRole));
    if(typeTo.compare("pattern") == 0){
        parentPath = CGInstance::getInstance()->getPatternPath();
    }else{
        parentPath = CGInstance::getInstance()->getRessourcePath();
    }
    QString pathToParent = this->getPathToParent(item->parent(),QFileInfo(item->text(0)).fileName());
    item->setText(2,parentPath+"/"+pathToParent);
    QList<QTreeWidgetItem*>iL = item->takeChildren();
    item->addChildren(iL);
    foreach(QTreeWidgetItem*i, iL){
        reviewPath(i);
    }
    return true;
}

void CGGlossaryTree::contextMenuEvent ( QContextMenuEvent * event ){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();

    if(item == this->topLevelItem(2) || item == this->topLevelItem(3) ){
        deleteDirPatternAction->setEnabled(false);
        deleteDirRessourceAction->setEnabled(false);
        renameDirPatternAction->setEnabled(false);
        renameDirRessourceAction->setEnabled(false);
    }else{
        deleteDirPatternAction->setEnabled(true);
        deleteDirRessourceAction->setEnabled(true);
        renameDirPatternAction->setEnabled(true);
        renameDirRessourceAction->setEnabled(true);
    }

    if(item == this->topLevelItem(2)){
        this->patternDirMenu->exec(this->mapToGlobal(event->pos()));
    }

    if(item == this->topLevelItem(3)){
        this->ressourceDirMenu->exec(this->mapToGlobal(event->pos()));
    }
    if(this->isPattern(item) && this->isElement(item)){
        this->patternMenu->exec(this->mapToGlobal(event->pos()));
    }
    if(this->isPattern(item) && this->isDir(item)){
        this->patternDirMenu->exec(this->mapToGlobal(event->pos()));
    }
    if(this->isRessource(item) && this->isDir(item)){
        this->ressourceDirMenu->exec(this->mapToGlobal(event->pos()));
    }
    if(this->isRessource(item) && this->isElement(item)){
        this->ressourceMenu->exec(this->mapToGlobal(event->pos()));
    }
}

bool CGGlossaryTree::isControl(QTreeWidgetItem* item){
    QString typeTo = qVariantValue<QString>(item->data(0,Qt::UserRole));
    if(typeTo.compare("control") == 0){
        return true;
    }
    return false;
}

bool CGGlossaryTree::isPattern(QTreeWidgetItem* item){
    QString typeTo = qVariantValue<QString>(item->data(0,Qt::UserRole));
    if(typeTo.compare("pattern") == 0){
        return true;
    }
    return false;
}

bool CGGlossaryTree::isOperator(QTreeWidgetItem* item){
    QString typeTo = qVariantValue<QString>(item->data(0,Qt::UserRole));
    if(typeTo.compare("operator") == 0){
        return true;
    }
    return false;
}

bool CGGlossaryTree::isElement(QTreeWidgetItem* item){
    if(item->text(1).compare("file") == 0){
        return true;
    }
    return false;
}

bool CGGlossaryTree::isRessource(QTreeWidgetItem* item){
    QString typeTo = qVariantValue<QString>(item->data(0,Qt::UserRole));
    if(typeTo.compare("ressource") == 0){
        return true;
    }
    return false;
}

bool CGGlossaryTree::isDir(QTreeWidgetItem* item){
    if(item->text(1).compare("dir") == 0){
        return true;
    }
    return false;
}

void CGGlossaryTree::createDummyTree(){
    itemOperatorTop = new QTreeWidgetItem(this);
    itemOperatorTop->setText(0, "operator");
    itemOperatorTop->setText(1, "dir");
    itemOperatorTop->setText(2, "top");
    itemOperatorTop->setData(0,Qt::UserRole,"operator");
    itemOperatorTop->setIcon(0,QIcon(":/icons/ui_tab_content.png"));

    itemControlTop = new QTreeWidgetItem(this);
    itemControlTop->setText(0, "control");
    itemControlTop->setText(1, "dir");
    itemControlTop->setText(2, "top");
    itemControlTop->setData(0,Qt::UserRole,"control");
    itemControlTop->setIcon(0,QIcon(":/icons/ui_tab_content.png"));

    itemPatternTop = new QTreeWidgetItem(this);
    itemPatternTop->setText(0, "pattern");
    itemPatternTop->setText(1, "dir");
    itemPatternTop->setText(2, CGInstance::getInstance()->getPatternPath());
    itemPatternTop->setData(0,Qt::UserRole,"pattern");
    itemPatternTop->setIcon(0,QIcon(":/icons/ui_tab_content.png"));

    itemRessourceTop = new QTreeWidgetItem(this);
    itemRessourceTop->setText(0, "ressource");
    itemRessourceTop->setText(1, "dir");
    itemRessourceTop->setText(2, CGInstance::getInstance()->getRessourcePath());
    itemRessourceTop->setData(0,Qt::UserRole,"ressource");
    itemRessourceTop->setIcon(0,QIcon(":/icons/box.png"));
    itemRessourceTop->setHidden(true);
}


void CGGlossaryTree::createActions(){
    renamePatternAction= new QAction(QIcon(":/icons/pencil.png"),
                                     tr("rename pattern"), this);
    renamePatternAction->setStatusTip(tr("rename pattern"));
    connect(renamePatternAction, SIGNAL(triggered()),
            this, SLOT(renamePattern()));

    deletePatternAction= new QAction(QIcon(":/icons/delete.png"),
                                     tr("delete pattern"), this);
    deletePatternAction->setStatusTip(tr("delete pattern"));
    connect(deletePatternAction, SIGNAL(triggered()),
            this, SLOT(deletePattern()));

    createRessourceAction= new QAction(QIcon(":/icons/add.png"),
                                       tr("add ressource"), this);
    createRessourceAction->setStatusTip(tr("add ressource"));
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

    createDirPatternAction= new QAction(QIcon(":/icons/add.png"),
                                        tr("add directory"), this);
    createDirPatternAction->setStatusTip(tr("create directory"));
    connect(createDirPatternAction, SIGNAL(triggered()),
            this, SLOT(createDirPattern()));

    deleteDirPatternAction= new QAction(QIcon(":/icons/delete.png"),
                                        tr("delete directory"), this);
    deleteDirPatternAction->setStatusTip(tr("delete directory"));
    connect(deleteDirPatternAction, SIGNAL(triggered()),
            this, SLOT(deleteDirPattern()));

    renameDirPatternAction= new QAction(QIcon(":/icons/pencil.png"),
                                        tr("rename directory"), this);
    renameDirPatternAction->setStatusTip(tr("rename directory"));
    connect(renameDirPatternAction, SIGNAL(triggered()),
            this, SLOT(renameDirPattern()));

    createDirRessourceAction= new QAction(QIcon(":/icons/add.png"),
                                          tr("add directory"), this);
    createDirRessourceAction->setStatusTip(tr("add directory"));
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

}

void CGGlossaryTree::createMenu(){
    patternMenu = new QMenu("Pattern");
    patternMenu->addAction(renamePatternAction);
    patternMenu->addAction(deletePatternAction);

    patternDirMenu = new QMenu("Pattern directory");
    patternDirMenu->addAction(createDirPatternAction);
    patternDirMenu->addAction(deleteDirPatternAction);
    patternDirMenu->addAction(renameDirPatternAction);

    ressourceMenu = new QMenu("Ressource");
    //    ressourceMenu->addAction(deleteRessourceAction);
    //    ressourceMenu->addAction(renameRessourceAction);

    ressourceDirMenu = new QMenu("Ressource directory");
    //    ressourceDirMenu->addAction(createRessourceAction);
    //    ressourceDirMenu->addAction(createDirRessourceAction);
    //    ressourceDirMenu->addAction(renameDirRessourceAction);
    //    ressourceDirMenu->addAction(deleteDirRessourceAction);
}


void CGGlossaryTree::deletePattern(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        QFile file(item->text(2));
        file.remove();
        item->parent()->removeChild(item);
    }
}

void CGGlossaryTree::renamePattern(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        bool ok;
        QString text = QInputDialog::getText(0, QObject::tr("Rename pattern"),
                                             QObject::tr("Pattern name:"), QLineEdit::Normal,
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

QTreeWidgetItem* CGGlossaryTree::exists(QTreeWidgetItem* parent, QString toAdd){
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

void CGGlossaryTree::addPattern(QString patternPath){
    QFileInfo infos(patternPath);
    if(infos.isDir()){
        patternPath.replace(CGInstance::getInstance()->getPatternPath()+"/","");
        QList<QString> pathL = patternPath.split("/");
        QTreeWidgetItem* previousItem = NULL;
        foreach(QString dir, pathL){
            if(dir.compare(".")!=0 && dir.compare("..")!=0 ){
                if(previousItem == NULL){
                    QTreeWidgetItem* i = exists(itemPatternTop, dir);
                    if(i == NULL){
                        previousItem = new QTreeWidgetItem(itemPatternTop);
                        previousItem->setText(0, dir);
                        previousItem->setText(1, "dir");
                        previousItem->setText(2, CGInstance::getInstance()->getPatternPath()+"/"+patternPath);
                        previousItem->setData(0,Qt::UserRole,"pattern");
                        previousItem->setIcon(0,QIcon(":/icons/ui_tab_content.png"));
                    }else{
                        previousItem = i;
                    }
                }else{
                    QTreeWidgetItem* i = exists(previousItem, dir);
                    if(i == NULL){
                        previousItem = new QTreeWidgetItem(previousItem);
                        previousItem->setText(0, dir);
                        previousItem->setText(1, "dir");
                        previousItem->setText(2, CGInstance::getInstance()->getPatternPath()+"/"+patternPath);
                        previousItem->setData(0,Qt::UserRole,"pattern");
                        previousItem->setIcon(0,QIcon(":/icons/ui_tab_content.png"));
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
        p.replace(CGInstance::getInstance()->getPatternPath(),"");
        QList<QString> pathL = p.split("/");
        QTreeWidgetItem* previousItem = NULL;
        foreach(QString dir, pathL){
            if(dir.compare(".")!=0 && dir.compare("..")!=0 ){
                if(dir.compare("") != 0){
                    if(previousItem == NULL){
                        QTreeWidgetItem* i = exists(itemPatternTop, dir);
                        if(i == NULL){
                            previousItem= new QTreeWidgetItem(itemPatternTop);
                            previousItem->setText(0, dir);
                            previousItem->setText(1, "dir");
                            previousItem->setText(2, CGInstance::getInstance()->getPatternPath()+"/"+patternPath);
                            previousItem->setData(0,Qt::UserRole,"pattern");
                            previousItem->setIcon(0,QIcon(":/icons/ui_tab_content.png"));
                        }else{
                            previousItem = i;
                        }
                    }else{
                        QTreeWidgetItem* i = exists(previousItem, dir);
                        if(i == NULL){
                            previousItem = new QTreeWidgetItem(previousItem);
                            previousItem->setText(0, dir);
                            previousItem->setText(1, "dir");
                            previousItem->setText(2, CGInstance::getInstance()->getPatternPath()+"/"+patternPath);
                            previousItem->setData(0,Qt::UserRole,"pattern");
                            previousItem->setIcon(0,QIcon(":/icons/ui_tab_content.png"));
                        }else{
                            previousItem = i;
                        }
                    }
                }
            }
        }
        if(previousItem == NULL) previousItem = itemPatternTop;
        previousItem = new QTreeWidgetItem(previousItem);
        previousItem->setText(0, patternFile);
        previousItem->setText(1, "file");
        previousItem->setText(2, patternPath);
        previousItem->setData(0,Qt::UserRole,"pattern");
        previousItem->setIcon(0,QIcon(":/icons/clogo.png"));
    }
}

void CGGlossaryTree::addRessource(QString ressourcePath){
    QFileInfo infos(ressourcePath);
    if(infos.isDir()){
        ressourcePath.replace(CGInstance::getInstance()->getRessourcePath()+"/","");
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
                        previousItem->setText(2, CGInstance::getInstance()->getRessourcePath()+"/"+ressourcePath);
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
                        previousItem->setText(2, CGInstance::getInstance()->getRessourcePath()+"/"+ressourcePath);
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
        p.replace(CGInstance::getInstance()->getRessourcePath(),"");
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
                            previousItem->setText(2, CGInstance::getInstance()->getRessourcePath()+"/"+ressourcePath);
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
                            previousItem->setText(2, CGInstance::getInstance()->getRessourcePath()+"/"+ressourcePath);
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

void CGGlossaryTree::createRessource(){
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

        QFile fileToCopy(CGInstance::getInstance()->getRessourcePath()+"/"+pathToParent);
        if (!fileToCopy.exists()){
            QTreeWidgetItem* itemPatternTop = new QTreeWidgetItem(item);
            itemPatternTop->setText(0, infos.fileName());
            itemPatternTop->setText(1, "file");
            itemPatternTop->setText(2, CGInstance::getInstance()->getRessourcePath()+"/"+pathToParent);
            itemPatternTop->setData(0,Qt::UserRole,"ressource");
            itemPatternTop->setIcon(0,QIcon(":/icons/page_red.png"));
            file.copy(CGInstance::getInstance()->getRessourcePath()+"/"+pathToParent);
            item->sortChildren(1,Qt::AscendingOrder);
        }else{
            CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::ERROR ,"directory named "+infos.baseName()+" already exist");

        }
    }
}

void CGGlossaryTree::deleteRessource(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        QFile file(item->text(2));
        file.remove();
        item->parent()->removeChild(item);
        //delete file on disk
    }
}

void CGGlossaryTree::renameRessource(){
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

void CGGlossaryTree::createDirPattern(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::DEBUG,"createDirPattern");
    bool ok;
    QString text = QInputDialog::getText(0, QObject::tr("Create directory"),
                                         QObject::tr("Directory name:"), QLineEdit::Normal,
                                         item->text(0), &ok);

    QString pathToParent = getPathToParent(item, text);
    CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::DEBUG,"relative pathToParent: "+pathToParent);

    QDir dir(CGInstance::getInstance()->getPatternPath()+"/"+pathToParent);

    if (ok && !text.isEmpty() && !dir.exists()){
        QTreeWidgetItem* itemPatternTop = new QTreeWidgetItem(item);
        itemPatternTop->setText(0, text);
        itemPatternTop->setText(1, "dir");
        itemPatternTop->setText(2, CGInstance::getInstance()->getPatternPath()+"/"+pathToParent);
        itemPatternTop->setData(0,Qt::UserRole,"pattern");
        itemPatternTop->setIcon(0,QIcon(":/icons/ui_tab_content.png"));

        dir.mkpath(CGInstance::getInstance()->getPatternPath()+"/"+pathToParent);
        item->sortChildren(1,Qt::AscendingOrder);
    }else{
        CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::ERROR ,"directory named "+text+" already exist");

    }
}

void CGGlossaryTree::deleteDirPattern(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        rmDir(item->text(2));
        item->parent()->removeChild(item);
    }
}

void CGGlossaryTree::renameDirPattern(){
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
/*

  */
QString CGGlossaryTree::getPathToParent(QTreeWidgetItem*item, QString from){
    if(item == this->topLevelItem(0) ||item == this->topLevelItem(1) ||item == this->topLevelItem(2) || item == this->topLevelItem(3)){
        return from;
    }else{
        return this->getPathToParent(item->parent(),item->text(0)+"/"+from);
    }
}

void CGGlossaryTree::newPattern(QString ){

}

void CGGlossaryTree::createDirRessource(){
    CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::DEBUG,"createDirRessource");
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    bool ok;
    QString text = QInputDialog::getText(0, QObject::tr("Create directory"),
                                         QObject::tr("Directory name:"), QLineEdit::Normal,
                                         item->text(0), &ok);

    QString pathToParent = getPathToParent(item, text);
    CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::DEBUG,"relative pathToParent: "+pathToParent);

    QDir dir(CGInstance::getInstance()->getRessourcePath()+"/"+pathToParent);
    CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::DEBUG,"absolute pathToParent: "+dir.absolutePath());

    if (ok && !text.isEmpty() && !dir.exists()){
        CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::DEBUG,"directory "+text+" creation");
        QTreeWidgetItem* itemRessourceTop = new QTreeWidgetItem(item);
        itemRessourceTop->setText(0, text);
        itemRessourceTop->setText(1, "dir");
        itemRessourceTop->setText(2, CGInstance::getInstance()->getRessourcePath()+"/"+pathToParent);
        itemRessourceTop->setData(0,Qt::UserRole,"ressource");
        itemRessourceTop->setIcon(0,QIcon(":/icons/box.png"));
        itemRessourceTop->setHidden(true);
        item->sortChildren(1,Qt::AscendingOrder);
        dir.mkpath(CGInstance::getInstance()->getRessourcePath()+"/"+pathToParent);
    }else{
        CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::ERROR ,"directory named "+text+" already exist");
    }
}

void CGGlossaryTree::deleteDirRessource(){
    QList<QTreeWidgetItem*> list = this->selectedItems();
    QTreeWidgetItem* item = list.first();
    if(item != this->topLevelItem(0) && item != this->topLevelItem(1) && item != this->topLevelItem(2) ){
        QDir dir(item->parent()->text(2));
        dir.rmdir(item->text(0));
        item->parent()->removeChild(item);
    }
}

void CGGlossaryTree::renameDirRessource(){
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

QTreeWidgetItem* CGGlossaryTree::getItemOperatorTop(){
    return itemOperatorTop;
}

QTreeWidgetItem* CGGlossaryTree::getItemControlTop(){
    return itemControlTop;
}

QTreeWidgetItem* CGGlossaryTree::getItemPatternTop(){
    return itemPatternTop;
}

QTreeWidgetItem* CGGlossaryTree::getItemRessourceTop(){
    return itemRessourceTop;
}

//source: http://stackoverflow.com/questions/2536524/copy-directory-using-qt
bool CGGlossaryTree::move(QString srcPath, QString dstPath)
{
    QDir parentDstDir(dstPath);
    if (parentDstDir.exists(QFileInfo(srcPath).fileName())){
        return false;
    }
    if(!QFileInfo(srcPath).isFile()){
        QDir srcDir(srcPath);
        QDir parentDstDir(dstPath);
        if (!parentDstDir.mkdir(QFileInfo(srcPath).fileName())){
            return false;
        }
        dstPath = dstPath+"/"+QFileInfo(srcPath).fileName();

        foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
            QString srcItemPath = srcPath + "/" + info.fileName();
            QString dstItemPath = dstPath;
            if (info.isDir()) {
                if (!move(srcItemPath, dstItemPath)) {
                    return false;
                }
            } else if (info.isFile()) {
                if (!QFile::copy(srcItemPath, dstItemPath+ "/" + info.fileName())) {
                    return false;
                }
            }
        }
    }else{
        dstPath = dstPath+"/"+QFileInfo(srcPath).fileName();
        if (!QFile::copy(srcPath, dstPath)) {
            return false;
        }
    }
    return true;
}

//source: http://stackoverflow.com/questions/2536524/copy-directory-using-qt
bool CGGlossaryTree::rmDir(QString dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
        return true;
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!rmDir(info.filePath()))
                return false;
        } else {
            if (!dir.remove(info.fileName()))
                return false;
        }
    }
    QDir parentDir(QFileInfo(dirPath).path());
    return parentDir.rmdir(QFileInfo(dirPath).fileName());
}



void CGGlossaryTree::enterSearch(){
    if(!hasBeenEdited && edit->text().compare("search") == 0){
        hasBeenEdited = true;
        edit->setText("");
    }
}

void CGGlossaryTree::setEdit(QLineEdit* edit){
    this->edit = edit;
}

void CGGlossaryTree::expandToItem(QTreeWidget* tree, QTreeWidgetItem* item){
    if(item->parent() != 0){
        item->parent()->setExpanded(true);
        this->expandToItem(tree, item->parent());
    }
}

void CGGlossaryTree::searchOperator(){
    QTreeWidgetItem *item;
    QList<QTreeWidgetItem *> found = this->findItems(
                edit->text(), Qt::MatchContains | Qt::MatchRecursive);
    QList<QTreeWidgetItem *> found2 = this->findItems(
                "", Qt::MatchContains | Qt::MatchRecursive);

    foreach(item, found2){
        item->setExpanded(false);
        item->setSelected(false);
    }

    if(edit->text().compare("") != 0){
        foreach(item, found){
            if(item->childCount() == 0){
                this->expandToItem(this, item);
                item->setSelected(true);
            }
        }
    }
}
