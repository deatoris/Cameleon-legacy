/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CInterfaceClient.h"

#include <QMetaType>
#include "CClient.h"
#include "CLogger.h"
#include <cli/CGProject.h>
#include<CClient.h>
bool Compare::operator()(const pair<int,int> & x,const pair<int,int> & y) {
    if(x.first<y.first)
        return true;
    else if (x.first>y.first)
        return false;
    else if(x.second<y.second)
        return true;
    else
        return false;
}

CInterfaceClient::CInterfaceClient(bool alllistening)
    :_alllistening(alllistening),_isstop(true)
{

    //    CLogger::getInstance()->registerComponent("CLIENTTOSERVER");
}

//BUILDER
void CInterfaceClient::createOperatorSend(COperator::Key Key,COperator::Id &opid,COperatorStructure &  opstructure){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"createOperatorSend");
    emit createOperatorSignal(Key);
    opid=_opid;
    opstructure=_opstructure;
    mapidoperatorstruc.insert(opid,opstructure);
}
//void CInterfaceClient::EndCreateOperatorSend(COperator::Id opid){
//    emit EndCreateOperatorSignal(opid);
//}

void CInterfaceClient::createUnitProcessSend(int& unitprocessid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"createUnitProcessSend");
    emit createUnitProcessSignal();
    unitprocessid = _unitprocessid;
}
void CInterfaceClient::linkUnitProcess(int unitprocessid, int unitprocessmotherid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"linkUnitProcess");
    emit linkUnitProcessSignal(unitprocessid, unitprocessmotherid);
}

void CInterfaceClient::unlinkUnitProcess(int unitprocessid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"unlinkUnitProcess");
    emit unlinkUnitProcessSignal(unitprocessid);
}

void CInterfaceClient::addOperatorToUnitProcessSend(int opid , int unitprocessid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"addOperatorToUnitProcessSend");
    emit addOperatorToUnitProcessSignal(opid,unitprocessid );
}

void CInterfaceClient::removeOperatorToUnitProcessSend(int opid ){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"removeOperatorToUnitProcessSend");
    emit removeOperatorToUnitProcessSignal(opid);
}

void CInterfaceClient::createOperatorReceptionSlot(COperator::Id opid,COperatorStructure   opstructure){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"createOperatorReceptionSlot");
    _opid = opid;
    _opstructure = opstructure;
}
void CInterfaceClient::createUnitProcessSlot(int unitprocessmotherid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"createUnitProcessSlot");
    _unitprocessid = unitprocessmotherid;
}


void CInterfaceClient::activateOperatorSend(COperator::Id opid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"activateOperatorSend");
    emit activateOperatorSignal(opid);
}

void CInterfaceClient::desactivateOperatorSend(COperator::Id opid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"desactivateOperatorSend");
    emit desactivateOperatorSignal(opid);
}

void CInterfaceClient::connectPlugOut2PlugInSend(  COperator::Id  opid1, COperator::PlugKey plugkeyout, COperator::Id opid2, COperator::PlugKey plugkeyin){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"connectPlugOut2PlugInSend");
    emit connectPlugOut2PlugInSignal(opid1,plugkeyout, opid2, plugkeyin);
}
void CInterfaceClient::disconnectPlugInSend(COperator::Id  opid, COperator::PlugKey plugkeyin){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"disconnectPlugInSend");
    emit disconnectPlugInSignal(opid,plugkeyin);
}
void CInterfaceClient::connectPlugOutFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyout){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"connectPlugOutFromOutsideSend");
    emit connectPlugOutFromOutsideSignal(opid1,plugkeyout);
}
void CInterfaceClient::connectPlugInFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyin){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"connectPlugInFromOutsideSend");

    emit connectPlugInFromOutsideSignal(opid1,plugkeyin);
}

void CInterfaceClient::disconnectPlugOutFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyout){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"disconnectPlugOutFromOutsideSend");
    emit disconnectPlugOutFromOutsideSignal(opid1,plugkeyout);
}

void CInterfaceClient::disconnectPlugInFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyin){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"disconnectPlugInFromOutsideSend");
    if(opid1<0){
        qDebug("PLANTAGE IS COMMING !!!!!!");
    }
    emit disconnectPlugInFromOutsideSignal(opid1,plugkeyin);
}

void CInterfaceClient::setNbrThreadSend(int nbrthread ){
    emit setNbrThreadSignal(nbrthread);
}

void CInterfaceClient::clearSend(){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"clearSend");
    emit clearSignal();
}

