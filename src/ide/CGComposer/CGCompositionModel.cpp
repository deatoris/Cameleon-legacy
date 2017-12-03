/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCompositionModel.h"
#include "CGCompositionView.h"
#include "CGCompositionItem.h"
#include "CGCompositionManager.h"
#include "CGCompositionFactory.h"
#include "CGComposer.h"
#include "CGDeleteOperator.h"
#include "CGCreateOperator.h"
#include "CLogger.h"
#include<CClient.h>
#include<CMachine.h>
int CGCompositionModel::sid = 0;

CGCompositionModel::CGCompositionModel(){
    id = sid;
    sid++;
    this->parentComposition = NULL;
    this->multiView = NULL;
    this->scene = NULL;
    infos = "Select this, clicking left then, click right, juste here, select properties and edit this doc.";
}

QString CGCompositionModel::getInformation(){
    return infos;
}

void CGCompositionModel::setInformation(QString infos){
    this->infos = infos;
}

CGCompositionModel::CGCompositionModel(CGComposer* composer){
    id = 0;
    sid = 1;
    this->name = "composition";
    this->parentComposition = NULL;
    this->multiView = NULL;

    this->scene = new CGComposition(this, composer->getOperatorMenu(),
                                    composer->getOperatorMultiMenu(),
                                    composer->getConnectorMenu(),
                                    composer->getMultiMenu(),
                                    composer->getCompositionMenu());

    this->scene->setSceneRect(CGComposition::srect);
    this->sceneview = new CGCompositionView(this->scene);
    this->sceneview->setComposerScene(this->scene);
    this->scene->setView(this->sceneview);
    this->sceneview->setAcceptDrops(true);
    this->scene->load();
    infos = "Select this, clicking left then, click right, juste here, select properties and edit this doc.";
}

CGCompositionModel::CGCompositionModel(CGCompositionModel* parentComposition){
    id = sid;
    sid++;
    infos = "Select this, clicking left then, click right, juste here, select properties and edit this doc.";

//    CClientSingleton::getInstance()->getCInterfaceClient2Server()->createUnitProcessSend(id);
//    CClientSingleton::getInstance()->getCInterfaceClient2Server()->linkUnitProcess(id, parentComposition->getId());
    this->parentComposition = parentComposition;

    //create scene
    this->scene = new CGComposition(this, CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMultiMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getConnectorMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getMultiMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getCompositionMenu());
    this->scene->setSceneRect(CGComposition::srect);
    this->sceneview = new CGCompositionView(this->scene);
    this->scene->setView(this->sceneview);
    this->sceneview->setAcceptDrops(true);
    this->sceneview->setComposerScene(this->scene);
    this->scene->load();
    this->sceneview->hide();
    this->sceneview->setEnabled(false);

    //create a new top item
    CGMulti* multiView = new CGMulti(CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMultiMenu(),
                                     CGCompositionManager::getInstance()->getCurrentComposerScene()->getConnectorMenu(),
                                     0,
                                     0);
    multiView->setModel(this);
    multiView->setMode(CGMulti::TOP);
    multiView->setPos(CGCompositionManager::getInstance()->getCurrentComposerScene()->getLastSceneRightPressedPos());
    multiView->renderModel();
    this->setView(multiView);
}

void CGCompositionModel::initialise(){
    //create scene
    this->scene = new CGComposition(this, CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMultiMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getConnectorMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getMultiMenu(),
                                    CGCompositionManager::getInstance()->getCurrentComposerScene()->getCompositionMenu());
    this->scene->setSceneRect(CGComposition::srect);
    this->sceneview = new CGCompositionView(this->scene);
    this->scene->setView(this->sceneview);
    this->sceneview->setAcceptDrops(true);
    this->sceneview->setComposerScene(this->scene);
    this->scene->load();
    this->sceneview->hide();
    this->sceneview->setEnabled(false);

    //create a new top item
    CGMulti* multiView = new CGMulti(CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMultiMenu(),
                                     CGCompositionManager::getInstance()->getCurrentComposerScene()->getConnectorMenu(),
                                     0,
                                     0);
    multiView->setModel(this);
    multiView->setMode(CGMulti::TOP);
    multiView->renderModel();
    this->setView(multiView);
}

