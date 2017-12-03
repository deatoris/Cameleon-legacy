/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGConnectConnector.h"
#include "CGLayoutManager.h"
#include<CUtilitySTL.h>

CGConnectConnector::CGConnectConnector()
{
    startItem = NULL;
    endItem = NULL;
    multi = NULL;
    type = "CONNECT";
    control = NULL;
    connector = NULL;
}

void CGConnectConnector::exec()
{
    if(control != NULL){
        controlconnector->setIsControlConnected(true);
        connector->setIsControlConnected(true);
        connector->connect(controlconnector);
        controlconnector->connect(connector);
        controlconnector->update();
        connector->update();
        CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
       // CGLayoutManager::getInstance()->getCurrentLayout()->update();
    }else{
        if(endItem != NULL){ //connection standard
            startItem->connect(endItem);
        }else if(endItem == NULL && multi == NULL){//connection from inner multi
            if(startItem->getSide().compare("IN") == 0){
                CGMultiConnector* multic = CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->getParentView()->addConnector("OUTIN");
                multic->getInnerConnector()->connect(startItem);
                multic->getInnerConnector()->freePosition();

                double r = (multic->getInnerConnector()->getRayon())/2;
                double teta = acos(multic->getInnerConnector()->pos().x()/r)*(SCD::sgn(multic->getInnerConnector()->pos().y()));

                double x1 = cos(teta)*((multic->getOuterConnector()->getRayon()-20)+20)/2;
                double y1 = sin(teta)*((multic->getOuterConnector()->getRayon()-20)+20)/2;
                multic->getOuterConnector()->setPos(x1,y1);

                foreach(CGMultiConnector*c,CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->getParentView()->getMultiConnectorsList()){
                    c->getOuterConnector()->rePosition();
                    c->getInnerConnector()->rePosition();
                }
            }else{
                CGMultiConnector* multic = CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->getParentView()->addConnector("INOUT");
                multic->getInnerConnector()->connect(startItem);
                multic->getInnerConnector()->freePosition();

                double r = (multic->getInnerConnector()->getRayon())/2;
                double teta = acos(multic->getInnerConnector()->pos().x()/r)*(SCD::sgn(multic->getInnerConnector()->pos().y()));

                double x1 = cos(teta)*((multic->getOuterConnector()->getRayon()-20)+20)/2;
                double y1 = sin(teta)*((multic->getOuterConnector()->getRayon()-20)+20)/2;
                multic->getOuterConnector()->setPos(x1,y1);

                foreach(CGMultiConnector*c,CGCompositionManager::getInstance()->getCurrentCompositionModel()->getInnerView()->getParentView()->getMultiConnectorsList()){
                    c->getOuterConnector()->rePosition();
                    c->getInnerConnector()->rePosition();
                }
            }
        }else{//connection from outer multi
            if(startItem->getSide().compare("OUT") == 0){
                CGMultiConnector* multic = multi->getParentView()->addConnector("OUTIN");
                multic->getOuterConnector()->connect(startItem);
                multic->getOuterConnector()->freePosition();

                double r = (multic->getOuterConnector()->getRayon())/2;
                double teta = acos(multic->getOuterConnector()->pos().x()/r)*(SCD::sgn(multic->getOuterConnector()->pos().y()));

                double x1 = cos(teta)*((multic->getInnerConnector()->getRayon()-20)+20)/2;
                double y1 = sin(teta)*((multic->getInnerConnector()->getRayon()-20)+20)/2;
                multic->getInnerConnector()->setPos(x1,y1);

                foreach(CGMultiConnector*c,multi->getParentView()->getMultiConnectorsList()){
                    c->getOuterConnector()->rePosition();
                    c->getInnerConnector()->rePosition();
                }


            }else{
                CGMultiConnector* multic = multi->getParentView()->addConnector("INOUT");
                multic->getOuterConnector()->connect(startItem);
                multic->getOuterConnector()->freePosition();

                double r = (multic->getOuterConnector()->getRayon())/2;
                double teta = acos(multic->getOuterConnector()->pos().x()/r)*(SCD::sgn(multic->getOuterConnector()->pos().y()));

                double x1 = cos(teta)*((multic->getInnerConnector()->getRayon()-20)+20)/2;
                double y1 = sin(teta)*((multic->getInnerConnector()->getRayon()-20)+20)/2;
                multic->getInnerConnector()->setPos(x1,y1);

                foreach(CGMultiConnector*c,multi->getParentView()->getMultiConnectorsList()){
                    c->getOuterConnector()->rePosition();
                    c->getInnerConnector()->rePosition();
                }
            }
        }
        CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
        if(CGLayoutManager::getInstance()->getCurrentLayout()!=0) CGLayoutManager::getInstance()->getCurrentLayout()->update();
    }
}

void CGConnectConnector::unexec()
{
    if(control != NULL){
        controlconnector->disconnect();
        connector->setIsControlConnected(false);
        controlconnector->setIsControlConnected(false);
        controlconnector->update();
        connector->update();
        CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
       if(CGLayoutManager::getInstance()->getCurrentLayout()) CGLayoutManager::getInstance()->getCurrentLayout()->update();
    }else if(endItem != NULL){
        startItem->disconnect();
        endItem->disconnect();
    }else{
        startItem->disconnect();
    }
}

void CGConnectConnector::setStartItem(CGConnector* startItem){
    this->startItem = startItem;
}

void CGConnectConnector::setEndItem(CGConnector* endItem){
    this->endItem = endItem;
}

void CGConnectConnector::setColor(QColor color)
{
    this->color = color;
}

void CGConnectConnector::setMulti(CGMulti* multi){
    this->multi = multi;
}


void CGConnectConnector::setControl(CControl* control){
    this->control = control;
}

void CGConnectConnector::setConnector(CGConnector* connector){
    this->connector = connector;
}

void CGConnectConnector::setControlConnector(CGConnector* controlconnector){
    this->controlconnector = controlconnector;
}
