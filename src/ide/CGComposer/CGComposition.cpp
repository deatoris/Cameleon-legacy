/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include <QtGui>

#include "CGComposition.h"
#include "../CGGlossary/CGGlossary.h"
#include "CGCompositionItem.h"
#include "CGCompositionFactory.h"
#include "../CGCommand/CGCreateOperator.h"
#include "../CGCommand/CGConnectConnector.h"
#include "../CGCommandManager.h"
#include "CGCompositionModel.h"
#include "CGCompositionManager.h"
#include "CGChangeView.h"
#include "CGDeleteOperator.h"
#include "CGLayoutManager.h"
#include "CLogger.h"
#include<CUtilitySTL.h>
#include "CClient.h"
#include "CGCreateControl.h"
QRectF CGComposition::srect = QRectF(-10000, -5000, 20000, 10000);

//! [0]
CGComposition::CGComposition(CGCompositionModel* model, QMenu *operatorMenu, QMenu *operatorMultiMenu, QMenu * connectorMenu, QMenu* multiMenu, QMenu* compositionMenu, QObject *parent)
    : QGraphicsScene(parent)
{
//    CLogger::getInstance()->registerComponent("COMPOSER");
    this->model = model;
    this->setBackgroundBrush(Qt::black);
    this->addBackgroundRec();
    this->operatorMenu = operatorMenu;
    this->connectorMenu = connectorMenu;
    this->multiMenu = multiMenu;
    this->compositionMenu = compositionMenu;
    this->operatorMultiMenu = operatorMultiMenu;
    sceneMode = Composition;
    line = 0;
    selectionRect = 0;
    lineColor = Qt::white;
    shape = 0;
    layoutItem = 0;
    selectedAction = 0;
    showOrigin = false;

    myItemColor = Qt::white;
    myTextColor = Qt::white;

    //cross activation
    cross = new QGraphicsLineItem(0,this);
    QGraphicsLineItem* line1 = new QGraphicsLineItem(cross,this);
    QGraphicsLineItem* line2 = new QGraphicsLineItem(cross,this);
    cross->setPen(QPen(Qt::white));
    line1->setLine(-10,0,10,0);
    line1->setPen(QPen(Qt::white));
    line2->setLine(0,-10,0,10);
    line2->setPen(QPen(Qt::white));
    cross->hide();
    cross->setPos(0,0);

    if(showOrigin){
        //draw origin
        QGraphicsLineItem* lineX = new QGraphicsLineItem(0,this);
        QGraphicsLineItem* lineY = new QGraphicsLineItem(0,this);
        cross->setPen(QPen(Qt::white));
        lineX->setLine(-10000,0,10000,0);
        lineX->setPen(QPen(Qt::white));
        lineY->setLine(0,-10000,0,10000);
        lineY->setPen(QPen(Qt::white));

    }
    //    textPo = new QGraphicsTextItem(0,this);
    toConnector = 0;
    toMultiConnector = 0;
}

void CGComposition::action(QAction* act){
    selectedAction = act;

}

void CGComposition::action(){
    CGCompositionManager::getInstance()->getComposer()->stop();
    QObject* obj = sender();
    selectedAction = (QAction*)obj;
    //action key
    QString actionKey = selectedAction->text();
    //operator id
    QGraphicsItem* item = this->selectedItems().first();
    CGOperator* ope = 0;
    if(item->type() == CGOperator::Type){
        ope = qgraphicsitem_cast<CGOperator *>(item);
    }
    //send action
    if(ope!=0){
        ope->saveAction(actionKey);
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->actionOperator(ope->getModel()->getId(), actionKey.toStdString());
    }
}

void CGComposition::addBackgroundRec(){
    rect = new QGraphicsRectItem();
    rect->setRect(CGComposition::srect);
    rect->setBrush(Qt::black);
    rect->setPen(QPen(Qt::white));
    this->addItem(rect);
    rect->setZValue(-2000);
}

QGraphicsRectItem* CGComposition::getRect(){
    return rect;
}