void CGCompositionModel::replace(CGOperatorModel* oldmodel, CGOperatorModel* newmodel){
    operatorMap.remove(oldmodel->getId());
    operatorsList.removeOne(oldmodel);

    operatorsList.append(newmodel);
    operatorMap.insert(newmodel->getId(),newmodel);

}

void CGCompositionModel::activate(QGraphicsItem* item){
    if(item->scene() == 0){
        this->scene->addItem(item);
        item->show();
        item->setEnabled(true);
    }
    if(item->type() == CGOperator::Type){
        CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);

        operatorsList.append(op->getModel());
        operatorMap.insert(op->getModel()->getId(),op->getModel());

        CClientSingleton::getInstance()->getCInterfaceClient2Server()->activateOperatorSend(op->getModel()->getId());
//        CClientSingleton::getInstance()->getCInterfaceClient2Server()->addOperatorToUnitProcessSend(op->getModel()->getId(), this->getId());
    }else if(item->type() == CGMulti::Type){
        CGMulti* m = qgraphicsitem_cast<CGMulti *>(item);
        this->modelsList.append(m->getModel());
        this->multiMap.insert(m->getModel()->getId(), m->getModel());
        m->getModel()->setParentCompositionModel(this->scene->getModel());
        //link with machine parent composition is made in the constructor
    }else if(item->type() == CGShape::Type){
        CGShape* s = qgraphicsitem_cast<CGShape *>(item);
        this->shapesList.append(s);
    }else if(item->type() == CGText::Type){
        CGText* t = qgraphicsitem_cast<CGText *>(item);
        this->textsList.append(t);
    }
//    if(this->parentComposition!=NULL) this->getInnerView()->processInnerRay(false);
    this->scene->update();
}

