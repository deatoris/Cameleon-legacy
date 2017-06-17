/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCompositionManager.h"
#include "CGComposer.h"
#include "CGCompositionView.h"
#include "CGChangeView.h"
#include "../CGInstance.h"
#include <CData.h>
#include <CClient.h>
#include "CGDeleteOperator.h"
#include "CGCompositionFactory.h"
#include "CGLayoutManager.h"

CGCompositionManager* CGCompositionManager::sceneManagerInstance = NULL;

CGCompositionManager::CGCompositionManager()
{
//    CLogger::getInstance()->registerComponent("PAINT");
//    CLogger::getInstance()->registerComponent("COMPOSITIONMANAGER");
    error = new CGErrorDialog();
    error->hide();
}

CGCompositionManager* CGCompositionManager::getInstance(){
    if(NULL != sceneManagerInstance){
        return sceneManagerInstance;
    }else{
        sceneManagerInstance = new CGCompositionManager();
        return sceneManagerInstance;
    }
}

QList<CGOperatorModel*> CGCompositionManager::getOperatorList(CGCompositionModel* top){
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::getCompositionList");
    QList<CGOperatorModel*> childList;
    foreach(CGOperatorModel* child, top->getOperatorModelList()){
        childList.append(child);
    }
    foreach(CGCompositionModel* compochild, top->getCompositionModelList()){
        childList.append(this->getOperatorList(compochild));
    }
    return childList;
}

QList<CGCompositionModel*> CGCompositionManager::getCompositionList(CGCompositionModel* top){
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::getCompositionList");
    QList<CGCompositionModel*> childList;
    foreach(CGCompositionModel* child, top->getCompositionModelList()){
        childList.append(child);
        childList.append(this->getCompositionList(child));
    }
    return childList;
}

QList<CGCompositionModel*> CGCompositionManager::getCompositionList(){
    QList<CGCompositionModel*> childList;
    childList.append(this->getParentCompositionModel());
    foreach(CGCompositionModel* child, this->getParentCompositionModel()->getCompositionModelList()){
        childList.append(child);
        childList.append(this->getCompositionList(child));
    }
    return childList;
}

CGCompositionModel* CGCompositionManager::getCompositionById(int id){
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::getCompositionById");
    CGCompositionModel* parent = this->getCurrentCompositionModel()->getParentCompositionModel();
    CGCompositionModel* current = this->getCurrentCompositionModel();
    while(parent!=NULL){
        current = parent;
        parent = parent->getParentCompositionModel();
    }
    if(id == 0){
        return current;
    }
    foreach(CGCompositionModel* model, current->getCompositionModelList()){
        if(model->getId() == id){
            return model;
        }else{
            CGCompositionModel* returnModel = model->getModelById(id);
            if(returnModel != NULL) return returnModel;
        }
    }
    return NULL;
}

void CGCompositionManager::clear(){
    CGCompositionModel* parent = this->getCurrentCompositionModel()->getParentCompositionModel();
    CGCompositionModel* current = this->getCurrentCompositionModel();
    while(parent!=NULL){
        current = parent;
        parent = parent->getParentCompositionModel();
    }
    this->showSceneFromModel(current);
    foreach(CGOperatorModel* ope, current->getOperatorModelList()){
        //        ope->getView()->disconnect();
        //        current->unactivate(ope->getView());

        CGDeleteOperator* del= new CGDeleteOperator();
        del->setScene(current->getScene());
        del->setItem(ope->getView());
        del->doCommand();
    }
    foreach(CGCompositionModel* comp, current->getCompositionModelList()){
        //        comp->getOuterView()->disconnect();
        //        current->unactivate(comp->getOuterView());
        CGDeleteOperator* del= new CGDeleteOperator();
        del->setScene(current->getScene());
        del->setItem(comp->getOuterView());
        del->doCommand();
    }
    foreach(CGShape* comp, current->getShapeModelList()){

        CGDeleteOperator* del= new CGDeleteOperator();
        del->setScene(current->getScene());
        del->setItem(comp);
        del->doCommand();
        //        current->unactivate(comp);
    }
    foreach(CGText* comp, current->getTextModelList()){

        CGDeleteOperator* del= new CGDeleteOperator();
        del->setScene(current->getScene());
        del->setItem(comp);
        del->doCommand();
        //        current->unactivate(comp);
    }
    current->getOperatorModelList().clear();
    current->getCompositionModelList().clear();
    current->getScene()->clear();
    current->getScene()->addBackgroundRec();
    CLogger::getInstance()->log("INSTANCE", CLogger::WARNING, "CGCompositionManager::clear() memory leaks here!");
    CGCompositionModel::sid = 1;
}