void CGComposition::reloadCursor(){
    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::reloadCursor");
    int mode = this->sceneMode;
    if(mode == Target){
        this->view->setCursor(Qt::CrossCursor);
    }else if(mode == InsertText){
        this->view->setCursor(Qt::IBeamCursor);
    }else if(mode == Composition){
        this->view->setCursor(Qt::ArrowCursor);
    }else if(mode == InsertShape){
        this->view->setCursor(Qt::CrossCursor);
    }else if(mode == MoveView){
    }else if(mode == Resize){
        this->view->setCursor(Qt::ArrowCursor);
    }else if(mode == MultiSelection){
        this->view->setCursor(Qt::ArrowCursor);
    }else if(mode == ControlConnection){
        CGConnector* connectortoconnect = this->getConnectorToConnect();
        QList<CGConnector*> list;
        if(this->connectorToConnect->getSide().compare("IN")==0) list = CGCompositionManager::getInstance()->searchCompatibleConnectorForControl(connectortoconnect->getModel()->getDataType(), connectorToConnect->getModel()->getSide(), true);
        if(this->connectorToConnect->getSide().compare("OUT")==0) list = CGCompositionManager::getInstance()->searchCompatibleConnectorForControl(connectortoconnect->getModel()->getDataType(), connectorToConnect->getModel()->getSide(), false);
        foreach(CGConnector* c, list){
            c->setControlMode(true);
        }
    }
    if(mode != ControlConnection){
        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchAllConnector();
        foreach(CGConnector* c, list){
            c->setControlMode(false);
            c->setIsHighLight(false);
            if(c->getIsControlConnected()) c->getConnectedControlConnector()->setIsHighLight(false);
        }
        if(CGLayoutManager::getInstance()->getCurrentLayout()!=0) CGLayoutManager::getInstance()->getCurrentLayout()->update();
        this->update();
    }
}

void CGComposition::setMode(Mode mode){
    this->sceneMode = mode;
    this->reloadCursor();
}

void CGComposition::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::mousePressEvent");
    if(mouseEvent->button() == Qt::LeftButton){
        QList<QGraphicsItem *> startItems = items(mouseEvent->scenePos());

        if(sceneMode == ControlConnection){
            if (startItems.count() > 1
                    && (startItems.first()->type() == CGMultiConnector::Type
                        ||startItems.first()->type() == CGConnector::Type
                        ||startItems.first()->type() == CGLayoutItemConnector::Type)){
                QGraphicsScene::mousePressEvent(mouseEvent);
            }
            CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
            return;
        }
        if(sceneMode == Composition){
            if (startItems.count() > 1
                    && (startItems.first()->type() == CGMultiConnector::Type
                        ||startItems.first()->type() == CGConnector::Type)){

                this->unselectAll();
                this->setMode(MovePlug);

                //MOVE POSITION START
                QList<QGraphicsItem *> startItems2 = items(mouseEvent->scenePos());
                if(startItems2.first()->type() == CGConnector::Type){
                    toConnector = qgraphicsitem_cast<CGConnector *>(startItems2.first());
                    toConnector->setIsMoving(true);
                    toConnector->update();
                }else if(startItems2.first()->type() == CGMultiConnector::Type){
                    toMultiConnector = qgraphicsitem_cast<CGMultiConnector *>(startItems2.first());
                    toMultiConnector->setIsMoving(true);
                    toMultiConnector->update();
                }

                //MOVE POSITION START
            }else if(startItems.count() == 1){
                this->unselectAll();
                this->ungraball();
                this->clearAllFocus();
                this->setMode(MultiSelection);
                QGraphicsScene::mousePressEvent(mouseEvent);
            }else if(startItems.count() == 2 && CGCompositionManager::getInstance()->getCurrentCompositionModel()->getParentCompositionModel() != NULL){
                QList<QGraphicsItem *> startItems = items(mouseEvent->scenePos());
                if(startItems.first()->type() == CGMulti::Type
                        &&startItems.first() == CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()){
                    this->unselectAll();
                    this->setMode(MultiSelection);
                }
            }else if(startItems.count() > 1
                     && startItems.first()->type() == CGEllipse::Type
                     &&startItems.first()->parentItem()->type() == CGLayoutItem::Type){
                CGLayoutItem *startItem =
                        qgraphicsitem_cast<CGLayoutItem *>(startItems.first()->parentItem());
                layoutItem = startItem;
                this->setMode(Resize);
                return;
            }else if(startItems.count() > 1
                     && startItems.first()->type() == CGEllipse::Type){
                CGShape *startItem =
                        qgraphicsitem_cast<CGShape *>(startItems.first()->parentItem());
                shape = startItem;
                this->setMode(Resize);
                return;
            }else if(startItems.count() > 1){
//                this->ungraball();
                QGraphicsScene::mousePressEvent(mouseEvent);
                return;
            }
        }

        QPen pen;
        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::white);
        CGCreateOperator* create;
        QList<QGraphicsItem *> startItems2 = items(mouseEvent->scenePos());
        switch (sceneMode) {
        case InsertText:
            CGCommandManager::getInstance()->startAction("CREATE OPERATOR");
            create = new CGCreateOperator();
            create->setScene(this);
            create->setType(CGText::Type);
            create->setEventPos(mouseEvent->scenePos());
            create->doCommand();
            CGCommandManager::getInstance()->endAction("CREATE OPERATOR");
            break;
        case InsertShape:
            selectionRect = new QGraphicsRectItem();
            multiSelectionPos = mouseEvent->scenePos();
            selectionRect->setRect(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), 0, 0);
            pen.setWidth(0.5);
            selectionRect->setPen(pen);
            selectionRect->setBrush(Qt::black);
            selectionRect->setBoundingRegionGranularity(1);
            this->addItem(selectionRect);
            break;
            break;
        case MovePlug:
            break;
        case MultiSelection:
            this->clearSelection();
            selectionRect = new QGraphicsRectItem();
            multiSelectionPos = mouseEvent->scenePos();
            selectionRect->setRect(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), 0, 0);
            pen.setWidth(0.5);
            selectionRect->setPen(pen);
            selectionRect->setBoundingRegionGranularity(1);
            this->addItem(selectionRect);
            break;
        case Target:
            this->lastLeftPressScenePos = mouseEvent->scenePos();
            cross->setPos(mouseEvent->scenePos());
            cross->update();
            cross->show();
            break;
        default:
            ;
        }
    }else{
        if(sceneMode != ControlConnection){
            this->lastRightPressScenePos = mouseEvent->scenePos();
            QList<QGraphicsItem *> selectItems = selectedItems();
            QList<QGraphicsItem *> underItems = items(mouseEvent->scenePos());
            if (selectItems.count() >= 2){
                multiMenu->exec(mouseEvent->screenPos());
            }else if(selectItems.count() == 0 && underItems.count() == 1){
                compositionMenu->exec(mouseEvent->screenPos());
            }else if(selectItems.count() == 0 && underItems.count() == 2){
                compositionMenu->exec(mouseEvent->screenPos());
            }
        }
    }

    if(sceneMode != ControlConnection){
        QGraphicsScene::mousePressEvent(mouseEvent);
        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchAllConnector();
        foreach(CGConnector* c, list){
            if(c->getIsControlConnected()){
                c->setIsHighLight(false);
                c->getConnectedControlConnector()->setIsHighLight(false);
            }
        }
    }
}