//PLAYER
void CInterfaceClient::startSend(){
    CLoggerInstance::getInstance()->log("CInterfaceClient::startSend()");

    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"startSend");

    setTmpPath(CGProject::getInstance()->getTmpPath().toStdString());
    if(_isstop == true)
        emit stopSignal();
    if(_isstop==true)
    {
        /*
        REFACTOR: ATTENTION A BIEN REMPLACER l'applycation des controller (ie, l'envoie des parametres)
        Par ce qui se passe sous apply

        QList<CGLayout*> lis = CGLayoutManager::getInstance()->getLayoutL();
        foreach (CGLayout* l , lis) {
            QList<QGraphicsItem*> listItem = l->items();
            foreach(QGraphicsItem*item, listItem)
                if(item->type() == CGLayoutItem::Type){
                    CGLayoutItem *layoutItem = qgraphicsitem_cast<CGLayoutItem *>(item);
                    CControl * c = layoutItem->getControl();
                    c->apply();
                }
        }
        QList<CGCompositionModel*> modelL = CGCompositionManager::getInstance()->getCompositionList();
        foreach (CGCompositionModel* l , modelL) {
            QList<QGraphicsItem*> listItem = l->getScene()->items();
            foreach(QGraphicsItem*item, listItem)
                if(item->type() == CGLayoutItem::Type){
                    CGLayoutItem *layoutItem = qgraphicsitem_cast<CGLayoutItem *>(item);
                    CControl * c = layoutItem->getControl();
                    if(c!=NULL && c!=0)c->apply();
                }
        }*/
    }
    emit startSignal();

    _isstop = false;
}

void CInterfaceClient::launchSignal(){
    emit startSignal();
    _isstop = false;
}

void CInterfaceClient::nextSend(){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"nextSend");
    setTmpPath(CGProject::getInstance()->getTmpPath().toStdString());
    if(_isstop == true)
        emit stopSignal();
    if(_isstop == true){

       /* QList<CGLayout*> lis = CGLayoutManager::getInstance()->getLayoutL();
        foreach (CGLayout* l , lis) {
            QList<QGraphicsItem*> listItem = l->items();
            foreach(QGraphicsItem*item, listItem)
                if(item->type() == CGLayoutItem::Type){
                    CGLayoutItem *layoutItem = qgraphicsitem_cast<CGLayoutItem *>(item);
                    CControl * c = layoutItem->getControl();
                    c->apply();
                }
        }
        QList<CGCompositionModel*> modelL = CGCompositionManager::getInstance()->getCompositionList();
        foreach (CGCompositionModel* l , modelL) {
            QList<QGraphicsItem*> listItem = l->getScene()->items();
            foreach(QGraphicsItem*item, listItem)
                if(item->type() == CGLayoutItem::Type){
                    CGLayoutItem *layoutItem = qgraphicsitem_cast<CGLayoutItem *>(item);
                    CControl * c = layoutItem->getControl();
                    c->apply();
                }
        }*/

    }
    emit nextSignal();

    _isstop = false;
}

void CInterfaceClient::nextReceive(){
   // CGCompositionManager::getInstance()->getComposer()->paused();
}

void CInterfaceClient::stopReceive(){
   // CGCompositionManager::getInstance()->getComposer()->stopped();
}

void CInterfaceClient::pauseSend(){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"pauseSend");
    emit pauseSignal();
}

void CInterfaceClient::pauseReceive(){
  //  CGCompositionManager::getInstance()->getComposer()->paused();
}

void CInterfaceClient::stopSend(){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"stopSend");
    if(_isstop==false){
        emit stopSignal();
        _isstop = true;
    }else{
        this->stopReceive();
    }
}
void CInterfaceClient::stopSlot(){
    this->stopReceive();
}
void CInterfaceClient::nextSlot(){
    this->nextReceive();
}
void CInterfaceClient::pauseSlot(){
    this->pauseReceive();
}
void CInterfaceClient::executedOperatorInClientSlot(COperator * op){
    this->executedOperatorInClient(op);
}
void CInterfaceClient::progressionExecutionSlot(double ratio){
    this->progressionExecution(ratio);
}

void CInterfaceClient::killSend(){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"killSend");
    emit killSignal();
}

void CInterfaceClient::setPlugInSend(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"setPlugInSend");
    emit setPlugInSignal( opid,  plugkeyin, data, state);
}