void CGCompositionManager::showSceneFromModel(CGCompositionModel* compositionModel){
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::showSceneFromModel");
    composerScene->unselectAll();
    composerScene->clearAllFocus();
    composer->centralWidget()->layout()->removeWidget(scenceview);
    scenceview->setInteractive(false);
    scenceview->hide();
    scenceview->setEnabled(false);

    currentCompositionModel = compositionModel;
    composerScene = currentCompositionModel->getScene();
    scenceview = currentCompositionModel->getSceneView();

    scenceview->setInteractive(true);
    scenceview->setEnabled(true);

    composer->centralWidget()->layout()->addWidget(scenceview);
    scenceview->show();
    composerScene->clearAllFocus();
    composerScene->unselectAll();
    composerScene->setFocus();
    scenceview->setFocus();
}

void CGCompositionManager::setCurrentCompositionModel(CGCompositionModel* currentCompositionModel){
    this->currentCompositionModel = currentCompositionModel;
}

CGCompositionModel* CGCompositionManager::getParentCompositionModel(){
    CGCompositionModel* parent = this->getCurrentCompositionModel()->getParentCompositionModel();
    CGCompositionModel* current = this->getCurrentCompositionModel();
    while(parent!=NULL){
        current = parent;
        parent = parent->getParentCompositionModel();
    }
    return current;
}

void CGCompositionManager::reinit(){
    CLogger::getInstance()->log("INSTANCE",CLogger::WARNING,"Memory leak here!");
    composerScene->unselectAll();
    composer->centralWidget()->layout()->removeWidget(scenceview);
    scenceview->setInteractive(false);
    scenceview->hide();
    scenceview->setEnabled(false);

    currentCompositionModel = new CGCompositionModel(composer);
    composerScene = currentCompositionModel->getScene();
    scenceview = currentCompositionModel->getSceneView();

    scenceview->hide();
    scenceview->setInteractive(true);
    scenceview->setEnabled(true);
    composerScene->unselectAll();
    scenceview->setFocus();
}

CGCompositionModel* CGCompositionManager::updateStructOperator(int opId, COperatorStructure opstructure){
    QMutexLocker locker(&_mutex);
    CLogger::getInstance()->log("SCD",CLogger::DEBUG,"CGOperator::updateStruct");
    CGOperatorModel* op = this->searchOperatorById(opId);
    //diff
    QMap<int,int> mapOldIdNewId;
    int delta = 0;
    for(int i =0;i<(int)opstructure.plugIn().size();i++){
        int currentid = i;
        CGConnector* c = op->getView()->getConnectorById(currentid);
        mapOldIdNewId.insert(currentid,currentid);
        if(c == NULL || c == 0 || c->getSide().compare("IN")!=0){
            delta++;
        }
    }
    for(int i =0;i<(int)opstructure.plugOut().size();i++){
        int currentid = op->getNumPlugIn()+i;
        mapOldIdNewId.insert(currentid,currentid+delta);
    }

    op->getView()->disconnect();

    //recreation
    CGOperatorModel* newOp = CGCompositionFactory::getInstance()->getCGOperatorByStruct(opstructure, opId);

    //    CGOperator* newOpe = new CGOperator(this->composer->getOperatorMenu(),this->composer->getConnectorMenu(),0,0);
    CGOperator* newOpe = op->getView();
    newOpe->setModel(newOp);
    newOpe->renderModel();
    newOp->setView(newOpe);


    //connection/disconnection & destruction
    foreach(CGConnector* c,op->getView()->getConnectorsList()){
        if(c->getConnectedConnector()!=NULL && c->isConnected()){
            CGConnector* connectedC = c->getConnectedConnector();
            c->disconnect();
            connectedC->disconnect();
            CGConnector* newCo = newOp->getView()->getConnectorById(mapOldIdNewId.value(c->getId()));
            newCo->connect(connectedC);
            connectedC->connect(newCo);
        }
        if(c->getConnectedControlConnector()!=NULL && c->getIsControlConnected()){
            CGConnector* connectedC = c->getConnectedControlConnector();
            connectedC->disconnect();
            c->disconnect();
            connectedC->setIsControlConnected(false);
            c->setIsControlConnected(false);
            CGConnector* newCo = newOp->getView()->getConnectorById(mapOldIdNewId.value(c->getId()));
            newCo->connect(connectedC);
            connectedC->connect(newCo);
            connectedC->setIsControlConnected(true);
            newCo->setIsControlConnected(true);
        }
    }
    newOpe->setPos(op->getView()->scenePos());
//    foreach(QString action,op->getView()->getActionList()){
//        newOpe->saveAction(action);
//    }

//    CGComposition* comp = dynamic_cast<CGComposition*>(op->getView()->scene());
//    comp->getModel()->unactivate(op->getView());
//    comp->getModel()->activate(newOpe);
    if(CGComposition* compo = dynamic_cast<CGComposition*>(op->getView()->scene())){
        compo->getModel()->replace(op,newOp);
    }

    newOpe->getModel()->setWaiting(false);
    op->setWaiting(false);
    return NULL;
}


