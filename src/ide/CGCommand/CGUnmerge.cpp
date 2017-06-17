/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGUnmerge.h"
#include "../CGSceneManager.h"

CGUnmerge::CGUnmerge()
{
    type = "UNMERGE";
}

bool CGUnmerge::belongToThis(CGConnector* conn){
    foreach(CGOperatorModel*ope, operatorsList){
        foreach(CGConnectorModel* connector,ope->getConnectorsList()){
            CGConnector* vc = ope->getView()->getConnectorById(connector->getId());
            if(vc == conn){
                return true;
            }
        }
    }
    foreach(CGCompositionModel*compo, modelsList){
        foreach(CGConnector* vc,compo->getOuterView()->getConnectorsList()){
            if(vc->isConnected()){
                if(vc == conn){
                    return true;
                }
            }
        }
    }
    return false;
}

void CGUnmerge::exec()
{
//    CGSceneManager::getInstance()->getCurrentComposerScene();
//    //record connection
//    modelsList = model->getCompositionModelList();
//    operatorsList = model->getOperatorModelList();
//    foreach(CGOperatorModel*ope, operatorsList){
//        foreach(CGConnectorModel* connector,ope->getConnectorsList()){
//            CGConnector* vc = ope->getView()->getConnectorById(connector->getId());
//            if(vc->isConnected() && !belongToThis(vc->getConnectedConnector())){
//                CGMultiConnector* mc = qgraphicsitem_cast<CGMultiConnector *>(vc->getConnectedConnector());;
//                CGConnector* toConnect = NULL;
//                if(mc->getParentConnector()->getOuterConnector()->getConnectedConnector()!=NULL){
//                    toConnect = mc->getParentConnector()->getOuterConnector()->getConnectedConnector();
//                    vc->disconnect();
//                    toConnect->disconnect();
//                }
//                if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
//            }else if(vc->isConnected()){
//                CGConnector* toConnect = vc->getConnectedConnector();
//                if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
//            }
//        }
//    }
//    foreach(CGCompositionModel*compo, modelsList){
//        foreach(CGConnector* vc,compo->getOuterView()->getConnectorsList()){
//            if(vc->isConnected() && !belongToThis(vc)){
//                if(vc->isConnected() && !belongToThis(vc->getConnectedConnector())){
//                    CGMultiConnector* mc = qgraphicsitem_cast<CGMultiConnector *>(vc->getConnectedConnector());;
//                    CGConnector* toConnect = NULL;
//                    if(mc->getParentConnector()->getOuterConnector()->getConnectedConnector()!=NULL){
//                        toConnect = mc->getParentConnector()->getOuterConnector()->getConnectedConnector();
//                        vc->disconnect();
//                        toConnect->disconnect();
//                    }
//                    if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
//                }else if(vc->isConnected()){
//                    CGConnector* toConnect = vc->getConnectedConnector();
//                    if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
//                }
//            }
//        }
//    }
//    foreach(CGOperatorModel*ope, operatorsList){
//        model->removeOperatorModel(ope);
//        model->getParentCompositionModel()->addOperatorModel(ope);
//        foreach(CGConnector* connector,ope->getView()->getConnectorsList()){
//            if(connector->getConnectedConnector()==NULL && recordConnectedConnector.contains(connector)){
//                connector->connect(recordConnectedConnector.value(connector));
//                recordConnectedConnector.value(connector)->connect(connector);
//            }
//        }
//    }
//    foreach(CGCompositionModel*compo, modelsList){
//        model->removeCompositionModel(compo);
//        model->getParentCompositionModel()->addCompositionModel(compo);
//        foreach(CGConnector* connector,compo->getOuterView()->getConnectorsList()){
//            if(connector->getConnectedConnector()==NULL && recordConnectedConnector.contains(connector)){
//                connector->connect(recordConnectedConnector.value(connector));
//                recordConnectedConnector.value(connector)->connect(connector);
//            }
//        }
//    }
//    model->getParentCompositionModel()->removeCompositionModel(this->model);
}

void CGUnmerge::unexec()
{
    qDebug() << "CGUnmerge::unexec, TODO COMMAND";

}

void CGUnmerge::setModel(CGCompositionModel* model){
    this->model = model;
}
