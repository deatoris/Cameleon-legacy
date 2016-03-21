/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCreateControl.h"
#include "CGCompositionItem.h"
#include "CGCompositionManager.h"
#include "CGLayoutItem.h"
#include "CGLayoutManager.h"
#include "CGLayoutFactory.h"
#include "CLogger.h"
#include "CControlRessource.h"
#include "CGProject.h"
#include "CGInstance.h"
CGCreateControl::CGCreateControl()
{
    type = "CREATELAYOUT";
    item = NULL;
    path = "";
    composition = 0;
}

void CGCreateControl::setPath(QString path){
    this->path = path;
}

void CGCreateControl::setScene(CGComposition* composition){
    this->composition = composition;
}

void CGCreateControl::exec(){
    if(item == NULL){
        if(typeItem == CGLayoutItem::Type && path.compare("")==0 && composition != 0){
            CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name, composition);
            if(layoutItem != NULL){
                layoutItem->setSelected(false);
                eventScenePos.setX(eventScenePos.x()-layoutItem->getRect().width()/2);
                eventScenePos.setY(eventScenePos.y()-layoutItem->getRect().height()/2);
                layoutItem->setPos(eventScenePos);
                item = layoutItem;
                item->setSelected(true);
                composition->unselectAll();
                composition->getModel()->activate(item);
                item->setSelected(true);
                CGCompositionManager::getInstance()->getComposer()->toFront();
            }else{
                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create object named "+name);
            }
            item->update();
        }else if(typeItem == CGLayoutItem::Type && path.compare("")!=0 && composition != 0){
            CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name, composition);
            if(layoutItem != NULL){
                layoutItem->setSelected(false);
                eventScenePos.setX(eventScenePos.x()-layoutItem->getRect().width()/2);
                eventScenePos.setY(eventScenePos.y()-layoutItem->getRect().height()/2);
                layoutItem->setPos(eventScenePos);
                item = layoutItem;

                if(name.compare("RessourceProject")==0)layoutItem->getControl()->fromString(CGProject::getInstance()->makeRelative(path).toStdString());
                if(name.compare("Ressource")==0)layoutItem->getControl()->fromString(CGInstance::getInstance()->makeRelative(path).toStdString());
//                CGProject::getInstance()->addRessource(path);
                item->setSelected(true);
            }else{
                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create object named "+name);
            }
            composition->unselectAll();
            composition->getModel()->activate(item);
            item->setSelected(true);
            CGCompositionManager::getInstance()->getComposer()->toFront();
            item->update();
        }else if(typeItem == CGLayoutItem::Type && path.compare("")!=0){
            CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name, CGLayoutManager::getInstance()->getCurrentLayout());
            if(layoutItem != NULL){
                layoutItem->setSelected(false);
                eventScenePos.setX(eventScenePos.x()-layoutItem->getRect().width()/2);
                eventScenePos.setY(eventScenePos.y()-layoutItem->getRect().height()/2);
                layoutItem->setPos(eventScenePos);
                item = layoutItem;

                if(name.compare("RessourceProject")==0)layoutItem->getControl()->fromString(CGProject::getInstance()->makeRelative(path).toStdString());
                if(name.compare("Ressource")==0)layoutItem->getControl()->fromString(CGInstance::getInstance()->makeRelative(path).toStdString());

                item->setSelected(true);
                CGLayoutManager::getInstance()->getController()->toFront();
            }else{
                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create object named "+name);
            }
            CGLayoutManager::getInstance()->getCurrentLayout()->unselectAll();
            CGLayoutManager::getInstance()->getCurrentLayout()->activate(item);
            item->setSelected(true);
            CGLayoutManager::getInstance()->getController()->toFront();
            item->update();
        }else if(typeItem == CGLayoutItem::Type){
            CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name, CGLayoutManager::getInstance()->getCurrentLayout());
            if(layoutItem != NULL){
                layoutItem->setSelected(false);
                eventScenePos.setX(eventScenePos.x()-layoutItem->getRect().width()/2);
                eventScenePos.setY(eventScenePos.y()-layoutItem->getRect().height()/2);
                layoutItem->setPos(eventScenePos);
                item = layoutItem;
                item->setSelected(true);
                CGLayoutManager::getInstance()->getController()->toFront();
            }else{
                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create object named "+name);
            }
            CGLayoutManager::getInstance()->getCurrentLayout()->unselectAll();
            CGLayoutManager::getInstance()->getCurrentLayout()->activate(item);
            item->setSelected(true);
            CGLayoutManager::getInstance()->getController()->toFront();
        }else if(typeItem == CGShape::Type){
            CGShape* shape = new CGShape(0,0,CGLayoutManager::getInstance()->getCurrentLayout());
            shape->setPos(eventScenePos);
            item = shape;
            CGLayoutManager::getInstance()->getCurrentLayout()->unselectAll();
            CGLayoutManager::getInstance()->getCurrentLayout()->activate(item);
            item->setSelected(true);
            CGLayoutManager::getInstance()->getController()->toFront();
        }else if(typeItem == CGText::Type){
            CGText* text = new CGText();
            text->setFont(CGLayoutManager::getInstance()->getCurrentLayout()->font());
            text->setTextInteractionFlags(Qt::TextEditorInteraction);
            text->setZValue(1000.0);
            QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                             CGLayoutManager::getInstance()->getCurrentLayout(), SLOT(editorLostFocus(CGText*)));
            QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                             CGLayoutManager::getInstance()->getCurrentLayout(), SIGNAL(itemSelected(QGraphicsItem*)));
            text->setDefaultTextColor(Qt::white);
            text->setPos(eventScenePos);
            item = text;
            CGLayoutManager::getInstance()->getCurrentLayout()->unselectAll();
            CGLayoutManager::getInstance()->getCurrentLayout()->activate(item);
            item->setSelected(true);
            CGLayoutManager::getInstance()->getController()->toFront();
        }
    }else{
        if(composition != 0){
            composition->unselectAll();
            composition->getModel()->activate(item);
        }
    }
}

void CGCreateControl::unexec(){
    CGLayoutManager::getInstance()->getCurrentLayout()->unactivate(item);
}


void CGCreateControl::setEventPos(QPointF eventScenePos){
    this->eventScenePos = eventScenePos;
}

void CGCreateControl::setItem(QGraphicsItem* item){
    this->item = item;
}

void CGCreateControl::setName(QString name){
    this->name = name;
}

void CGCreateControl::setType(int type){
    this->typeItem = type;
}