int CGComposition::distance(QPointF p1, QPointF p2){
    int x = (p1.x()-p2.x());
    int y = (p1.y()-p2.y());
    return sqrt(x*x+y*y);
}

void CGComposition::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::mouseMoveEvent");
    if(sceneMode == MovePlug){
        QList<QGraphicsItem *> startItems = items(mouseEvent->scenePos());
        CGConnector *startItem = toConnector;
        if(toConnector!=0 && startItems.first() == toConnector){       //INNER
            toConnector->moveTo(mouseEvent->scenePos());
        }else if(toMultiConnector!=0 && startItems.first() == toMultiConnector){
            if(toMultiConnector == toMultiConnector->getParentConnector()->getInnerConnector()){
                toMultiConnector->getParentConnector()->getInnerConnector()->moveTo(mouseEvent->scenePos());
                double r = (toMultiConnector->getParentConnector()->getInnerConnector()->getRayon())/2;
                double teta = acos(toMultiConnector->getParentConnector()->getInnerConnector()->pos().x()/r)*(SCD::sgn(toMultiConnector->getParentConnector()->getInnerConnector()->pos().y()));

                double x = cos(teta)*((toMultiConnector->getParentConnector()->getOuterConnector()->getRayon()-20)+20)/2;
                double y = sin(teta)*((toMultiConnector->getParentConnector()->getOuterConnector()->getRayon()-20)+20)/2;
                toMultiConnector->getParentConnector()->getOuterConnector()->setPos(x,y);
            }else{
                toMultiConnector->getParentConnector()->getOuterConnector()->moveTo(mouseEvent->scenePos());
                double r = (toMultiConnector->getParentConnector()->getOuterConnector()->getRayon())/2;
                double teta = acos(toMultiConnector->getParentConnector()->getOuterConnector()->pos().x()/r)*(SCD::sgn(toMultiConnector->getParentConnector()->getOuterConnector()->pos().y()));

                double x = cos(teta)*((toMultiConnector->getParentConnector()->getInnerConnector()->getRayon()-20)+20)/2;
                double y = sin(teta)*((toMultiConnector->getParentConnector()->getInnerConnector()->getRayon()-20)+20)/2;
                toMultiConnector->getParentConnector()->getInnerConnector()->setPos(x,y);
            }
        }else if(toMultiConnector!=0){ //OUTTER
            startItem = toMultiConnector;
            //CONNECTION MODE START
            if(!startItem->isConnected()){
                this->setMode(Connect);
                if(startItem->getModel()!=NULL){
                    QList<CGConnector*> list = CGCompositionManager::getInstance()->searchCompatibleConnector(startItem->getModel()->getDataType(), startItem->getModel()->getSide());
                    foreach(CGConnector* c, list){
                        if(!startItem->getModel()->getCGOperatorModel()->has(c->getModel())) c->setCompatibleMode(true);
                    }
                }else{
                    CGMultiConnector* connector = toMultiConnector;
                    if(connector->getNearest() != NULL){
                        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchCompatibleConnector(connector->getNearest()->getModel()->getDataType(), connector->getNearest()->getModel()->getSide());
                        foreach(CGConnector* c, list){
                            if(c->parentItem()!=toMultiConnector->parentItem()) c->setCompatibleMode(true);
                        }
                    }else{
                        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchCompatibleConnector("CDATAGENERIC", connector->getSide());
                        foreach(CGConnector* c, list){
                            if(c->parentItem()!=toMultiConnector->parentItem()) c->setCompatibleMode(true);
                        }
                    }
                }

                foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCurrentCompositionModel()->getCompositionModelList()){
                    if(model->getOuterView()!=toMultiConnector->parentItem()) model->getOuterView()->setIsCompatibleMode(true);
                }
                if(CGCompositionManager::getInstance()->getCurrentCompositionModel()->getParentCompositionModel() != NULL){
                    CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->setIsCompatibleMode(true);
                }

                line = new QGraphicsLineItem(QLineF(toMultiConnector->scenePos(),
                                                    toMultiConnector->scenePos()));
                line->setPen(QPen(lineColor, 2));

                addItem(line);
            }
            //CONNECTION MODE START
        }else if(toConnector!=0){ //OUTTER
            //CONNECTION MODE START
            if(!startItem->isConnected()){
                this->setMode(Connect);
                if(startItem->getModel()!=NULL){
                    QList<CGConnector*> list = CGCompositionManager::getInstance()->searchCompatibleConnector(startItem->getModel()->getDataType(), startItem->getModel()->getSide());
                    foreach(CGConnector* c, list){
                        if(!startItem->getModel()->getCGOperatorModel()->has(c->getModel())) c->setCompatibleMode(true);
                    }
                }else{
                    CGMultiConnector* connector = qgraphicsitem_cast<CGMultiConnector *>(startItems.first());
                    if(connector->getNearest() != NULL){
                        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchCompatibleConnector(connector->getNearest()->getModel()->getDataType(), connector->getNearest()->getModel()->getSide());
                        foreach(CGConnector* c, list){
                            c->setCompatibleMode(true);
                        }
                    }else{
                        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchCompatibleConnector("GENERIC", connector->getSide());
                        foreach(CGConnector* c, list){
                            c->setCompatibleMode(true);
                        }
                    }
                }

                foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCurrentCompositionModel()->getCompositionModelList()){
                    model->getOuterView()->setIsCompatibleMode(true);
                }
                if(CGCompositionManager::getInstance()->getCurrentCompositionModel()->getParentCompositionModel() != NULL){
                    CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->setIsCompatibleMode(true);
                }

                line = new QGraphicsLineItem(QLineF(toConnector->scenePos(),
                                                    toConnector->scenePos()));
                line->setPen(QPen(lineColor, 2));

                addItem(line);
            }
            //CONNECTION MODE START
        }
    }else if (sceneMode == Connect && line != 0){
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
        line->setZValue(-1000);
        if(toConnector!=0){
            //            toConnector->moveTo(mouseEvent->scenePos());
            QLineF newLine(toConnector->scenePos(), mouseEvent->scenePos());
            line->setLine(newLine);
        }
    }else if (sceneMode == Composition && !(mouseEvent->modifiers().testFlag(Qt::AltModifier))) {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }else if((sceneMode == MultiSelection || sceneMode == InsertShape) && selectionRect != 0){
        QRect rect;
        rect.setRect(multiSelectionPos.x(),
                     multiSelectionPos.y(),
                     mouseEvent->scenePos().x()-multiSelectionPos.x(),
                     mouseEvent->scenePos().y()-multiSelectionPos.y());

        if((mouseEvent->scenePos().x()-multiSelectionPos.x()) < 0){
            int x = multiSelectionPos.x() + (mouseEvent->scenePos().x()-multiSelectionPos.x());
            int w = -(mouseEvent->scenePos().x()-multiSelectionPos.x());
            rect.setX(x);
            rect.setWidth(w);
        }

        if((mouseEvent->scenePos().y()-multiSelectionPos.y()) < 0){
            int y = multiSelectionPos.y() + (mouseEvent->scenePos().y()-multiSelectionPos.y());
            int h = -(mouseEvent->scenePos().y()-multiSelectionPos.y());
            rect.setY(y);
            rect.setHeight(h);
        }
        selectionRect->setRect(rect);
    }else if(sceneMode == Resize && layoutItem != 0){
        layoutItem->resize(mouseEvent->scenePos());
    }else if(sceneMode == Resize && shape != 0){
        shape->resize(mouseEvent->scenePos());
    }
    //    QString p= QString::number(mouseEvent->scenePos().x()) + " " + QString::number(mouseEvent->scenePos().y());
    //    textPo->setPlainText(p);
    //    textPo->setDefaultTextColor(Qt::white);
    //    textPo->setPos(mouseEvent->scenePos().x()+20,mouseEvent->scenePos().y()+20);
}