CGCompositionModel* CGCompositionManager::getCurrentCompositionModel(){
    return currentCompositionModel;
}

CGOperatorModel * CGCompositionManager::searchOperatorById(int idOperator){
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::searchOperatorById");
    CGCompositionModel* ccompositionModel = this->getCurrentComposerScene()->getModel();
    while(ccompositionModel->getParentCompositionModel() != NULL){
        ccompositionModel = ccompositionModel->getParentCompositionModel();
    }
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::searchOperatorById end");
    return ccompositionModel->getOperatorById(idOperator);
}

QList<CGConnector*> CGCompositionManager::searchCompatibleConnectorForControl(QString dataType, QString side, bool connected){
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::searchCompatibleConnectorForControl");
    QList<CGConnector*> list;
    foreach(CGCompositionModel* model,currentCompositionModel->getCompositionModelList()){
        foreach(CGMultiConnector* connector, model->getOuterView()->getParentView()->getMultiConnectorsList()){
            if(connected || !connector->getOuterConnector()->isConnected()){
                if(connector->getOuterConnector()->getOtherSide()->isConnected()){
                    CGConnector* tc = connector->getOuterConnector()->getOtherSide()->getConnectedConnector();
                    if(tc->getModel() != 0 && tc->getModel()->getSide().compare(side) != 0){
                        if(dataType.compare(QString::fromStdString(DataGeneric::KEY)) == 0
                                || tc->getModel()->getDataType().compare(dataType) == 0
                                || tc->getModel()->getDataType().compare(QString::fromStdString(DataGeneric::KEY)) == 0) {
                            list.append(connector->getOuterConnector());
                        }
                    }
                }
            }
        }
    }
    foreach(CGOperatorModel* model,currentCompositionModel->getOperatorModelList()){
        foreach(CGConnector* connector, model->getView()->getConnectorsList()){
            //            if(connector->getModel()->getDataType().compare(QString::fromStdString(DataGeneric::KEY)) != 0){
            if(connected || !connector->isConnected()){
                if((connector->getModel()->getDataType().compare(dataType) == 0
                    && connector->getModel()->getSide().compare(side) != 0) || (connector->getModel()->getDataType().compare(QString::fromStdString(DataGeneric::KEY)) == 0
                                                                                && connector->getModel()->getSide().compare(side) != 0))
                    list.append(connector);
            }
            //            }
        }
    }
    return list;
}

void CGCompositionManager::addOpeError(int opId, QString message){
    errorOpeMap.insert(opId, message);
    error->load();
    this->composer->getMachine()->show();
    this->composer->getMachineTab()->setCurrentIndex(1);
}

void CGCompositionManager::addCtlError(int opId, QString message){
    errorCtlMap.insert(opId, message);
    error->load();
    error->show();
    this->composer->getMachine()->show();
    this->composer->getMachineTab()->setCurrentIndex(1);
}

void CGCompositionManager::clearError(){
    errorCtlMap.clear();
    errorOpeMap.clear();
    error->clear();
}

CGErrorDialog* CGCompositionManager::getErrorDial(){
    return error;
}

QMap<int,QString> CGCompositionManager::getOpeErrorMap(){
    return errorOpeMap;
}

