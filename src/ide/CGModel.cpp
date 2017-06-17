/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGModel.h"
#include "CGView.h"
#include "CGOperator.h"
#include "CGSceneManager.h"
#include "CGFactory.h"
#include "CGComposer.h"
#include "CGDeleteOperator.h"
#include "CGCreateOperator.h"
#include<CClient.h>
#include<CMachine.h>
CGWorkspace* CGWorkspace::workspaceInstance = NULL;

void CGWorkspace::setName(QString name){
    this->name = name;
}

void CGWorkspace::setPath(QString path){
    this->path = path;
}

void CGWorkspace::setLogPath(QString logPath){
    this->logPath = logPath;
}

void CGWorkspace::setLibPath(QString libPath){
    this->libPath = libPath;
}

void CGWorkspace::setCurrentComposition(QString currentComposition){
    this->currentComposition = currentComposition;
}

void CGWorkspace::setTmpPath(QString tmpPath){
    this->tmpPath = tmpPath;
}

void CGWorkspace::setRessourcePath(QString ressourcePath){
    this->ressourcePath = ressourcePath;
}

void CGWorkspace::addRecentComposition(QString composition){
    this->recentCompositions.append(composition);
}

void CGWorkspace::addInstalledLib(QString name, QString version){
    this->installedLib.insert(name,version);
}

void CGWorkspace::removeInstalledLib(QString name, QString version){
    this->installedLib.remove(name);
}

QString CGWorkspace::getName(){
    return name;
}

QString CGWorkspace::getPath(){
    return path;
}

QString CGWorkspace::getLogPath(){
    return logPath;
}

QString CGWorkspace::getLibPath(){
    return libPath;
}

QString CGWorkspace::getCurrentComposition(){
    return currentComposition;
}

QString CGWorkspace::getTmpPath(){
    return tmpPath;
}

QString CGWorkspace::getRessourcePath(){
    return ressourcePath;
}

QList<QString> CGWorkspace::getRecentCompositions(){
    return recentCompositions;
}

CGCompositionModel* CGWorkspace::getCurrentModel(){
    return currentModel;
}

void CGWorkspace::save(){

}

void CGWorkspace::createWorkspace(QString path){

}

CGWorkspace::CGWorkspace()
{

}

void CGWorkspace::init(QString path, CGComposer* composer){
    currentModel = new CGCompositionModel(composer);

}

CGWorkspace* CGWorkspace::getInstance(){
    if(NULL != workspaceInstance){
        return workspaceInstance;
    }else{
        workspaceInstance = new CGWorkspace();
        return workspaceInstance;
    }
}

void CGWorkspace::loadComposition(){
}


CGCompositionModel::CGCompositionModel(){
    this->parentComposition = NULL;
    this->multiView = NULL;
    this->scene = NULL;
}

CGCompositionModel::CGCompositionModel(CGComposer* composer){
    this->parentComposition = NULL;
    this->multiView = NULL;

    this->scene = new CGScene(this, composer->getOperatorMenu(),
                              composer->getOperatorMultiMenu(),
                              composer->getConnectorMenu(),
                              composer->getMultiMenu(),
                              composer->getCompositionMenu());

    this->scene->setSceneRect(QRectF(-10000, -5000, 20000, 10000));
    this->sceneview = new CGView(this->scene);
    this->sceneview->setComposerScene(this->scene);
    this->scene->setView(this->sceneview);
    this->sceneview->setAcceptDrops(true);
    this->scene->load();
}

CGCompositionModel::CGCompositionModel(CGCompositionModel* parentComposition){
    this->parentComposition = parentComposition;

    //create scene
    this->scene = new CGScene(this, CGSceneManager::getInstance()->getCurrentComposerScene()->getOperatorMenu(),
                              CGSceneManager::getInstance()->getCurrentComposerScene()->getOperatorMultiMenu(),
                              CGSceneManager::getInstance()->getCurrentComposerScene()->getConnectorMenu(),
                              CGSceneManager::getInstance()->getCurrentComposerScene()->getMultiMenu(),
                              CGSceneManager::getInstance()->getCurrentComposerScene()->getCompositionMenu());
    this->scene->setSceneRect(QRectF(-10000, -5000, 20000, 10000));
    this->sceneview = new CGView(this->scene);
    this->scene->setView(this->sceneview);
    this->sceneview->setAcceptDrops(true);
    this->sceneview->setComposerScene(this->scene);
    this->scene->load();
    this->sceneview->hide();
    this->sceneview->setEnabled(false);

    //create a new top item
    CGMulti* multiView = new CGMulti(CGSceneManager::getInstance()->getCurrentComposerScene()->getOperatorMultiMenu(),
                                     CGSceneManager::getInstance()->getCurrentComposerScene()->getConnectorMenu(),
                                     0,
                                     0);
    multiView->setModel(this);
    multiView->setMode(CGMulti::TOP);
    multiView->setPos(CGSceneManager::getInstance()->getCurrentComposerScene()->getLastSceneRightPressedPos());
    multiView->renderModel();
    this->setView(multiView);
}