CControl* CGComposition::getControlToConnect(){
    return control;
}

CGConnector* CGComposition::getConnectorToConnect(){
    return connectorToConnect;
}

void CGComposition::setToConnect(CControl* control, CGConnector* connectorToConnect){
    this->control = control;
    this->connectorToConnect = connectorToConnect;
}

void CGComposition::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::mouseReleaseEvent");
    if(sceneMode == Composition && this->itemAt(mouseEvent->scenePos())!=rect){
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        return;
    }
    if(sceneMode == MovePlug){
        if(toMultiConnector!=0) toMultiConnector->setIsMoving(false);
        if(toMultiConnector!=0) toMultiConnector->update();
        if(toConnector!=0) toConnector->setIsMoving(false);
        if(toConnector!=0) toConnector->update();
        toConnector = 0;
        toMultiConnector = 0;
        this->setMode(Composition);
    }else if (line != 0 && sceneMode == Connect) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if ((startItems.count() > 0 && endItems.count() > 0 &&
             (startItems.first()->type() == CGConnector::Type || startItems.first()->type() == CGMultiConnector::Type) &&
             (endItems.first()->type() == CGConnector::Type || endItems.first()->type() == CGMultiConnector::Type) &&
             startItems.first() != endItems.first())
                ) {
            CGConnector *startItem =
                    qgraphicsitem_cast<CGConnector *>(startItems.first());
            CGConnector *endItem =
                    qgraphicsitem_cast<CGConnector *>(endItems.first());

            bool ok = true;
            if(startItems.first()->type() == CGMultiConnector::Type){
                CGMultiConnector *m = qgraphicsitem_cast<CGMultiConnector *>(startItems.first());
                if(m->getSide().compare("OUT") == 0
                        || (m->getSide().compare("IN") == 0
                            && (!m->getOtherSide()->isConnected() || (m->getOtherSide()->isConnected() && !m->getOtherSide()->getConnectedConnector()->getIsControlConnected())))){
                    ok = true;

                }else{
                    ok = false;
                }
                startItem = qgraphicsitem_cast<CGMultiConnector *>(startItems.first());
            }
            if(endItems.first()->type() == CGMultiConnector::Type){
                CGMultiConnector *m = qgraphicsitem_cast<CGMultiConnector *>(endItems.first());
                if(m->getSide().compare("OUT") == 0
                        || (m->getSide().compare("IN") == 0
                            && (!m->getOtherSide()->isConnected() || (m->getOtherSide()->isConnected() && !m->getOtherSide()->getConnectedConnector()->getIsControlConnected())))){
                    ok = true;

                }else{
                    ok = false;
                }
                endItem = qgraphicsitem_cast<CGMultiConnector *>(endItems.first());
            }
            if(endItem->getCompatibleMod() && startItem->isIn() != endItem->isIn() &&
                    !endItem->isConnected()
                    && startItem->parentItem() != endItem->parentItem() && ok){
                CGCommandManager::getInstance()->startAction("CONNECT");
                CGConnectConnector* connect = new CGConnectConnector();
                connect->setStartItem(startItem);
                connect->setEndItem(endItem);
                connect->setColor(lineColor);
                connect->doCommand();
                CGCommandManager::getInstance()->endAction("CONNECT");
            }
        }else if(startItems.count() > 0 && endItems.count() > 0 &&
                 (startItems.first()->type() == CGConnector::Type || startItems.first()->type() == CGMultiConnector::Type) &&
                 (endItems.first()->type() == CGMulti::Type)){
            CGConnector *startItem =
                    qgraphicsitem_cast<CGConnector *>(startItems.first());
            CGMulti *multi =
                    qgraphicsitem_cast<CGMulti *>(endItems.first());
            if(startItems.first()->type() == CGMultiConnector::Type){
                startItem = qgraphicsitem_cast<CGMultiConnector *>(startItems.first());
            }
            if(multi->getMode() == CGMulti::OUTER && startItem->parentItem() != multi){
                CGCommandManager::getInstance()->startAction("CONNECT");
                CGConnectConnector* connect = new CGConnectConnector();
                connect->setStartItem(startItem);
                connect->setMulti(multi);
                connect->setColor(lineColor);
                connect->doCommand();
                CGCommandManager::getInstance()->endAction("CONNECT");
            }
        }else if(startItems.count() > 0 && endItems.count() == 1
                 && (startItems.first()->type() == CGConnector::Type || startItems.first()->type() == CGMultiConnector::Type) && this->model->getParentCompositionModel()!=NULL){
            CGConnector *startItem =
                    qgraphicsitem_cast<CGConnector *>(startItems.first());
            if(startItems.first()->type() == CGMultiConnector::Type){
                startItem = qgraphicsitem_cast<CGMultiConnector *>(startItems.first());
            }
            CGConnectConnector* connect = new CGConnectConnector();
            connect->setStartItem(startItem);
            connect->setEndItem(NULL);
            connect->setColor(lineColor);
            connect->doCommand();
        }
        this->setMode(Composition);

        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchAllConnector();
        foreach(CGConnector* c, list){
            c->setCompatibleMode(false);
        }

        foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCurrentCompositionModel()->getCompositionModelList()){
            model->getOuterView()->setIsCompatibleMode(false);
        }
        if(CGCompositionManager::getInstance()->getCurrentCompositionModel()->getParentCompositionModel() != NULL){
            CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->setIsCompatibleMode(false);
        }

        if(toMultiConnector!=0) toMultiConnector->setIsMoving(false);
        if(toMultiConnector!=0) toMultiConnector->update();
        if(toConnector!=0) toConnector->setIsMoving(false);
        if(toConnector!=0) toConnector->update();
        toConnector = 0;
        toMultiConnector = 0;
        this->setMode(Composition);

        this->update();
    }else if(sceneMode == MultiSelection && selectionRect != 0){
        QPainterPath path;
        path.addRect(selectionRect->rect());
        this->setSelectionArea(path);
        this->removeItem(selectionRect);
        this->setMode(Composition);
    }else if(sceneMode == InsertShape && selectionRect != 0){
        CGCommandManager::getInstance()->startAction("CREATE SHAPE");
        CGCreateOperator* create;
        create = new CGCreateOperator();
        create->setScene(this);
        create->setType(CGShape::Type);
        create->setEventPos(mouseEvent->scenePos());
        create->setRect(selectionRect->rect());
        create->doCommand();
        this->removeItem(selectionRect);
        CGCommandManager::getInstance()->endAction("CREATE SHAPE");
    }else if(sceneMode == Resize && layoutItem != 0){
        layoutItem = 0;
        this->setMode(Composition);
    }else if(sceneMode == Resize && shape != 0){
        shape = 0;
        this->setMode(Composition);
        QList<CGConnector*> list = CGCompositionManager::getInstance()->searchAllConnector();
        foreach(CGConnector* c, list){
            c->setCompatibleMode(false);
            c->setIsHighLight(false);
        }


        foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCurrentCompositionModel()->getCompositionModelList()){
            model->getOuterView()->setIsCompatibleMode(false);
        }
        if(CGCompositionManager::getInstance()->getCurrentCompositionModel()->getParentCompositionModel() != NULL){
            CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->setIsCompatibleMode(false);
        }

        this->update();
    }
    line = 0;
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

}
#include "CGProject.h"
#include "CGProjectTree.h"
void CGComposition::dropEvent ( QGraphicsSceneDragDropEvent * event ){

    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::dropEvent");
    QTreeWidget *treeWidget = CGGlossary::getInstance()->getComposerTree();
    QList<QTreeWidgetItem*> wList;
    QString ressourceType = "Ressource";
    if(event->source() == treeWidget){
        wList = treeWidget->selectedItems();
    }else if(event->source() == CGProject::getInstance()->getComposerTree()){
        wList = CGProject::getInstance()->getComposerTree()->selectedItems();
        ressourceType = "RessourceProject";
    }

    CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
    if(wList.size()>0){
        QVariant v = wList[0]->data(0,Qt::UserRole);
        QString s = v.toString();
        if(wList[0]->text(1).compare("dir") == 0){
            QVariant v = wList[0]->data(0,Qt::UserRole);
            QString s = v.toString();
            if(s.compare("ressource") == 0){
                //TODO: add ressource
                CGCommandManager::getInstance()->startAction("CREATE CONTROL");
                CGCreateControl* create = new CGCreateControl();
                create->setType(CGLayoutItem::Type);
                create->setEventPos(event->scenePos());
                create->setName(ressourceType);
                create->setPath(wList[0]->text(2));
                create->setScene(this);
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE CONTROL");
            }
        }else if(wList[0]->text(1).compare("file") == 0){
            if(s.compare("ressource") == 0){
                CGCommandManager::getInstance()->startAction("CREATE CONTROL");
                CGCreateControl* create = new CGCreateControl();
                create->setType(CGLayoutItem::Type);
                create->setEventPos(event->scenePos());
                create->setName(ressourceType);
                create->setPath(wList[0]->text(2));
                create->setScene(this);
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE CONTROL");
            }else if(s.compare("pattern") == 0){
                CGCommandManager::getInstance()->startAction("CREATE PATTERN");
                CGCreateOperator* create = new CGCreateOperator();
                create->setScene(this);
                create->setIsPattern(true);
                create->setEventPos(event->scenePos());
                create->setOperatorName(wList[0]->text(2));
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE PATTERN");
            }else if(s.compare("operator") == 0){
                CGCommandManager::getInstance()->startAction("CREATE OPERATOR");
                CGCreateOperator* create = new CGCreateOperator();
                create->setScene(this);
                create->setType(CGOperator::Type);
                create->setEventPos(event->scenePos());
                create->setOperatorName(wList[0]->text(2));
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE OPERATOR");
            }else if(s.compare("control") == 0){
                CGCommandManager::getInstance()->startAction("CREATE CONTROL");
                CGCreateControl* create = new CGCreateControl();
                create->setType(CGLayoutItem::Type);
                create->setEventPos(event->scenePos());
                create->setName(wList[0]->text(2));
                create->setScene(this);
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE CONTROL");
            }
        }
    }
}