QMap<int,QString> CGCompositionManager::getCtlErrorMap(){
    return errorCtlMap;
}

CGConnector* CGCompositionManager::getOtherSide(CGMultiConnector* tc){
    if(tc->getOtherSide()->isConnected()&&tc->getOtherSide()->getConnectedConnector()->type()==CGMultiConnector::Type){
        CGMultiConnector* connector = qgraphicsitem_cast<CGMultiConnector *>(tc->getOtherSide()->getConnectedConnector());
        return this->getOtherSide(connector);
    }else if(tc->getOtherSide()->isConnected()){
        return tc->getOtherSide()->getConnectedConnector();
    }
    return 0;
}

QList<CGConnector*> CGCompositionManager::searchCompatibleConnector(QString dataType, QString side, bool connected){
    //TODO: REVIEW CONNECTION CONDITION
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::searchCompatibleConnector");
    QList<CGConnector*> list;
    if(this->getCurrentCompositionModel()->getParentCompositionModel() != NULL){
        foreach(CGMultiConnector* connector, this->getCurrentCompositionModel()->getInnerView()->getParentView()->getMultiConnectorsList()){
            if(connected || !connector->getInnerConnector()->isConnected()){
                if(!connector->getInnerConnector()->getOtherSide()->isConnected()){
                    list.append(connector->getInnerConnector());
                }else if(connector->getInnerConnector()->getOtherSide()->isConnected()){
                    CGConnector* tc = this->getOtherSide(connector->getInnerConnector());
                    if(tc == 0){
                        list.append(connector->getInnerConnector());

                    }else if(tc != 0 && tc->getModel()->getSide().compare(side) != 0){
                        if(dataType.compare(QString::fromStdString(DataGeneric::KEY)) == 0
                                || tc->getModel()->getDataType().compare(dataType) == 0
                                || tc->getModel()->getDataType().compare(QString::fromStdString(DataGeneric::KEY)) == 0) {
                            list.append(connector->getInnerConnector());
                        }
                    }
                }
            }
        }
    }
    foreach(CGCompositionModel* model,currentCompositionModel->getCompositionModelList()){
        foreach(CGMultiConnector *connector, model->getOuterView()->getParentView()->getMultiConnectorsList()){
            if(connected || !connector->getOuterConnector()->isConnected()){
                if(!connector->getOuterConnector()->getOtherSide()->isConnected()){
                    list.append(connector->getOuterConnector());
                }else if(connector->getOuterConnector()->getOtherSide()->isConnected()){
                    if(connector->getOuterConnector()->getSide().compare("OUT") == 0
                            || (connector->getOuterConnector()->getSide().compare("IN") == 0
                                && !connector->getOuterConnector()->getOtherSide()->getConnectedConnector()->getIsControlConnected())){
                        CGConnector* tc = this->getOtherSide(connector->getOuterConnector());
                        if(tc == 0){
                            list.append(connector->getOuterConnector());
                        }else if(tc != 0 && tc->getModel()->getSide().compare(side) != 0){
                            if(dataType.compare(QString::fromStdString(DataGeneric::KEY)) == 0
                                    || tc->getModel()->getDataType().compare(dataType) == 0
                                    || tc->getModel()->getDataType().compare(QString::fromStdString(DataGeneric::KEY)) == 0) {
                                list.append(connector->getOuterConnector());
                            }
                        }
                    }
                }
            }
        }
    }
    foreach(CGOperatorModel* model,currentCompositionModel->getOperatorModelList()){
        foreach(CGConnector* connector, model->getView()->getConnectorsList()){
            if(connected || !connector->isConnected()){
                if((dataType.compare(QString::fromStdString(DataGeneric::KEY)) == 0 && connector->getModel()->getSide().compare(side) != 0) ||
                        (connector->getModel()->getDataType().compare(dataType) == 0
                         && connector->getModel()->getSide().compare(side) != 0)
                        || (connector->getModel()->getDataType().compare(QString::fromStdString(DataGeneric::KEY)) == 0
                            && connector->getModel()->getSide().compare(side) != 0))
                    list.append(connector);
            }
        }
    }
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::searchCompatibleConnector end");
    return list;
}

