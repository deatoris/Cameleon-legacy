/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGLayout.h"
#include <QtGui>

#include "CGLayout.h"
#include "../CGGlossary/CGGlossary.h"
#include "CGLayoutItem.h"
#include "CGLayoutFactory.h"
#include "../CGCommandManager.h"
#include "CGLayoutManager.h"
#include "CGChangeView.h"
#include "CGDeleteOperator.h"
#include "CGCreateControl.h"
#include "CGCreateOperator.h"
#include "CGCompositionManager.h"
#include "CGProject.h"
#include "CGProjectTree.h"
int CGLayout::lid = 0;

//! [0]
CGLayout::CGLayout(QMenu *operatorMenu, QMenu * connectorMenu, QObject *parent)
    : QGraphicsScene(parent)
{
    this->id = lid;
    lid++;
    this->view = new CGLayoutView(this);
    this->setBackgroundBrush(Qt::darkGray);

    isPopup = false;

    this->operatorMenu = operatorMenu;
    this->connectorMenu = connectorMenu;
    sceneMode = Composition;

    selectionRect = 0;
    shape = 0;
    layoutItem = 0;

    showOrigin = false;

    myItemColor = Qt::white;
    myTextColor = Qt::white;

    rect = new QGraphicsRectItem();
    //40880049: ControllerView save/load position
    rect->setRect(-10000, -5000, 20000, 10000);
    //    rect->setRect(-960*2, -540*2, 1920*2, 1080*2);
    rect->setBrush(Qt::black);
    rect->setPen(QPen(Qt::white));
    this->addItem(rect);
    rect->setZValue(-2000);

    if(showOrigin){
        //cross activation
        QGraphicsLineItem* cross = new QGraphicsLineItem(0,this);
        QGraphicsLineItem* line1 = new QGraphicsLineItem(cross,this);
        QGraphicsLineItem* line2 = new QGraphicsLineItem(cross,this);
        cross->setPen(QPen(Qt::white));
        line1->setLine(-10,0,10,0);
        line1->setPen(QPen(Qt::white));
        line2->setLine(0,-10,0,10);
        line2->setPen(QPen(Qt::white));
        cross->hide();
        cross->setPos(0,0);

        //draw origin
        QGraphicsLineItem* lineX = new QGraphicsLineItem(0,this);
        QGraphicsLineItem* lineY = new QGraphicsLineItem(0,this);
        cross->setPen(QPen(Qt::white));
        lineX->setLine(-10000,0,10000,0);
        lineX->setPen(QPen(Qt::white));
        lineY->setLine(0,-10000,0,10000);
        lineY->setPen(QPen(Qt::white));

    }

}
void CGLayout::setName(QString name){
    this->name = name;
}

void CGLayout::setType(QString type){
    this->type = type;
}

QString CGLayout::getType(){
    return this->type;
}

int CGLayout::getId(){
    return id;
}


QString CGLayout::getName(){
    return name;
}

QGraphicsRectItem* CGLayout::getRect(){
    return rect;
}

void CGLayout::reloadCursor(){
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
    }
}

void CGLayout::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::RightButton){
        if(this->itemAt(mouseEvent->scenePos()) != rect){
            CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
            QGraphicsScene::mousePressEvent(mouseEvent);
            return;
        }
    }
    if(mouseEvent->button() == Qt::LeftButton){
        QList<QGraphicsItem *> startItems = items(mouseEvent->scenePos());

        if(sceneMode == Composition){
            if(startItems.count() == 1){
                this->unselectAll();
                this->clearFocus();
                this->setMode(MultiSelection);
            }else if(startItems.count() > 1
                     && startItems.first()->type() == CGEllipse::Type
                     &&startItems.first()->parentItem()->type() == CGLayoutItem::Type){
                CGLayoutItem *startItem =
                        qgraphicsitem_cast<CGLayoutItem *>(startItems.first()->parentItem());
                layoutItem = startItem;
                this->setMode(Resize);
                return;
            }else if(startItems.count() > 1
                     && startItems.first()->type() == CGEllipse::Type
                     &&startItems.first()->parentItem()->type() == CGShape::Type){
                CGShape *startItem =
                        qgraphicsitem_cast<CGShape *>(startItems.first()->parentItem());
                shape = startItem;
                this->setMode(Resize);
                return;
            }else if(this->itemAt(mouseEvent->scenePos()) != rect){
                CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
                QGraphicsScene::mousePressEvent(mouseEvent);
                return;
            }

        }

        QPen pen;
        CGCreateOperator* create;
        pen.setStyle(Qt::DotLine);
        pen.setColor(Qt::white);
        switch (sceneMode) {
        case MultiSelection:
            selectionRect = new QGraphicsRectItem();
            multiSelectionPos = mouseEvent->scenePos();
            selectionRect->setRect(mouseEvent->scenePos().x(), mouseEvent->scenePos().y(), 0, 0);
            pen.setWidth(0.5);
            selectionRect->setPen(pen);
            selectionRect->setBoundingRegionGranularity(1);
            this->addItem(selectionRect);
            break;
        case InsertText:
            CGCommandManager::getInstance()->startAction("CREATE OPERATOR");
            create = new CGCreateOperator();
            create->setLayout(this);
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
            selectionRect->setBrush(Qt::gray);
            selectionRect->setBoundingRegionGranularity(1);
            this->addItem(selectionRect);
            break;
            break;
        default:
            ;
        }
    }/*else{
        QList<QGraphicsItem *> selectItems = selectedItems();
        QList<QGraphicsItem *> underItems = items(mouseEvent->scenePos());
        if(selectItems.count() == 0 && underItems.count() == 1){
            CGLayoutManager::getInstance()->getControllerMenu()->exec(mouseEvent->screenPos());
        }
    }*/
    QList<QGraphicsItem*> itemsL = this->items();
    foreach(QGraphicsItem* item, itemsL){
        if(item->type() == CGLayoutItem::Type){
            CGLayoutItem* layout = qgraphicsitem_cast<CGLayoutItem *>(item);
            QList<CGLayoutItemConnector*> list = layout->getConnectorsL();
            foreach(CGLayoutItemConnector* c, list){
                c->setCompatibleMode(false);
                c->setIsHighLight(false);
                c->parentItem()->setSelected(false);
                if(c->isConnected()) c->getConnectedConnector()->setIsHighLight(false);
            }
            layout->setSelected(false);
        }
    }
    CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);

    this->update();
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void CGLayout::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(sceneMode == Composition && this->itemAt(mouseEvent->scenePos())){
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
}