void CGCompositionModel::unactivate(QGraphicsItem* item){
    this->scene->removeItem(item);
    item->hide();
    item->setEnabled(false);
    if(item->type() == CGOperator::Type){
        CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->desactivateOperatorSend(op->getModel()->getId());
//        CClientSingleton::getInstance()->getCInterfaceClient2Server()->removeOperatorToUnitProcessSend(op->getModel()->getId());
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
    CGOperatorModel* model = CGCompositionFactory::getInstance()->getCGOperatorByName(name);
    if(model != NULL){
        CGOperator* view = new CGOperator(this->scene->getOperatorMenu(),this->scene->getConnectorMenu(), 0,0);
        view->setModel(model);
        view->renderModel();
        model->setView(view);
        return model;
    }else{
        CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create object named "+name);
        return NULL;
    }
}

void CGCompositionModel::save(){

}

void CGCompositionModel::saveAs(){

}

void CGCompositionModel::setName(QString name){
    this->name = name;
    this->getOuterView()->update();
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

CGCompositionView* CGCompositionModel::getSceneView(){
    return sceneview;
}

void CGCompositionModel::setSceneView(CGCompositionView* view){
    this->sceneview = view;
}

void CGCompositionModel::setId(int id){
    this->id = id;
}

int CGCompositionModel::getId(){
    return this->id;
}

void CGCompositionModel::setScene(CGComposition* scene){
    this->scene = scene;
}

CGComposition* CGCompositionModel::getScene(){
    return this->scene;
}

CGCompositionModel* CGCompositionModel::getModelById(int id){
    //recurcive search
    QList<CGCompositionModel*> modelList = this->getCompositionModelList();
    foreach(CGCompositionModel* model, modelList){
        if(model->getId() == id ){
            return model;
        }
    }
    foreach(CGCompositionModel* model, modelList){
        CGCompositionModel* m =  model->getModelById(id);
        if(m!=NULL) return m;
    }
    return NULL;
}

vector<string> CGOperatorModel::getActionList(){
    return this->actionList;
}

CGOperatorModel* CGCompositionModel::getOperatorById(int id){
    //TODO will be a little more complicated :)
    //you need to parse composition child map
    if(operatorMap.contains(id)) return operatorMap.value(id);

    //recurcive search
    QList<CGCompositionModel*> modelList = this->getCompositionModelList();

    foreach(CGCompositionModel* model, modelList){
        CGOperatorModel* ope = model->getOperatorById(id);
        if(ope!=NULL) return ope;
    }
    return NULL;
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
    this->isUpdateWaiting = false;
}


void CGOperatorModel::setWaiting(bool updateWaiting){
    this->isUpdateWaiting = updateWaiting;
}

bool CGOperatorModel::getWaiting(){
    return isUpdateWaiting;
}

void CGOperatorModel::setNumPlugIn(int num){
    this->numPlugIn = num;
}

int CGOperatorModel::getNumPlugIn(){
    return numPlugIn;
}

void CGOperatorModel::addConnector(CGConnectorModel* connector){
    connectorsList.append(connector);
    connectorMap.insert(connector->getId(), connector);
}

int CGOperatorModel::getNumPlugOut(){
    int count = 0;
    foreach(CGConnectorModel* model, connectorsList){
        if(model->getSide().compare("OUT") == 0) count++;
    }
    return count;
}

CGConnectorModel* CGOperatorModel::getConnectorById(int id){
    return connectorMap.value(id);
}

QList<CGConnectorModel*> CGOperatorModel::getConnectorsList(){
    return connectorsList;
}

void CGOperatorModel::setActionList(vector<string> actionList){
    this->actionList = actionList;
}

void CGOperatorModel::setInformation(QString information){
    this->information = information;
}

QString CGOperatorModel::getInformation(){
    return information;
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

bool CGOperatorModel::has(CGConnectorModel* connector){
    foreach(CGConnectorModel* c,connectorsList){
        if(c == connector) return true;
    }
    return false;
}

CGOperator* CGOperatorModel::getView(){
    return this->operatorView;
}

QString CGOperatorModel::getKey(){
    return key;
}

void CGOperatorModel::setKey(QString key){
    this->key = key;
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

void CGConnectorModel::setName(QString name){
    this->name = name;
}


void CGConnectorModel::setExposedName(QString name){
    this->exposedName = name;

}

QString CGConnectorModel::getExposedName(){
    return exposedName;
}

QString CGConnectorModel::getName(){
    return name;
}

void CGConnectorModel::setDataType(CData::Key dataType){
    this->dataType = dataType;
}

QString CGConnectorModel::getDataType(){
    return QString::fromStdString(this->dataType);
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
    if(connectedConnector->getCGOperatorModel()!=NULL &&
            this->getCGOperatorModel()!=NULL &&
            !this->getCGOperatorModel()->getView()->getConnectorById(this->getId())->getControlMode()){
        if(this->getSide()=="OUT")
        {
            this->connectedConnector = connectedConnector;
            int idop1   = this->getCGOperatorModel()->getId();
            int idplug1 = this->getId();

            int idop2   = connectedConnector->getCGOperatorModel()->getId();
            int idplug2 = connectedConnector->getId();
            int nPlugIn = this->getCGOperatorModel()->getNumPlugIn();
            CClientSingleton::getInstance()->getCInterfaceClient2Server()->connectPlugOut2PlugInSend(idop1,idplug1-nPlugIn,idop2,idplug2);

        }
    }else{
        this->connectedConnector = connectedConnector;
    }
}

void CGConnectorModel::disconnect(){
    if(this->getCGOperatorModel()!=NULL && !this->getCGOperatorModel()->getView()->getConnectorById(this->getId())->getIsControlConnected()){
        if(this->getSide()=="IN")
        {
            int idop1   = this->getCGOperatorModel()->getId();
            int idplug1 = this->getId();
            CClientSingleton::getInstance()->getCInterfaceClient2Server()->disconnectPlugInSend(idop1,idplug1);

        }
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


