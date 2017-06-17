/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGChangeView.h"
#include "../CGComposer/CGComposition.h"
#include "../CGComposer/CGCompositionManager.h"
#include "../CGComposer/CGCompositionModel.h"
#include "CGLayout.h"
#include "CGLayoutManager.h"
#include "CGProject.h"

CGChangeView::CGChangeView()
{
    type = "CHANGEVIEW";
    compositionModel = NULL;
    layout = NULL;
}

void CGChangeView::exec()
{
    if(layout == NULL){
        previousModel = CGCompositionManager::getInstance()->getCurrentCompositionModel();
        CGCompositionManager::getInstance()->showSceneFromModel(compositionModel);
        CGProject::getInstance()->switchToModel(compositionModel);
        if(previousModel->getParentCompositionModel() == compositionModel){
            previousModel->getOuterView()->setSelected(true);
            previousModel->getOuterView()->setIsUp(true);
            compositionModel->getSceneView()->centerOn(previousModel->getOuterView()->scenePos());
        }
    }else{
        previousLayout = CGLayoutManager::getInstance()->getCurrentLayout();
        CGLayoutManager::getInstance()->showLayout(layout->getId());
        CGProject::getInstance()->switchToLayout(layout);
    }
}

void CGChangeView::unexec()
{
    if(layout == NULL){
        CGCompositionManager::getInstance()->showSceneFromModel(previousModel);
    }else{
        CGLayoutManager::getInstance()->showLayout(previousLayout->getId());
    }
}

void CGChangeView::setCompositionModel(CGCompositionModel* compositionModel){
    this->compositionModel = compositionModel;
}

void CGChangeView::setLayout(CGLayout* layout){
    this->layout = layout;
}