void CGLayout::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(sceneMode == Composition && this->itemAt(mouseEvent->scenePos())!=rect){
        QGraphicsScene::mouseReleaseEvent(mouseEvent);
        return;
    }

    if(sceneMode == MultiSelection && selectionRect != 0){
        QPainterPath path;
        path.addRect(selectionRect->rect());
        this->setSelectionArea(path);
        this->removeItem(selectionRect);
        this->setMode(Composition);
    }else if(sceneMode == InsertShape && selectionRect != 0){
        CGCommandManager::getInstance()->startAction("CREATE OPERATOR");
        CGCreateOperator* create;
        create = new CGCreateOperator();
        create->setLayout(this);
        create->setType(CGShape::Type);
        create->setEventPos(mouseEvent->scenePos());
        create->setRect(selectionRect->rect());
        create->doCommand();
        this->removeItem(selectionRect);
        CGCommandManager::getInstance()->endAction("CREATE OPERATOR");
    }else if(sceneMode == Resize && layoutItem != 0){
        layoutItem = 0;
        this->setMode(Composition);
    }else if(sceneMode == Resize && shape != 0){
        shape = 0;
        this->setMode(Composition);
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);

}

void CGLayout::dropEvent ( QGraphicsSceneDragDropEvent * event ){
    QTreeWidget *treeWidget = CGGlossary::getInstance()->getControllerTree();
    QList<QTreeWidgetItem*> wList;
    QString ressourceName = "Ressource";
    if(event->source() == treeWidget){
        wList = treeWidget->selectedItems();
    }else if(event->source() == CGProject::getInstance()->getControllerTree()){
        wList = CGProject::getInstance()->getControllerTree()->selectedItems();
        ressourceName = "RessourceProject";
    }
    if(wList.size()>0){
        if(wList[0]->text(1).compare("dir") == 0){
            QVariant v = wList[0]->data(0,Qt::UserRole);
            QString s = v.toString();
            if(s.compare("ressource") == 0){
                //TODO: add ressource
                CGCommandManager::getInstance()->startAction("CREATE CONTROL");
                CGCreateControl* create = new CGCreateControl();
                create->setType(CGLayoutItem::Type);
                create->setEventPos(event->scenePos());
                create->setName(ressourceName);
                create->setPath(wList[0]->text(2));
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE CONTROL");
            }
        }else if(wList[0]->text(1).compare("file") == 0){
            QVariant v = wList[0]->data(0,Qt::UserRole);
            QString s = v.toString();
            if(s.compare("ressource") == 0){
                CGCommandManager::getInstance()->startAction("CREATE CONTROL");
                CGCreateControl* create = new CGCreateControl();
                create->setType(CGLayoutItem::Type);
                create->setEventPos(event->scenePos());
                create->setName(ressourceName);
                create->setPath(wList[0]->text(2));
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE CONTROL");
            }else if(s.compare("pattern") == 0){
                CGCommandManager::getInstance()->startAction("CREATE PATTERN");
                CGCreateOperator* create = new CGCreateOperator();
                create->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                create->setLayout(this);
                create->setIsPattern(true);
                create->setEventPos(event->scenePos());
                create->setOperatorName(wList[0]->text(2));
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE PATTERN");
            }else {
                CGCommandManager::getInstance()->startAction("CREATE CONTROL");
                CGCreateControl* create = new CGCreateControl();
                create->setType(CGLayoutItem::Type);
                create->setEventPos(event->scenePos());
                create->setName(wList[0]->text(2));
                create->doCommand();
                CGCommandManager::getInstance()->endAction("CREATE CONTROL");
            }
        }
    }
}