CGCompositionModel* CGComposition::getModel(){
    return model;
}


void CGComposition::dragEnterEvent ( QGraphicsSceneDragDropEvent *  ){

}

void CGComposition::dragMoveEvent ( QGraphicsSceneDragDropEvent *  ){

}

void CGComposition::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent){
    if(sceneMode != ControlConnection){
        CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::mouseDoubleClickEvent");
        QList<QGraphicsItem *> itemsL = items(mouseEvent->scenePos());
        if(itemsL.count() == 1){
            CGCompositionManager::getInstance()->up();
        }else if(itemsL.count() == 2 && this->model->getParentCompositionModel()!=NULL){
            CGCompositionManager::getInstance()->up();
        }else{
            if (itemsL.first()->type() == CGOperator::Type){
                CGCompositionManager::getInstance()->getComposer()->toFront();
            }
            if (itemsL.first()->type() == CGMulti::Type){
                CGMulti *item = qgraphicsitem_cast<CGMulti *>(itemsL.first());
                CGCommandManager::getInstance()->startAction("CHANGE VIEW");
                CGChangeView* command = new CGChangeView();
                command->setCompositionModel(item->getModel());
                command->doCommand();
                CGCommandManager::getInstance()->endAction("CHANGE VIEW");
            }
        }
        QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
    }
}


