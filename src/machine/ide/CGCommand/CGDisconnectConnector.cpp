/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGDisconnectConnector.h"
#include "../CGComposer/CGCompositionManager.h"
#include "CGLayoutManager.h"

CGDisconnectConnector::CGDisconnectConnector()
{
    type = "DISCONNECT";
    control = NULL;
}

void CGDisconnectConnector::exec(){
    if(control != NULL){
        if(connector->isConnected() && connector->getConnectedConnector()->type() == CGMultiConnector::Type){
            CGMultiConnector *m = qgraphicsitem_cast<CGMultiConnector *>(connector->getConnectedConnector());
            m->getOtherSide()->setIsControlConnected(false);
        }
        if(controlconnector->getSide().compare("OUT") == 0) connector->disconnect();
        controlconnector->disconnect();
        connector->setIsControlConnected(false);
        controlconnector->setIsControlConnected(false);
        controlconnector->update();
        connector->update();
        if(CGCompositionManager::getInstance()->getCurrentComposerScene()!=0)CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
        if(CGLayoutManager::getInstance()->getCurrentLayout()!=0)CGLayoutManager::getInstance()->getCurrentLayout()->update();
    }else{
        wasConnectedTo = connector->getConnectedConnector();
        connector->disconnect();
        wasConnectedTo->disconnect();
        CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
    }
}

void CGDisconnectConnector::unexec(){
    if(control != NULL){
        controlconnector->setIsControlConnected(true);
        connector->setIsControlConnected(true);
        connector->connect(controlconnector);
        controlconnector->connect(connector);
        controlconnector->update();
        connector->update();
        CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
        CGLayoutManager::getInstance()->getCurrentLayout()->update();
    }else{
        wasConnectedTo->connect(connector);
        connector->connect(wasConnectedTo);
    }
}

void CGDisconnectConnector::setControl(CControl* control){
    this->control = control;
}

void CGDisconnectConnector::setConnector(CGConnector* connector){
    this->connector = connector;
}

void CGDisconnectConnector::setControlConnector(CGConnector* controlconnector){
    this->controlconnector = controlconnector;
}
