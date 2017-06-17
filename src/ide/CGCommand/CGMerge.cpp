/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGMerge.h"
#include "../CGSceneManager.h"
#include "../component/CGOperator.h"
#include "../CGComposer.h"
CGMerge::CGMerge()
{
    compositionModel = NULL;
    multi = NULL;
    type = "MERGE";
}

void CGMerge::exec(){
    //remove item from scene

//    if(compositionModel == NULL && multi == NULL){
//        compositionModel = new CGCompositionModel(parentComposition);
//        multi = compositionModel->getOuterView();
//        multi->setSelected(false);
//    }else{
//        parentComposition->addCompositionModel(compositionModel);

//    }
//    foreach(CGOperatorModel* ope, operatorsList){
//        foreach(CGConnector* con, ope->getView()->getConnectorsList()){
//            if(con->getConnectedConnector() != NULL){
//                recordConnectedConnector.insert(con, con->getConnectedConnector());
//            }
//        }
//        parentComposition->removeOperatorModel(ope);
//        compositionModel->addOperatorModel(ope);
//    }
//    foreach(CGCompositionModel* composition, modelsList){
//        foreach(CGConnector* con, composition->getOuterView()->getConnectorsList()){
//            if(con->getConnectedConnector() != NULL){
//                recordConnectedConnector.insert(con, con->getConnectedConnector());
//            }
//        }
//        parentComposition->removeCompositionModel(composition);
//        compositionModel->addCompositionModel(composition);
//    }

//    //reconnect
//    foreach(CGOperatorModel* ope, operatorsList){
//        foreach(CGConnector* con, ope->getView()->getConnectorsList()){
//            if(recordConnectedConnector.contains(con)){
//                if(!belongToThis(recordConnectedConnector.value(con)) && !con->isConnected()){
//                    compositionModel->getInnerView()->addConnection(con, recordConnectedConnector.value(con));
//                }else if(!con->isConnected()){
//                    con->connect(recordConnectedConnector.value(con));
//                }
//            }
//        }
//    }
//    foreach(CGCompositionModel* composition, modelsList){
//        foreach(CGConnector* con, composition->getOuterView()->getConnectorsList()){
//            if(recordConnectedConnector.contains(con)){
//                if(!belongToThis(recordConnectedConnector.value(con)) && !con->isConnected()){
//                    compositionModel->getInnerView()->addConnection(con, recordConnectedConnector.value(con));
//                }else if(!con->isConnected()){
//                    con->connect(recordConnectedConnector.value(con));
//                }
//            }
//        }
//    }

}

void CGMerge::unexec(){
    //diconnect top item
    //delete it
//    parentComposition->removeCompositionModel(compositionModel);

//    //demove others items from it
//    foreach(CGOperatorModel* ope, operatorsList){
//        foreach(CGConnector* con, ope->getView()->getConnectorsList()){
//            con->disconnect();
//        }
//        parentComposition->addOperatorModel(ope);

//    }
//    foreach(CGCompositionModel* composition, modelsList){
//        foreach(CGConnector* con, composition->getOuterView()->getConnectorsList()){
//            con->disconnect();
//        }
//        parentComposition->addCompositionModel(composition);
//    }

//    //reconnect
//    foreach(CGOperatorModel* ope, operatorsList){
//        foreach(CGConnector* con, ope->getView()->getConnectorsList()){
//            if(recordConnectedConnector.contains(con)){
//                if(!con->isConnected()){
//                    con->connect(recordConnectedConnector.value(con));
//                }
//            }
//        }
//    }
//    foreach(CGCompositionModel* composition, modelsList){
//        foreach(CGConnector* con, composition->getOuterView()->getConnectorsList()){
//            if(recordConnectedConnector.contains(con)){
//                if(!con->isConnected()){
//                    con->connect(recordConnectedConnector.value(con));
//                }
//            }
//        }
//    }
}

void CGMerge::setParentComposition(CGCompositionModel* parentComposition){
    this->parentComposition = parentComposition;
}

void CGMerge::setOperatorsList(QList<CGOperatorModel*> operatorsList){
    this->operatorsList = operatorsList;
}

void CGMerge::setModelList(QList<CGCompositionModel*> modelsList){
    this->modelsList = modelsList;
}

void CGMerge::setShapesList(QList<CGShapeModel*> shapesList){
    this->shapesList = shapesList;
}

void CGMerge::setTextsList(QList<CGTextModel*> textsList){
    this->textsList = textsList;
}

void CGMerge::setName(QString name){

}

void CGMerge::setMenu(QMenu* operatormenu, QMenu* connectormenu)
{
    this->operatormenu = operatormenu;
    this->connectormenu = connectormenu;
}

void CGMerge::setEventPos(QPointF eventScenePos){
    this->eventScenePos = eventScenePos;
}

void CGMerge::setComposer(CGComposer* composer){
    this->composer = composer;
}

bool CGMerge::belongToThis(CGConnector* conn){
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
            if(vc == conn){
                return true;
            }
        }
    }
    return false;
}