QPointF CGComposition::getLastSceneRightPressedPos(){
    return QPointF(this->lastRightPressScenePos.x(),this->lastRightPressScenePos.y());
}

QPointF CGComposition::getLastSceneLeftPressedPos(){
    return QPointF(this->lastLeftPressScenePos.x(),this->lastLeftPressScenePos.y());
}

void CGComposition::ungraball(){
    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::ungraball");
    //foreach (QGraphicsItem *item, this->items()) {
        if(this->mouseGrabberItem()!=0) this->mouseGrabberItem()->ungrabMouse();
        if(this->mouseGrabberItem()!=0) this->mouseGrabberItem()->ungrabKeyboard();
        if(this->mouseGrabberItem()!=0) this->mouseGrabberItem()->clearFocus();
        if(this->focusItem()!=0)this->focusItem()->ungrabKeyboard();
        if(this->focusItem()!=0)this->focusItem()->ungrabMouse();
        if(this->focusItem()!=0)this->focusItem()->clearFocus();
        //}
    this->setFocusItem(0);
}

void CGComposition::unselectAll(){

    foreach (QGraphicsItem *item, this->items()) {
        item->setSelected(false);
        if(this->mouseGrabberItem() == item) item->ungrabMouse();
        if(item->isVisible()) item->grabKeyboard();
        if(item->isVisible()) item->ungrabKeyboard();
        if(item->isVisible()) item->clearFocus();
    }
    this->setFocusItem(0);
}

