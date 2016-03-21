/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGDeleteOperator.h"
#include "../CGComposer/CGCompositionManager.h"
#include "../CGComposer/CGCompositionItem.h"
#include "CGProject.h"

CGDeleteOperator::CGDeleteOperator()
{
    type = "DELETE";
    move = false;
}

void CGDeleteOperator::exec(){
    scene->unselectAll();
    if(item->type() == CGMulti::Type && move == false){
        CGMulti* m = qgraphicsitem_cast<CGMulti *>(item);
        foreach(CGCompositionModel* multi, m->getModel()->getCompositionModelList()){
            multi->getOuterView()->disconnect();
            CGDeleteOperator* del= new CGDeleteOperator();
            del->setScene(scene);
            del->setItem(multi->getOuterView());
            del->doCommand();
        }
        foreach(CGOperatorModel* ope, m->getModel()->getOperatorModelList()){
            ope->getView()->disconnect();
            CGDeleteOperator* del= new CGDeleteOperator();
            del->setScene(scene);
            del->setItem(ope->getView());
            del->doCommand();
        }
    }else if(item->type() == CGOperator::Type){
        CGOperator* m = qgraphicsitem_cast<CGOperator *>(item);
        m->disconnect();
    }else if(item->type() == CGMulti::Type){
        CGMulti* m = qgraphicsitem_cast<CGMulti *>(item);
        CGProject::getInstance()->deleteComposition(m->getModel());
    }
    scene->getModel()->unactivate(item);
}

void CGDeleteOperator::setMove(bool move){
    this->move = move;
}

void CGDeleteOperator::unexec(){
    scene->getModel()->activate(item);
    if(item->type() == CGMulti::Type){
        CGMulti* m = qgraphicsitem_cast<CGMulti *>(item);
        CGProject::getInstance()->addComposition(m->getModel());
    }
}

void CGDeleteOperator::setItem(QGraphicsItem* item){
    this->item = item;
}

void CGDeleteOperator::setScene(CGComposition* scene){
    this->scene = scene;
}

