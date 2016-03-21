/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGDeleteControl.h"
#include "CGLayoutManager.h"
#include "CGCompositionManager.h"

CGDeleteControl::CGDeleteControl()
    :scene(0)
{
    type = "DELETECONTROL";
    scene = 0;
}

void CGDeleteControl::exec(){
    CGComposition* comp = 0;
    foreach (QGraphicsItem *item, scene->items()) {
        item->setSelected(false);
        if(scene->mouseGrabberItem() == item) item->ungrabMouse();
        if(item->isVisible()) item->grabKeyboard();
        if(item->isVisible()) item->ungrabKeyboard();
        if(item->isVisible()) item->clearFocus();
    }
    scene->setFocusItem(0);

    if(scene!=0) comp = dynamic_cast<CGComposition*>(scene);
    if(comp==0)CGLayoutManager::getInstance()->getCurrentLayout()->unactivate(item);
    if(comp!=0)comp->getModel()->unactivate(item);
    item->hide();
    item->setEnabled(false);
}

void CGDeleteControl::unexec(){
    CGComposition* comp = dynamic_cast<CGComposition*>(scene);
    if(comp==0)CGLayoutManager::getInstance()->getCurrentLayout()->activate(item);;
    if(comp!=0)comp->getModel()->activate(item);
}

void CGDeleteControl::setScene(QGraphicsScene* scene){
    this->scene = scene;
}


void CGDeleteControl::setItem(QGraphicsItem* item){
    this->item = item;
}