void CGComposition::editorLostFocus(CGText *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void CGComposition::keyPressEvent ( QKeyEvent * keyEvent ){
    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::keyPressEvent");
    if(this->sceneMode == Composition){
        /*if(keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == Qt::Key_Z){
            CGCommandManager::getInstance()->undo();
        }else if(keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == Qt::Key_Y){
            CGCommandManager::getInstance()->redo();
        }else if(keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == Qt::Key_C){
            CGCompositionManager::getInstance()->getComposer()->copy();
        }else if(keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == Qt::Key_V){
            CGCompositionManager::getInstance()->getComposer()->paste();
        }else */if(keyEvent->key() == Qt::Key_W){
            CGCompositionManager::getInstance()->getComposer()->showInfo();
        }/*else if(keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == Qt::Key_X){
            CGCompositionManager::getInstance()->getComposer()->cut();
        }*/else if(keyEvent->key() == Qt::Key_Delete){
            CGCompositionManager::getInstance()->getComposer()->deletecomponent();
        }else if(keyEvent->key() == Qt::Key_A){
            bool t = true;
            foreach(QGraphicsItem* item, this->items()){
                if(item->hasFocus()) t = false;

            }
            if(t){
                foreach(QGraphicsItem* item, this->items()){
                    item->setSelected(true);
                }
            }
        }
    }
    QGraphicsScene::keyPressEvent(keyEvent);
}

CGCompositionView* CGComposition::getView(){
    return this->view;
}

void CGComposition::setView(CGCompositionView* view){
    this->view = view;
}

void CGComposition::clearAllFocus(){
    foreach(QGraphicsItem* item, this->items()){
        item->clearFocus();
        item->setSelected(false);
        if(item->type() == CGLayoutItem::Type){
            CGLayoutItem *layI =
                    qgraphicsitem_cast<CGLayoutItem *>(item);
            if(this->mouseGrabberItem() == item) layI->ungrabKeyboard();
            if(this->mouseGrabberItem() == item) layI->ungrabMouse();
            if(this->mouseGrabberItem() == item) layI->getControl()->clearFocus();
        }
    }
    foreach(CGOperatorModel* ope, model->getOperatorModelList()){
        ope->getView()->clearFocus();
        ope->getView()->setSelected(false);
    }
    foreach(CGCompositionModel* composition, model->getCompositionModelList()){
        composition->getOuterView()->clearFocus();
        composition->getOuterView()->setSelected(false);
        composition->getInnerView()->clearFocus();
        composition->getInnerView()->setSelected(false);
        composition->getInnerView()->getParentView()->clearFocus();
        composition->getInnerView()->getParentView()->setSelected(false);
    }
}

void CGComposition::load(){
    CLogger::getInstance()->log("COMPOSER",CLogger::DEBUG,"CGComposition::load");
    foreach(CGOperatorModel* ope, model->getOperatorModelList()){
        this->addItem(ope->getView());
    }
    foreach(CGCompositionModel* composition, model->getCompositionModelList()){
        this->addItem(composition->getOuterView());
    }
    if(model->getParentCompositionModel()!=NULL){
        this->getRect()->setBrush(Qt::black);
        this->setBackgroundBrush(Qt::black);
    }else{
        this->getRect()->setBrush(Qt::black);
        this->setBackgroundBrush(Qt::black);
    }
}

QMenu* CGComposition::getCommentShapeMenu(){
    return NULL;
}

QMenu* CGComposition::getCommentTextMenu(){
    return NULL;

}

QMenu* CGComposition::getOperatorMenu(){
    return operatorMenu;

}

QMenu* CGComposition::getOperatorMultiMenu(){
    return operatorMultiMenu;

}

QMenu* CGComposition::getConnectorMenu(){
    return connectorMenu;

}

QMenu* CGComposition::getMultiMenu(){
    return multiMenu;

}

QMenu* CGComposition::getCompositionMenu(){
    return compositionMenu;
}

void CGComposition::setTextColor(const QColor &color)
{
    myTextColor = color;
    foreach(QGraphicsItem* item, selectedItems()){
        if (item->type() == CGText::Type) {
            CGText *shape =
                    qgraphicsitem_cast<CGText *>(item);
            shape->setDefaultTextColor(myTextColor);
        }
    }
}

void CGComposition::setItemColor(const QColor &color)
{
    myItemColor = color;
    foreach(QGraphicsItem* item, selectedItems()){
        if (item->type() == CGShape::Type) {
            CGShape *shape =
                    qgraphicsitem_cast<CGShape *>(item);
            shape->setColor(myItemColor);
            shape->update();
        }
    }
}

void CGComposition::setFont(const QFont &font)
{
    myFont = font;

    foreach(QGraphicsItem* item, selectedItems()){
        if (item->type() == CGText::Type) {
            CGText *shape =
                    qgraphicsitem_cast<CGText *>(item);
            shape->setFont(myFont);
        }
    }
}

bool CGComposition::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

int CGComposition::getMode(){
    return this->sceneMode;
}
