/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCreateOperator.h"
#include "../CGComposer/CGCompositionManager.h"
#include "../CGComposer/CGCompositionFactory.h"
#include "CGProject.h"
#include "CLogger.h"

CGCreateOperator::CGCreateOperator()
{
    item = NULL;
    type = "CREATE";
    layout = NULL;
    scene = NULL;
    isPattern = false;
}

void CGCreateOperator::exec()
{
    if(item == NULL){
        if(typeItem == CGOperator::Type){
            CGOperatorModel* model = scene->getModel()->addOperatorByName(operatorName);
            if(model != NULL){
                CGOperator* view = model->getView();
                model->getView()->setPos(eventScenePos);
                model->setPos(eventScenePos);
                item = view;
            }else{
                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create object named "+operatorName);
            }
            if(scene!=NULL && item!=NULL){
                scene->getModel()->activate(item);
//                scene->addItem(item);
            }
            if(CGCompositionManager::getInstance()->getCurrentCompositionModel()->getParentCompositionModel()!=NULL)CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->processInnerRay(true);
            CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
        }else if(isPattern){
            if(layout==NULL) {
                CGCompositionModel* mod = CGCompositionFactory::getInstance()->getCompositionByPatternName(operatorName, scene->getModel());
                mod->getOuterView()->setPos(eventScenePos);
                mod->getInnerView()->processInnerRay(true);
                mod->getSceneView()->centerOn(mod->getInnerView()->scenePos());
            }else{
                CGCompositionModel* mod = CGCompositionFactory::getInstance()->getCompositionByPatternName(operatorName, scene->getModel());
                QRectF r = CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->sceneRect();
                int x = r.x()+r.width()/2;
                int y = r.y()+r.height()/2;
                mod->getOuterView()->setPos(x,y);
                mod->getInnerView()->processInnerRay(true);
                mod->getSceneView()->centerOn(mod->getInnerView()->scenePos());
            }
            CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
        }else if(typeItem == CGMulti::Type){
            CGCompositionModel* compositionModel = new CGCompositionModel(scene->getModel());
            compositionModel->setName(operatorName);
            CGMulti* multi = compositionModel->getOuterView();
            multi->setSelected(false);
            multi->setPos(eventScenePos);
            item = multi;
            multi->setIsCompatibleMode(false);
            multi->setSelected(false);
            CGProject::getInstance()->addComposition(compositionModel);
            CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
            if(scene!=NULL && item!=NULL){
                scene->getModel()->activate(item);
//                scene->addItem(item);
            }
            compositionModel->getInnerView()->processInnerRay(true);
        }else if(typeItem == CGShape::Type){
            if(layout==NULL) {
                CGShape* shape = new CGShape(0,0,scene);
//                scene->addItem(shape);
                shape->setRect(rect);
                item = shape;
            }else{
                CGShape* shape = new CGShape(0,0,layout);
//                layout->addItem(shape);
                shape->setRect(rect);
                item = shape;
            }
        }else if(typeItem == CGText::Type){
            CGText* text = new CGText();
            if(layout==NULL) {
                text->setFont(scene->font());
                text->setTextInteractionFlags(Qt::TextEditorInteraction);
                text->setZValue(1000.0);

                QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                 scene, SLOT(editorLostFocus(CGText*)));
                QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                 scene, SIGNAL(itemSelected(QGraphicsItem*)));
                scene->addItem(text);
                CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
//                if(scene!=NULL && item!=NULL){
//                    scene->getModel()->activate(item);
////                    scene->addItem(item);
//                }
            }else{
                text->setFont(layout->font());
                text->setTextInteractionFlags(Qt::TextEditorInteraction);
                text->setZValue(1000.0);

                QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                 layout, SLOT(editorLostFocus(CGText*)));
                QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                 layout, SIGNAL(itemSelected(QGraphicsItem*)));
                layout->addItem(text);
                CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
            }
            text->setDefaultTextColor(Qt::white);
            text->setPos(eventScenePos);
            item = text;
            CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
            item->setSelected(true);
            text->setFocus();
            if(scene!=NULL && item!=NULL){
                scene->getModel()->activate(item);
//                scene->addItem(item);
            }
        }
        CGCompositionManager::getInstance()->getComposer()->toFront();
        if(item!=NULL)item->setSelected(true);
    }else{
        if(scene!=NULL){
            if(scene!=NULL){
//                item->scene()->removeItem(item);
                scene->unselectAll();
                scene->getModel()->activate(item);
//                if(scene->getModel()->getInnerView()!=0)scene->getModel()->getInnerView()->processInnerRay(false);
            }else{
                layout->removeItem(item);
            }
        }
        if(typeItem == CGMulti::Type){
            CGMulti* multiItem = qgraphicsitem_cast<CGMulti *>(item);
            CGProject::getInstance()->addComposition(multiItem->getModel());
            multiItem->setIsCompatibleMode(false);
            multiItem->setSelected(false);
            multiItem->getModel()->getInnerView()->processInnerRay(true);
            multiItem->getModel()->getSceneView()->centerOn(multiItem->getModel()->getInnerView()->scenePos());
        }
        CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
    }
}

void CGCreateOperator::setIsPattern(bool isPattern){
    this->isPattern = isPattern;
}

void CGCreateOperator::setRect(QRectF rect){
    this->rect = rect;
}

void CGCreateOperator::unexec()
{
    if(typeItem == CGMulti::Type){
        CGMulti* multiItem = qgraphicsitem_cast<CGMulti *>(item);
        CGProject::getInstance()->deleteComposition(multiItem->getModel());
    }
    if(scene!=NULL){
        scene->getModel()->unactivate(item);
    }else{
        if(item!=NULL) layout->removeItem(item);
    }
}

void CGCreateOperator::setOperatorName(QString operatorName)
{
    this->operatorName = operatorName;
}

void CGCreateOperator::setEventPos(QPointF eventScenePos)
{
    this->eventScenePos = eventScenePos;
}

void CGCreateOperator::setType(int typeItem){
    this->typeItem = typeItem;
}

void CGCreateOperator::setScene(CGComposition* scene){
    this->scene = scene;
}

QGraphicsItem* CGCreateOperator::getItem(){
    return item;
}

void CGCreateOperator::setItem(QGraphicsItem* item){
    this->item = item;
}

void CGCreateOperator::setLayout(CGLayout *layout){
    this->layout = layout;
}
