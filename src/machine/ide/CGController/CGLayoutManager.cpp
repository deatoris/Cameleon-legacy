/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGLayoutManager.h"
#include "CGProject.h"
#include "CLogger.h"
#include "CGCompositionManager.h"
#include "../CGCommand/CGDeleteControl.h"
#include "../CGCommand/CGDisconnectConnector.h"
CGLayoutManager* CGLayoutManager::layoutManagerInstance = NULL;
CGLayoutManager::CGLayoutManager()
{
    currentLayout = 0;
    menuExtension = 0;
}

void CGLayoutManager::showLayout(int id){
    //    if(!this->controller->getStandaloneMode()){
    CGLayout* prev = 0;
    if(currentLayout != 0){
        currentLayout->getView()->hide();
        prev = currentLayout;
        layout->removeWidget(currentLayout->getView());
    }
    if(id != -1){
        QList<QGraphicsItem*> persList;
        if(prev != 0){
            foreach(QGraphicsItem*item, prev->items()){
                if(CGLayoutItem::Type == item->type()){
                    CGLayoutItem* lay = qgraphicsitem_cast<CGLayoutItem *>(item);
                    if(lay->getIsPersistant()) persList.append(lay);
                }
                if(CGShape::Type == item->type()){
                    CGShape* lay = qgraphicsitem_cast<CGShape *>(item);
                    if(lay->getPersistent()) persList.append(lay);
                }
                if(CGText::Type == item->type()){
                    CGText* lay = qgraphicsitem_cast<CGText *>(item);
                    if(lay->getPersistent()) persList.append(lay);
                }
            }
        }

        currentLayout = mapNameLayout.value(id);
        if(currentLayout!=0) layout->addWidget(currentLayout->getView());
        if(currentLayout!=0) CGLayoutManager::getInstance()->getController()->setResolution(currentLayout->getResolution());
        if(currentLayout!=0) CGLayoutManager::getInstance()->getController()->setIsPopup(currentLayout->getIsPopup());
        if(currentLayout!=0) currentLayout->getView()->show();
        if(currentLayout!=0) currentLayout->getView()->clearFocus();

        foreach(QGraphicsItem* item, persList){
            currentLayout->addItem(item);
        }
    }else{
        currentLayout = 0;
    }
    //    }else{
    //        CGLayout* prev = 0;
    //        if(mapNameLayout.value(id) !=0 && mapNameLayout.value(id)->getIsPopup()){
    //            CGLayout* dial = mapNameLayout.value(id);

    //            QGraphicsItemGroup* group = new QGraphicsItemGroup(0,dial);


    //            foreach(QGraphicsItem* item, dial->items()){
    //                if(item->type() == CGLayoutItem::Type){
    //                    group->addToGroup(item);
    //                }
    //                if(item->type() == CGShape::Type){
    //                    group->addToGroup(item);
    //                }
    //                if(item->type() == CGText::Type){
    //                    group->addToGroup(item);
    //                }
    //            }

    //            QRectF r = group->boundingRect();

    //            QRectF r2;
    //            r2.setTop(200);
    //            r2.setLeft(200);
    //            r2.setWidth(r.width());
    //            r2.setHeight(r.height());

    //            group->translate(-r.x()-r.width()/2,-r.y()-r.height()/2);
    //            dial->setSceneRect(-r.width()/2,-r.height()/2,r.width(),r.height());
    //            dial->getRect()->hide();
    //            foreach(QGraphicsItem* item, dial->items()){
    //                item->setFlag(QGraphicsItem::ItemIsMovable, false);
    //                item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    //                item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    //            }
    //            dial->getView()->centerOn(0,0);

    //            dial->destroyItemGroup(group);

    //            if(dial->getView()->parentWidget()==currentLayout->getView()->parentWidget()){
    //                QWidget* w = new QWidget();
    //                QHBoxLayout* lay = new QHBoxLayout();
    //                w->setLayout(lay);

    //                w->setGeometry(r2.toRect());
    //                w->setFixedHeight(r2.toRect().height());
    //                w->setFixedWidth(r2.toRect().width());

    //                lay->addWidget(dial->getView());
    //                dial->getView()->show();
    //                w->setWindowFlags(Qt::Dialog);
    //                w->show();
    //                dial->getView()->clearFocus();
    //            }else{
    //                dial->getView()->parentWidget()->show();
    //            }
    //        }else{
    //            if(id != -1){
    //                if(currentLayout != 0 && mapNameLayout.value(id) !=0 && !mapNameLayout.value(id)->getIsPopup()){
    //                    currentLayout->getView()->hide();
    //                    prev = currentLayout;
    //                    layout->removeWidget(currentLayout->getView());
    //                }
    //                QList<QGraphicsItem*> persList;
    //                if(prev != 0){
    //                    foreach(QGraphicsItem*item, prev->items()){
    //                        if(CGLayoutItem::Type == item->type()){
    //                            CGLayoutItem* lay = qgraphicsitem_cast<CGLayoutItem *>(item);
    //                            if(lay->getIsPersistant()) persList.append(lay);
    //                        }
    //                        if(CGShape::Type == item->type()){
    //                            CGShape* lay = qgraphicsitem_cast<CGShape *>(item);
    //                            if(lay->getPersistent()) persList.append(lay);
    //                        }
    //                        if(CGText::Type == item->type()){
    //                            CGText* lay = qgraphicsitem_cast<CGText *>(item);
    //                            if(lay->getPersistent()) persList.append(lay);
    //                        }
    //                    }
    //                }

    //                currentLayout = mapNameLayout.value(id);
    //                if(currentLayout!=0) CGLayoutManager::getInstance()->getController()->setIsPopup(currentLayout->getIsPopup());
    //                if(currentLayout!=0){
    //                    if(prev != 0 && !currentLayout->getIsPopup()){
    //                        foreach(QGraphicsItem*item, persList){
    //                            currentLayout->addItem(item);
    //                        }
    //                    }
    //                    QGraphicsItemGroup* group = new QGraphicsItemGroup(0,currentLayout);


    //                    foreach(QGraphicsItem* item, currentLayout->items()){
    //                        if(item->type() == CGLayoutItem::Type){
    //                            group->addToGroup(item);
    //                        }
    //                        if(item->type() == CGShape::Type){
    //                            group->addToGroup(item);
    //                        }
    //                        if(item->type() == CGText::Type){
    //                            group->addToGroup(item);
    //                        }
    //                    }

    //                    QRectF r = group->boundingRect();

    //                    QRectF r2;
    //                    r2.setTop(200);
    //                    r2.setLeft(200);
    //                    r2.setWidth(r.width());
    //                    r2.setHeight(r.height());

    //                    group->translate(-r.x()-r.width()/2,-r.y()-r.height()/2);
    //                    currentLayout->setSceneRect(-r.width()/2,-r.height()/2,r.width(),r.height());
    //                    currentLayout->getRect()->hide();
    //                    foreach(QGraphicsItem* item, currentLayout->items()){
    //                        item->setFlag(QGraphicsItem::ItemIsMovable, false);
    //                        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    //                        item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);
    //                    }
    //                    currentLayout->getView()->centerOn(0,0);

    //                    currentLayout->destroyItemGroup(group);

    //                    layout->addWidget(currentLayout->getView());
    //                    currentLayout->getView()->show();
    //                    currentLayout->getView()->clearFocus();

    //                }

    //            }else{
    //                currentLayout = 0;
    //            }
    //        }
    //    }
}