void CInterfaceClient::updateOperatorSlot(COperator::Id opid,COperator::State state){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"updateOperatorSlot");
    this->updateOperatorReceive(opid,state);
}
void CInterfaceClient::updateUnitProcessSlot(int unitprocessid,COperator::State state){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"updateUnitProcessSlot");
    this->updateUnitProcessReceive(unitprocessid,state);
}

void CInterfaceClient::updatePlugInSlot(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"updatePlugInSlot");
    this->updatePlugInReceive(opid, plugkey,  data, state);
}

void CInterfaceClient::updatePlugOutSlot(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"updatePlugOutSlot");
    this->updatePlugOutReceive(opid, plugkey,  data, state);
}
void CInterfaceClient::updateOperatorStructureSlot(COperator::Id opid,COperatorStructure   opstructure){
    this->updateOperatorStructureReceive(opid,opstructure);
}

void CInterfaceClient::updateOperatorReceive(COperator::Id opid,COperator::State state){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"updateOperatorReceive");
/*    CGOperatorModel * op = CGCompositionManager::getInstance()->searchOperatorById(opid);
    if(op!=NULL){
        CGOperator* view = op->getView();
        view->updateState(state);
        if(CGCompositionManager::getInstance()->getComposer()->getIsNext()){
            CGCompositionManager::getInstance()->getComposer()->setIsNext(false);
            CGComposition*composition = (CGComposition*)op->getView()->scene();
            CGCompositionManager::getInstance()->showSceneFromModel(composition->getModel());
            composition->unselectAll();
            composition->getView()->centerOn(op->getView()->scenePos());
            op->getView()->setSelected(true);
            op->getView()->setIsHighlight(true);
        }
    }*/
}

void CInterfaceClient::updateUnitProcessReceive(int unitprocessid,COperator::State state){
}


void CInterfaceClient::updatePlugInReceive(COperator::Id opid,COperator::PlugKey plugkey, CData * , CPlug::State state){
   /* CGOperatorModel * op = CGCompositionManager::getInstance()->searchOperatorById(opid);
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"updatePlugInReceive");
    if(op!=NULL&&plugkey<op->getConnectorsList().size()){
        CGConnectorModel* co = op->getConnectorsList()[plugkey];
        CGOperator* view = op->getView();
        view->getConnectorById(co->getId())->updateState(state);
    }*/
}
void CInterfaceClient::progressionExecution(double ){
    //PROGRESSION HERE
}

void CInterfaceClient::updatePlugOutReceive(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"updatePlugOutReceive");
    /*CGOperatorModel * op = CGCompositionManager::getInstance()->searchOperatorById(opid);
    if (op != NULL&&plugkey+op->getNumPlugIn()<op->getConnectorsList().size()){

        if(CClientSingleton::getInstance()->getMode()==IDE){
            CGConnectorModel* co = op->getConnectorsList()[plugkey+op->getNumPlugIn()];
            CGOperator* view = op->getView();
            view->getConnectorById(co->getId())->updateState(state);
        }

        vector<pair<bool, pair<int,pair<int, pair<CControl *,int> > > > >::iterator it;
        for(it = _v_control.begin();it!=_v_control.end();it++ )
        {
            if(it->first==true&&it->second.first == opid && it->second.second.first == plugkey)
            {
                CControl * control =it->second.second.second.first;
                CControl::PlugKey plugkeycontrol =it->second.second.second.second;
                control->updatePlugInControlComingFromMachine(plugkeycontrol,data,state);
            }
        }

    }*/
}

void CInterfaceClient::updateOperatorStructureReceive(COperator::Id opid,COperatorStructure opstructure){
    //CGCompositionManager::getInstance()->updateStructOperator(opid,opstructure);
    //qDebug() << "[WARNING] Uncatch updateOperatorStructure (addPlug) " << opid << " " << opstructure.getName().c_str() << " " << opstructure.plugIn().size() << " " << opstructure.plugOut().size();
    mapidoperatorstruc.insert(opid,opstructure);
}

COperatorStructure CInterfaceClient::getOperatorStructureById(COperator::Id opid){
    return mapidoperatorstruc.value(opid);
}