void CGCompositionManager::showConnected(CGConnector* connector){
    connector->setIsHighLight(true);
    CGComposition*c = (CGComposition*)connector->getOperator()->scene();
    c->getView()->centerOn(connector->getOperator());
    CGCompositionManager::getInstance()->showSceneFromModel(c->getModel());
    if(connector->getConnectedControlConnector()!=0){
        connector->getConnectedControlConnector()->parentItem()->setSelected(true);
    }else{
        if(connector->getConnectedConnector()!=0)connector->getConnectedConnector()->parentItem()->setSelected(true);
    }
    //find composition
    //setComposition
    //highlight connector

}

QList<CGConnector*> CGCompositionManager::searchAllConnector(){
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::searchAllConnector");
    QList<CGConnector*> list;
    if(this->getCurrentCompositionModel()->getParentCompositionModel() != NULL){
        foreach(CGConnector* connector, this->getCurrentCompositionModel()->getInnerView()->getConnectorsList()){
            list.append(connector);
        }
    }
    foreach(CGCompositionModel* model,currentCompositionModel->getCompositionModelList()){
        foreach(CGConnector* connector, model->getOuterView()->getConnectorsList()){
            list.append(connector);
        }
    }
    foreach(CGOperatorModel* model,currentCompositionModel->getOperatorModelList()){
        foreach(CGConnector* connector, model->getView()->getConnectorsList()){
            list.append(connector);
        }
    }
    CLogger::getInstance()->log("COMPOSITIONMANAGER",CLogger::DEBUG,"CGCompositionManager::searchAllConnector end");
    return list;
}

void CGCompositionManager::init(CGComposer* composer){
    this->composer = composer;
    currentCompositionModel = new CGCompositionModel(composer);
    composerScene = currentCompositionModel->getScene();
    scenceview = currentCompositionModel->getSceneView();

    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    QWidget* composerWidgetScene = new QWidget;
    composerWidgetScene->setLayout(layout);
    composerWidgetScene->setAcceptDrops(true);
    currentCompositionModel->getSceneView()->hide();
    layout->addWidget(currentCompositionModel->getSceneView());

    composer->setCentralWidget(composerWidgetScene);
}

bool CGCompositionManager::setArguments(QList<argument> commandArgs){
    QList<CGLayoutItem*> args = this->searchLayoutItemByType(QString::fromStdString(ControlEditorArgument::KEY));
    foreach(CGLayoutItem* item, args){
        ControlEditorArgument* ctrl = dynamic_cast<ControlEditorArgument*>(item->getControl());
        foreach(argument arg, commandArgs){
            QString keyArg = "-"+QString::fromStdString(ctrl->getKeyArgument());
            if(keyArg.compare(arg.key) == 0){
                ctrl->setArgument(arg.value.toStdString());
                ctrl->apply();
            }
        }
    }
    return true;
}

QList<CGLayoutItem*> CGCompositionManager::searchLayoutItemByType(QString layoutItemType){
    QList<CGLayoutItem*> args;
    //scan all composition
    QList<CGCompositionModel*> mL = this->getCompositionList();
    foreach(CGCompositionModel*m, mL){
        foreach(QGraphicsItem* i, m->getScene()->items()){
            if(i->type() == CGLayoutItem::Type){
                CGLayoutItem* lItem = dynamic_cast<CGLayoutItem*>(i);
                if(lItem->getControl()->getKey() == layoutItemType.toStdString()){
                    args.append(lItem);
                }
            }
        }
    }

    //scan all layouts
    QList<CGLayout*> lL = CGLayoutManager::getInstance()->getLayoutL();
    foreach(CGLayout* l, lL){
        foreach(QGraphicsItem* i, l->items()){
            if(i->type() == CGLayoutItem::Type){
                CGLayoutItem* lItem = dynamic_cast<CGLayoutItem*>(i);
                if(lItem->getControl()->getKey() == layoutItemType.toStdString()){
                    args.append(lItem);
                }
            }
        }
    }
    return args;
}

CGComposition * CGCompositionManager::getCurrentComposerScene(){
    return composerScene;
}

CGComposer * CGCompositionManager::getComposer(){
    return composer;
}

void CGCompositionManager::up(){
    CGCompositionModel* compo = CGCompositionManager::getInstance()->getCurrentCompositionModel()->getParentCompositionModel();
    if(compo != NULL){
        CGChangeView* command = new CGChangeView();
        command->setCompositionModel(compo);
        command->doCommand();
    }
}