QList<QString> CGLayoutManager::getManagedResolutions(){
    QList<QString> list;
    list.append("800 x 600");
    list.append("1024 x 768");
    list.append("1280 x 1024");
    list.append("1920 x 1080");
    return list;
}

CGLayout* CGLayoutManager::getLayout(){
    CGLayout* layout = new CGLayout(0,0,0);
    mapNameLayout.insert(layout->getId(),layout);
    layoutL.append(layout);
    return layout;
}

void CGLayoutManager::addLayout(CGLayout* layout){
    layoutL.append(layout);
    mapNameLayout.insert(layout->getId(),layout);
}

void CGLayoutManager::delLayout(CGLayout* layout){
    foreach(QGraphicsItem* item, layout->items()){
        if(item->type() == CGLayoutItem::Type){
            CGLayoutItem* layout = qgraphicsitem_cast<CGLayoutItem *>(item);
            foreach(CGLayoutItemConnector* connector, layout->getConnectorsL()){
                if(connector->isConnected()){
                    CGDisconnectConnector* disconnect = new CGDisconnectConnector();
                    disconnect->setControl(layout->getControl());
                    disconnect->setControlConnector(connector);
                    disconnect->setConnector(connector->getConnectedConnector());
                    disconnect->doCommand();
                }
            }
        }
    }
    foreach(QGraphicsItem* item, layout->items()){
        if(item->type() == CGLayoutItem::Type){
            CGDeleteControl* del= new CGDeleteControl();
            del->setItem(item);
            del->setScene(layout);
            del->doCommand();
        }
    }
    mapNameLayout.remove(layout->getId());
    layoutL.removeOne(layout);
    if(currentLayout!=0){
        if(currentLayout->getId() == layout->getId()){
            if(mapNameLayout.count() != 0) {
                showLayout(mapNameLayout.keys().first());
            }else{
                showLayout(-1);
            }
        }
    }
}