void CGLayout::dragEnterEvent ( QGraphicsSceneDragDropEvent *  ){

}

void CGLayout::dragMoveEvent ( QGraphicsSceneDragDropEvent *  ){

}

void CGLayout::mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent){

    QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
}


QPointF CGLayout::getLastSceneRightPressedPos(){
    return QPointF(this->lastRightPressScenePos.x(),this->lastRightPressScenePos.y());
}

QPointF CGLayout::getLastSceneLeftPressedPos(){
    return QPointF(this->lastLeftPressScenePos.x(),this->lastLeftPressScenePos.y());
}

void CGLayout::unselectAll(){
    foreach (QGraphicsItem *item, this->items()) {
        item->setSelected(false);
    }
    this->setFocusItem(0);
}

void CGLayout::editorLostFocus(CGText *item)
{
    QTextCursor cursor = item->textCursor();
    cursor.clearSelection();
    item->setTextCursor(cursor);

    if (item->toPlainText().isEmpty()) {
        removeItem(item);
        item->deleteLater();
    }
}

void CGLayout::keyPressEvent ( QKeyEvent * keyEvent ){
    /*if(keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == Qt::Key_Z){
        CGCommandManager::getInstance()->undo();
    }else if(keyEvent->modifiers().testFlag(Qt::ControlModifier) && keyEvent->key() == Qt::Key_Y){
        CGCommandManager::getInstance()->redo();
    }else */if(keyEvent->key() == Qt::Key_Delete){
        CGLayoutManager::getInstance()->getController()->deleteItem();
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
    QGraphicsScene::keyPressEvent(keyEvent);

}

CGLayoutView* CGLayout::getView(){
    return this->view;
}

void CGLayout::setView(CGLayoutView* view){
    this->view = view;
}

void CGLayout::load(){
    //    foreach(CGOperatorModel* ope, model->getOperatorModelList()){
    //        this->addItem(ope->getView());
    //    }
    //    foreach(CGLayoutModel* composition, model->getCompositionModelList()){
    //        this->addItem(composition->getOuterView());
    //    }
    //    if(model->getParentCompositionModel()!=NULL){
    //        this->getRect()->setBrush(Qt::gray);
    //        this->setBackgroundBrush(Qt::gray);
    //    }else{
    //        this->getRect()->setBrush(Qt::black);
    //        this->setBackgroundBrush(Qt::gray);
    //    }
}

QMenu* CGLayout::getCommentShapeMenu(){
    return NULL;
}

QMenu* CGLayout::getCommentTextMenu(){
    return NULL;

}

QMenu* CGLayout::getOperatorMenu(){
    return operatorMenu;

}

QMenu* CGLayout::getConnectorMenu(){
    return connectorMenu;

}

void CGLayout::setTextColor(const QColor &color)
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

void CGLayout::setItemColor(const QColor &color)
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

void CGLayout::activate(QGraphicsItem *item){
    if(item->scene()==0){
        this->addItem(item);
    }
    if (item->type() == CGLayoutItem::Type) {
        CGLayoutItem *ctrl =
                qgraphicsitem_cast<CGLayoutItem *>(item);
        ctrl->setLayout(this);
    }
}

void CGLayout::unactivate(QGraphicsItem *item){
    this->removeItem(item);
    if (item->type() == CGLayoutItem::Type) {
        CGLayoutItem *ctrl =
                qgraphicsitem_cast<CGLayoutItem *>(item);
        ctrl->setLayout(0);
    }
}

void CGLayout::setFont(const QFont &font)
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

bool CGLayout::isItemChange(int type)
{
    foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}

int CGLayout::getMode(){
    return sceneMode;
}

void CGLayout::setMode(Mode mode){
    this->sceneMode = mode;
    this->reloadCursor();
}

QString CGLayout::getResolution(){
    return this->resolution;
}

void CGLayout::setResolution(QString s){
    //40880049: ControllerView save/load position
    rect->setRect(-10000, -5000, 20000, 10000);
    rect->update();
//    if(s.compare("") != 0){
//        this->resolution = s;
//        QList<QString> l = s.split(" x ");
//        int width = l.value(0).toInt();
//        int height = l.value(1).toInt();
//        rect->setRect(-width/2, -height/2, width, height);
//        rect->update();
//    }
}

bool CGLayout::getIsPopup(){
    return isPopup;
}

void CGLayout::setIsPopup(bool isPopup){
    this->isPopup = isPopup;
}