/*
 * CUT FOR CLI
int CInterfaceClient::addListenerPlugOutOperator2PlugInControl(COperator::Id opid,COperator::PlugKey plugkeyop,CControl * control,CControl::PlugKey plugkeycontrol){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"addListenerPlugOutOperator2PlugInControl");
    _v_control.push_back(make_pair(true,make_pair(opid,make_pair(plugkeyop,make_pair(control,plugkeycontrol)))));

    emit connectListening( opid,  plugkeyop);
    return (int)_v_control.size()-1;
}

void CInterfaceClient::removeListenerPlugOutOperator2PlugInControl(int controlid){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"removeListenerPlugOutOperator2PlugInControl");
    _v_control[controlid].first = false;
    int opid = _v_control[controlid].second.first;
    int plugkey = _v_control[controlid].second.second.first;
    vector<pair<bool, pair<int,pair<int, pair<CControl *,int> > > > >::iterator it;
    bool _touch = false;
    for(it = _v_control.begin();it!=_v_control.end();it++ ){
        if(it->first==true&&it->second.first == opid && it->second.second.first == plugkey){
            _touch = true;
        }
    }
    if(_touch == false)
        emit disconnectListening( opid,  plugkey);

}*/

void CInterfaceClient::setTmpPath(string str){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"setTmpPath");
    emit setTmpPathSignal(str);
}

void CInterfaceClient::actionOperator(COperator::Id opid, string actionkey){
    emit actionOperatorSignal(opid,actionkey);
}

void CInterfaceClient::executedOperatorInClient(COperator * op){
    op->exec();
    emit endExecutedOperatorInClientSignal(op);
}

void CInterfaceClient::errorOperatorSlot(COperator::Id opid,string msg){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"errorOperatorSlot");
   /* CGOperatorModel * op = CGCompositionManager::getInstance()->searchOperatorById(opid);
    op->getView()->setError(msg.c_str());
    CGCompositionManager::getInstance()->addOpeError(op->getId(),msg.c_str());
    op->getView()->update();
    CGCompositionManager::getInstance()->getComposer()->pause();*/
}

