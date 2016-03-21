/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCopyPaste.h"
#include "../CGComposer/CGCompositionManager.h"
#include "CGConnectConnector.h"
#include "CGCreateOperator.h"
#include<CClient.h>
CGCopyPaste::CGCopyPaste()
{
}

void CGCopyPaste::copy(QList<QGraphicsItem*> items){
    CGCommandManager::getInstance()->startAction("COPY");
    if(CClientSingleton::getInstance()->applyStopForEachAction()==true)
            CGCompositionManager::getInstance()->getComposer()->stop();
    foreach (QGraphicsItem *item, items) {
        if (item->type() == CGMulti::Type) {
            CGMulti* multi = qgraphicsitem_cast<CGMulti *>(item);
            modelsList.append(multi->getModel());
        }else if (item->type() == CGOperator::Type) {
            CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);
            operatorsList.append(op->getModel());
        }else if(item->type() == CGText::Type){
            CGText* op = qgraphicsitem_cast<CGText *>(item);
            textsList.append(op);
        }else if(item->type() == CGShape::Type){
            CGShape* op = qgraphicsitem_cast<CGShape *>(item);
            shapesList.append(op);
        }
    }

    /////////////////////////////
    //clone structures & store connections informations
    foreach(CGText* t, textsList){
        CGText* tclone = t->clone();
        textsListCopy.append(tclone);
    }
    foreach(CGShape* s, shapesList){
        CGShape* sclone = s->clone();
        shapesListCopy.append(sclone);
    }

    foreach(CGCompositionModel* comp, modelsList){
        CGMulti* multi = comp->getOuterView()->getParentView()->clone();
        multi->getOuterView()->setPos(comp->getOuterView()->scenePos());
        multi->getModel()->setName(comp->getName()+" "+QString::number(comp->getId()));
        modelsListCopy.append(multi);
        foreach(CGConnector* connector, comp->getOuterView()->getConnectorsList()){
            CGConnector* connectorCopy = multi->getOuterView()->getConnectorById(connector->getId());
            mapNewPrevious.insert(connectorCopy, connector);
            mapPreviousNew.insert(connector, connectorCopy);
            if(connector->getConnectedConnector()!=NULL){
                mapPreviousConnection.insert(connector, connector->getConnectedConnector());
            }
        }
    }
    foreach(CGOperatorModel* ope, operatorsList){
        CGOperator* operatorNew = ope->getView()->clone();
        operatorNew->setPos(ope->getView()->scenePos());
        operatorsListCopy.append(operatorNew);
        foreach(CGConnector* connector, ope->getView()->getConnectorsList()){
            CGConnector* connectorCopy = operatorNew->getConnectorById(connector->getModel()->getId());
            mapNewPrevious.insert(connectorCopy, connector);
            mapPreviousNew.insert(connector, connectorCopy);
            if(connector->getConnectedConnector()!=NULL){
                mapPreviousConnection.insert(connector, connector->getConnectedConnector());
            }
        }
    }
    foreach(CGMulti* comp, modelsListCopy){
        foreach(CGConnector* con, comp->getOuterView()->getConnectorsList()){
            if(!con->isConnected()
                    && mapPreviousConnection.contains(mapNewPrevious.value(con))){
                CGConnector* previousConnected = mapPreviousConnection.value(mapNewPrevious.value(con));
                CGConnector* toconnect = mapPreviousNew.value(previousConnected);
                recordConnectedConnector.insert(toconnect,con);
            }
        }
    }
    foreach(CGOperator* ope, operatorsListCopy){
        foreach(CGConnector* con, ope->getConnectorsList()){
            if(!con->isConnected()
                    && mapPreviousConnection.contains(mapNewPrevious.value(con))){
                CGConnector* previousConnected = mapPreviousConnection.value(mapNewPrevious.value(con));
                CGConnector* toconnect = mapPreviousNew.value(previousConnected);
                recordConnectedConnector.insert(toconnect,con);
            }
        }
    }
    /////////////////////////////
    CGCommandManager::getInstance()->endAction("COPY");
}

void CGCopyPaste::paste(CGComposition* scene,QPointF pos){
    /////////////////////////////
    //add & connect objects

    CGCommandManager::getInstance()->startAction("PASTE");
    if(CClientSingleton::getInstance()->applyStopForEachAction()==true)
        CGCompositionManager::getInstance()->getComposer()->stop();
    //translate objects

    QGraphicsItemGroup* group = new QGraphicsItemGroup(0,scene);
    foreach(CGText* t, textsListCopy){
        group->addToGroup(t);
    }
    foreach(CGShape* s, shapesListCopy){
        group->addToGroup(s);
    }
    foreach(CGOperator* ope, operatorsListCopy){
        group->addToGroup(ope);
    }
    foreach(CGMulti* comp, modelsListCopy){
        group->addToGroup(comp->getOuterView());
    }

    group->setPos(pos);

    scene->destroyItemGroup(group);

    foreach(CGText* t, textsListCopy){
        CGCreateOperator* create = new CGCreateOperator();
        create->setScene(scene);
        create->setItem(t);
        create->setType(CGText::Type);
        create->doCommand();
    }

    foreach(CGShape* s, shapesListCopy){
        CGCreateOperator* create = new CGCreateOperator();
        create->setScene(scene);
        create->setItem(s);
        create->setType(CGShape::Type);
        create->doCommand();
    }

    foreach(CGMulti* comp, modelsListCopy){
        CGCreateOperator* create = new CGCreateOperator();
        create->setScene(scene);
        create->setItem(comp->getOuterView());
        create->setType(CGMulti::Type);
        create->doCommand();
    }

    foreach(CGOperator* ope, operatorsListCopy){
        CGCreateOperator* create = new CGCreateOperator();
        create->setItem(ope);
        create->setScene(scene);
        create->setType(CGOperator::Type);
        create->doCommand();
    }

    foreach(CGOperator* ope, operatorsListCopy){
        foreach(CGConnector* con, ope->getConnectorsList()){
            if(recordConnectedConnector.contains(con)){
                CGConnectConnector* connect = new CGConnectConnector();
                connect->setStartItem(recordConnectedConnector.value(con));
                connect->setEndItem(con);
                connect->doCommand();
            }
        }
    }

    foreach(CGMulti* composition, modelsListCopy){
        foreach(CGConnector* con, composition->getOuterView()->getConnectorsList()){
            if(recordConnectedConnector.contains(con)){
                CGConnectConnector* connect = new CGConnectConnector();
                connect->setStartItem(recordConnectedConnector.value(con));
                connect->setEndItem(con);
                connect->doCommand();
            }
        }
    }
    //end
    /////////////////////////////

    /////////////////////////////
    //selects
    foreach(QGraphicsItem* item, CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems()){
        item->setSelected(false);
    }

    foreach(CGMulti* comp, modelsListCopy){
        comp->getOuterView()->setSelected(true);
    }
    foreach(CGOperator* ope, operatorsListCopy){
        ope->setSelected(true);
    }

    foreach(CGText* t, textsListCopy){
        t->setSelected(true);
    }

    foreach(CGShape* s, shapesListCopy){
        s->setSelected(true);
    }

    modelsListCopy.clear();
    operatorsListCopy.clear();
    textsListCopy.clear();
    shapesListCopy.clear();

    CGCommandManager::getInstance()->endAction("PASTE");
    //end
    /////////////////////////////
}