void CGCompositionModel::activate(QGraphicsItem* item){
    this->scene->addItem(item);
    if(item->type() == CGOperator::Type){
        CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);
        qDebug() << "[INTEGRATION] MACHINE CALL ACTIVATE OPERATOR NAMED " << op->getModel()->getName();
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->activateOperator(op->getModel()->getId());
        operatorsList.append(op->getModel());
        operatorMap.insert(op->getModel()->getId(),op->getModel());
    }else if(item->type() == CGMulti::Type){
        CGMulti* m = qgraphicsitem_cast<CGMulti *>(item);
        this->modelsList.append(m->getModel());
        this->multiMap.insert(m->getModel()->getId(), m->getModel());
        m->getModel()->setParentCompositionModel(this->scene->getModel());
    }else if(item->type() == CGShape::Type){
        CGShape* s = qgraphicsitem_cast<CGShape *>(item);
        this->shapesList.append(s);
    }else if(item->type() == CGText::Type){
        CGText* t = qgraphicsitem_cast<CGText *>(item);
        this->textsList.append(t);
    }
    if(this->parentComposition!=NULL) this->getInnerView()->processInnerRay();
    this->scene->update();
}

void CGCompositionModel::unactivate(QGraphicsItem* item){
    this->scene->removeItem(item);
    if(item->type() == CGOperator::Type){
        CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);
        //qDebug() << "[INTEGRATION] MACHINE CALL UNACTIVATE OPERATOR NAMED " << op->getModel()->getName();
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->desactivateOperator(op->getModel()->getId());
        operatorsList.removeOne(op->getModel());
        operatorMap.remove(op->getModel()->getId());
    }else if(item->type() == CGMulti::Type){
        CGMulti* m = qgraphicsitem_cast<CGMulti *>(item);
        this->modelsList.removeOne(m->getModel());
        this->multiMap.remove(m->getModel()->getId());
    }else if(item->type() == CGShape::Type){
        CGShape* s = qgraphicsitem_cast<CGShape *>(item);
        this->shapesList.removeOne(s);
    }else if(item->type() == CGText::Type){
        CGText* t = qgraphicsitem_cast<CGText *>(item);
        this->textsList.removeOne(t);
    }
}

bool CGCompositionModel::belongToThis(CGConnector* conn){
    foreach(CGOperatorModel*ope, this->getOperatorModelList()){
        foreach(CGConnectorModel* connector,ope->getConnectorsList()){
            CGConnector* vc = ope->getView()->getConnectorById(connector->getId());
            if(vc == conn){
                return true;
            }
        }
    }
    foreach(CGCompositionModel*compo, this->getCompositionModelList()){
        foreach(CGConnector* vc,compo->getOuterView()->getConnectorsList()){
            if(vc == conn){
                return true;
            }
        }
    }
    return false;
}

CGOperatorModel* CGCompositionModel::addOperatorByName(QString name){
    qDebug() << "[INTEGRATION][DONE] but [TODO] ADD INFO MACHINE WHEN CALL CREATE OPERATOR NAMED " << name;


    CGOperatorModel* model = CGFactory::getInstance()->getCGOperatorByName(name);
    CGOperator* view = new CGOperator(this->scene->getOperatorMenu(),this->scene->getConnectorMenu(), 0,0);
    view->setModel(model);
    view->renderModel();
    model->setView(view);
    return model;
}

void CGCompositionModel::save(){

}

void CGCompositionModel::saveAs(){

}


void CGCompositionModel::setName(QString name){
    this->name = name;
}

void CGCompositionModel::setPath(QString path){
    this->path = path;
}


QString CGCompositionModel::getName(){
    return name;
}

QString CGCompositionModel::getPath(){
    return path;
}

CGCompositionModel* CGCompositionModel::getParentCompositionModel(){
    return this->parentComposition;
}

void CGCompositionModel::setParentCompositionModel(CGCompositionModel* composition){
    this->parentComposition = composition;
}

void CGCompositionModel::setView(CGMulti* multiView){
    this->multiView = multiView;
}

CGView* CGCompositionModel::getSceneView(){
    return sceneview;
}