CGLayout* CGLayoutManager::getLayout(int id){
    if(mapNameLayout.contains(id)) return mapNameLayout.value(id);
    CGLayout* layout = new CGLayout(0,0,0);
    mapNameLayout.insert(layout->getId(),layout);
    layoutL.append(layout);
    return layout;
}

void CGLayoutManager::renameLayout(int , QString ){

}

QMenu* CGLayoutManager::getControllerMenu(){
    return controller->getControllerMenu();
}

QMenu* CGLayoutManager::getControlMenu(){
    return controller->getControlMenu();
}

void CGLayoutManager::setControlExtentionMenu(QMenu* menu){
    if(menuExtension != 0){
        controller->getControlMenu()->removeAction(menuExtension->menuAction());
    }
    if(menu != 0){
        QMenu* ctlMenu = controller->getControlMenu();
        ctlMenu->addMenu(menu);
    }
    menuExtension = menu;
}

QMenu* CGLayoutManager::getConnectorMenu(){
    return controller->getConnectorMenu();
}

CGController* CGLayoutManager::getController(){
    return this->controller;
}

void CGLayoutManager::showConnected(CGConnector* connector){
    //    CGLayout*c = (CGLayout*)connector->parentItem()->scene();
    QGraphicsScene* scene = connector->parentItem()->scene();
    if(CGComposition* compo = dynamic_cast<CGComposition*>(scene)){
        CGCompositionManager::getInstance()->showSceneFromModel(compo->getModel());
        connector->setIsHighLight(true);
        connector->parentItem()->setSelected(true);
        CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->centerOn(connector->parentItem());
    }else if(CGLayout* l = dynamic_cast<CGLayout*>(scene)){
        CGLayoutManager::getInstance()->showLayout(l->getId());
        connector->setIsHighLight(true);
        connector->parentItem()->setSelected(true);
        CGLayoutManager::getInstance()->getCurrentLayout()->getView()->centerOn(connector->parentItem());
    }

}

void CGLayoutManager::init(CGController* controller){
    this->controller = controller;

    layout = new QHBoxLayout;
    controllerWidgetScene = new QWidget;
    layout->setMargin(0);
    layout->setSpacing(0);
    controllerWidgetScene->setLayout(layout);
    controllerWidgetScene->setAcceptDrops(true);
    controller->setCentralWidget(controllerWidgetScene);
}

CGLayout* CGLayoutManager::getCurrentLayout(){
    return currentLayout;
}

QList<CGLayout*> CGLayoutManager::getLayoutL(){
    return layoutL;
}

void CGLayoutManager::clear(){
    CLogger::getInstance()->log("INSTANCE", CLogger::WARNING, "CGLayoutManager::clear() memory leaks here!");
    foreach(CGLayout* layout, layoutL){
        delLayout(layout);
        CGProject::getInstance()->deleteLayout(layout);
    }
    layoutL.clear();
    mapNameLayout.clear();
    CGLayout::lid = 0;
    currentLayout = 0;
}

CGLayoutItem* CGLayoutManager::getItemById(int id){
    foreach(CGLayout* layout, layoutL){
        QList<QGraphicsItem*> list = layout->items();
        foreach(QGraphicsItem*item, list){
            if(item->type() == CGLayoutItem::Type){
                CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                if(ctrl->getControl()->getId() == id) return ctrl;
            }
        }
    }
    foreach(QGraphicsItem* item,CGCompositionManager::getInstance()->getParentCompositionModel()->getScene()->items()){
        if(item->type() == CGLayoutItem::Type){
            CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
            if(ctrl->getControl()->getId() == id) return ctrl;
        }
    }
    foreach(CGCompositionModel* m,CGCompositionManager::getInstance()->getCompositionList(CGCompositionManager::getInstance()->getParentCompositionModel())){
        foreach(QGraphicsItem* item,m->getScene()->items()){
            if(item->type() == CGLayoutItem::Type){
                CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                if(ctrl->getControl()->getId() == id) return ctrl;
            }
        }
    }

    return NULL;
}

CGLayoutManager* CGLayoutManager::getInstance(){
    if(NULL != layoutManagerInstance){
        return layoutManagerInstance;
    }else{
        layoutManagerInstance = new CGLayoutManager();
        return layoutManagerInstance;
    }
}