void CInterfaceClient::connectWithServer2(CProcessorObject * processor){
    CLogger::getInstance()->log("CLIENTTOSERVER",CLogger::DEBUG,"connectWithServer2");
    qRegisterMetaType<COperator::Key>("COperator::Key");
    qRegisterMetaType<COperator::Id>("COperator::Id");
    qRegisterMetaType<COperator::PlugKey>("COperator::PlugKey");
    qRegisterMetaType<CPlug::State>("CPlug::State");
    qRegisterMetaType<COperator::State>("COperator::State");
    qRegisterMetaType<COperatorStructure>("COperatorStructure");
    qRegisterMetaType<string>("string");
    qRegisterMetaType<COperator *>("COpreator*");

    connect(this,SIGNAL(createOperatorSignal(COperator::Key)),processor,SLOT(createOperatorSlot(COperator::Key)),Qt::DirectConnection);
    connect(processor,SIGNAL(createOperatorReceptionSignal(COperator::Id ,COperatorStructure)),this,SLOT(createOperatorReceptionSlot(COperator::Id ,COperatorStructure)),Qt::DirectConnection);

    connect(this,SIGNAL(activateOperatorSignal(COperator::Id)),processor,SLOT(activateOperatorSlot(COperator::Id)));
    connect(this,SIGNAL(desactivateOperatorSignal(COperator::Id)),processor,SLOT(desactivateOperatorSlot(COperator::Id)));
    connect(this ,SIGNAL(connectPlugOut2PlugInSignal(COperator::Id,COperator::PlugKey,COperator::Id,COperator::PlugKey)),processor,SLOT(connectPlugOut2PlugInSlot(COperator::Id,COperator::PlugKey,COperator::Id,COperator::PlugKey)));
    connect(this ,SIGNAL(disconnectPlugInSignal(COperator::Id,COperator::PlugKey)),processor,SLOT(disconnectPlugInSlot(COperator::Id,COperator::PlugKey)));

    connect(this ,SIGNAL(connectPlugInFromOutsideSignal(COperator::Id,COperator::PlugKey)),processor,SLOT(connectPlugInFromOutsideSlot(COperator::Id,COperator::PlugKey)));
    connect(this ,SIGNAL(connectPlugOutFromOutsideSignal(COperator::Id,COperator::PlugKey)),processor,SLOT(connectPlugOutFromOutsideSlot(COperator::Id,COperator::PlugKey)));
    connect(this ,SIGNAL(disconnectPlugInFromOutsideSignal(COperator::Id,COperator::PlugKey)),processor,SLOT(disconnectPlugInFromOutsideSlot(COperator::Id,COperator::PlugKey)));
    connect(this ,SIGNAL(disconnectPlugOutFromOutsideSignal(COperator::Id,COperator::PlugKey)),processor,SLOT(disconnectPlugOutFromOutsideSlot(COperator::Id,COperator::PlugKey)));
    connect(this ,SIGNAL(setNbrThreadSignal(int)),processor,SLOT(setNbrThreadSlot(int)));

    connect(this ,SIGNAL(clearSignal()),processor,SLOT(clearSlot()));
    connect(this ,SIGNAL(startSignal()),processor,SLOT(startSlot()));
    connect(this ,SIGNAL(nextSignal()),processor,SLOT(nextSlot()));
    connect(processor ,SIGNAL(nextSignal()),this,SLOT(nextSlot()));
    connect(this ,SIGNAL(pauseSignal()),processor,SLOT(pauseSlot()));
    connect(processor ,SIGNAL(pauseSignal()),this,SLOT(pauseSlot()));
    connect(this ,SIGNAL(stopSignal()),processor,SLOT(stopSlot()));
    connect(processor ,SIGNAL(stopSignal()),this,SLOT(stopSlot()));
    connect(this ,SIGNAL(killSignal()),processor,SLOT(killSlot()));

    connect(this ,SIGNAL(setPlugInSignal(COperator::Id,COperator::PlugKey,CData*,CPlug::State)),processor,SLOT(setPlugInSlot(COperator::Id,COperator::PlugKey,CData*,CPlug::State)));
    connect(processor,SIGNAL(updateOperatorSignal(COperator::Id,COperator::State)),this,SLOT(updateOperatorSlot(COperator::Id,COperator::State)));
    connect(processor,SIGNAL(updateUnitProcessSignal(int,COperator::State)),this,SLOT(updateUnitProcessSlot(int,COperator::State)));

    connect(processor,SIGNAL(updatePlugInSignal(COperator::Id,COperator::PlugKey,CData*,CPlug::State)),this,SLOT(updatePlugInSlot(COperator::Id,COperator::PlugKey,CData*,CPlug::State)));
    connect(processor,SIGNAL(updatePlugOutSignal(COperator::Id,COperator::PlugKey,CData*,CPlug::State)),this,SLOT(updatePlugOutSlot(COperator::Id,COperator::PlugKey,CData*,CPlug::State)));

    connect(this,SIGNAL(connectListening(COperator::Id,COperator::PlugKey)),processor,SLOT(connectListening(COperator::Id,COperator::PlugKey)));
    connect(this,SIGNAL(disconnectListening(COperator::Id,COperator::PlugKey)),processor,SLOT(disconnectListening(COperator::Id,COperator::PlugKey)));

    connect(this,SIGNAL(setTmpPathSignal(string)),processor,SLOT(setTmpPathSlot(string)),Qt::DirectConnection);

    connect(processor,SIGNAL(errorOperatorSignal(COperator::Id,string)),this,SLOT(errorOperatorSlot(COperator::Id,string)));

    connect(this,SIGNAL(createUnitProcessSignal()),processor,SLOT(createUnitProcessSlot()),Qt::DirectConnection);
    connect(processor,SIGNAL(createUnitProcessSignal(int)),this,SLOT(createUnitProcessSlot(int)),Qt::DirectConnection);
    connect(this,SIGNAL(addOperatorToUnitProcessSignal(int,int)),processor,SLOT(addOperatorToUnitProcessSlot(int,int)));
    connect(this,SIGNAL(removeOperatorToUnitProcessSignal(int)),processor,SLOT(removeOperatorToUnitProcessSlot(int)));
    connect(this,SIGNAL(linkUnitProcessSignal(int,int)),processor,SLOT(linkUnitProcessSlot(int,int)));
    connect(this,SIGNAL(unlinkUnitProcessSignal(int)),processor,SLOT(unlinkUnitProcessSlot(int)));

    connect(this,SIGNAL(actionOperatorSignal(COperator::Id,string)),processor,SLOT(actionOperatorSlot(COperator::Id,string)),Qt::DirectConnection);

    connect(processor,SIGNAL(updateOperatorStructureSignal(COperator::Id,COperatorStructure)),this,SLOT(updateOperatorStructureSlot(COperator::Id,COperatorStructure)),Qt::DirectConnection);

   connect(processor,SIGNAL(progressionExecutionSignal(double)),this,SLOT(progressionExecutionSlot(double)));


    connect(processor,SIGNAL(executedOperatorInClientSignal(COperator*)),this,SLOT(executedOperatorInClientSlot(COperator*)));
    connect(this,SIGNAL(endExecutedOperatorInClientSignal(COperator*)),processor,SLOT(endExecutedOperatorInClientSlot(COperator*)));

}