void CGCompositionModel::setSceneView(CGView* view){
    this->sceneview = view;
}

void CGCompositionModel::setId(int id){
    this->id = id;
}

int CGCompositionModel::getId(){
    return this->id;
}

void CGCompositionModel::setScene(CGScene* scene){
    this->scene = scene;
}

CGScene* CGCompositionModel::getScene(){
    return this->scene;
}


CGOperatorModel* CGCompositionModel::getOperatorById(int id){
    //TODO will be a little more complicated :)
    //you need to parse composition child map
    return operatorMap.value(id);
}

QList<CGOperatorModel*> CGCompositionModel::getOperatorModelList(){
    return this->operatorsList;
}

QList<CGCompositionModel*> CGCompositionModel::getCompositionModelList(){
    return this->modelsList;

}

QList<CGShape*> CGCompositionModel::getShapeModelList(){
    return this->shapesList;

}

QList<CGText*> CGCompositionModel::getTextModelList(){
    return this->textsList;

}

CGMulti* CGCompositionModel::getOuterView(){
    if(multiView == NULL) return NULL;
    return multiView->getOuterView();
}

CGMulti* CGCompositionModel::getInnerView(){
    if(multiView == NULL) return NULL;
    return multiView->getInnerView();
}


CGOperatorModel::CGOperatorModel(){

}

void CGOperatorModel::addConnector(CGConnectorModel* connector){
    connectorsList.append(connector);
    connectorMap.insert(connector->getId(), connector);
}

CGConnectorModel* CGOperatorModel::getConnectorById(int id){
    return connectorMap.value(id);
}


QList<CGConnectorModel*> CGOperatorModel::getConnectorsList(){
    return connectorsList;
}

void CGOperatorModel::setId(int id){
    this->id = id;
}

int CGOperatorModel::getId(){
    return id;
}

void CGOperatorModel::setPos(QPointF scenePos){
    this->scenePos = scenePos;
}

QPointF CGOperatorModel::getPos(){
    return scenePos;
}

void CGOperatorModel::setView(CGOperator* operatorView){
    this->operatorView = operatorView;
}

CGOperator* CGOperatorModel::getView(){
    return this->operatorView;
}

QString CGOperatorModel::getName(){
    return name;
}

void CGOperatorModel::setName(QString name){
    this->name = name;
}

CGConnectorModel::CGConnectorModel(CGOperatorModel* parent){
    this->parent = parent;
}

void CGConnectorModel::setDataType(QString dataType){
    this->dataType = dataType;
}

QString CGConnectorModel::getDataType(){
    return this->dataType;
}

void CGConnectorModel::setSide(QString side){
    this->side = side;
}

QString CGConnectorModel::getSide(){
    return side;
}

int CGConnectorModel::getId(){
    return this->id;
}

void CGConnectorModel::setId(int id){
    this->id = id;
}

void CGConnectorModel::connect(CGConnectorModel* connectedConnector){
    //qDebug() << "[INTEGRATION] MACHINE CALL connect operator";

    if(this->getSide()=="OUT")
    {
        this->connectedConnector = connectedConnector;
        int idop1   = this->getCGOperatorModel()->getId();
        int idplugout = this->getId();

        int idop2   = connectedConnector->getCGOperatorModel()->getId();
        int idplugin = connectedConnector->getId();
        //cout<<"[CLIENT] connect operator\n *idop1="<<idop1<<"\n *idplug1="<<idplug1<<"\n *idop2="<<idop2<<"\n *idplug2="<<idplug2    <<endl;
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->connectPlugOut2PlugIn(idop1,idplugout,idop2,idplugin);

    }



}

void CGConnectorModel::disconnect(){
    //qDebug() << "[INTEGRATION] MACHINE CALL disconnect operator";
    if(this->getSide()=="IN")
    {
        int idop1   = this->getCGOperatorModel()->getId();
        int idplug1 = this->getId();


       //cout<<"[CLIENT] deconnect operator\n *idop1="<<idop1<<"\n *idplug1="<<idplug1<<"\n *idop2="<<idop2<<"\n *idplug2="<<idplug2    <<endl;
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->disconnectPlugIn(idop1,idplug1);

    }
    this->connectedConnector = NULL;
}

bool CGConnectorModel::getIsConnected(){
    if(this->connectedConnector != NULL) return true;
    return false;
}

CGConnectorModel* CGConnectorModel::getConnectedConnector(){
    return this->connectedConnector;
}
CGOperatorModel* CGConnectorModel::getCGOperatorModel(){
    return this->parent;
}

CGShapeModel::CGShapeModel(){

}

CGTextModel::CGTextModel(){

}


