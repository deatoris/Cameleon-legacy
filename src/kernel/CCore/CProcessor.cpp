/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CProcessor.h"
#include<CGlossary.h>
#include<CError.h>
#include<string>
#include <iostream>
#include <fstream>
#include <sstream>
#include<QDebug>
using namespace std;
#include<CConnectorDirect.h>
#include<CMachine.h>
#include"CLogger.h"
CProcessor::CProcessor()
    :_current_process(0),_player(STOP)
{
    _process_waiting_execution.push_back(true);
    _process_stack.push_back(std::queue<int>());
    _process_mother.push_back(0);
}

void CProcessor::setNbrThread(int ){
}
CProcessor::~CProcessor()
{
    for(int i =0;i<(int)_compo.size();i++)
        delete _compo[i].first;
}
void CProcessor::start(){


    this->setPlayer(START);
    this->executingLoop();
    if(this->getPlayer()==STOP)
        this->stop();
    this->actualizeState();
}
void CProcessor::pause(){
    this->setPlayer(PAUSE);
}

void CProcessor::next(){
    this->setPlayer(NEXT);
    if(this->executingLoopOneEvent()==false){
        CMachineSingleton::getInstance()->getProcessor()->nextSend();
    }
}

void CProcessor::actualizeState(){
    CProcessorObject::ModeFilter temp = CMachineSingleton::getInstance()->getProcessor()->getFilterSignal();
    CMachineSingleton::getInstance()->getProcessor()->setFilterSignal(CProcessorObject::FILTEREDSEND);
    for(int i =0;i<(int)_compo.size();i++){
        if(_compo[i].second==ALIVE){

            COperator * op = _compo[i].first;
            op->updateState();
            vector<CPlugOut* >::iterator itout;
            for(itout =op->plugOut().begin();itout!=op->plugOut().end();itout++){
                (*itout)->update();
            }
            //All input plug becomes (NEW,OLD)->OLD
            vector<CPlugIn* >::iterator itin;
            for(itin =op->plugIn().begin();itin!=op->plugIn().end();itin++){
                (*itin)->update();
            }
        }
    }
    CMachineSingleton::getInstance()->getProcessor()->setFilterSignal(temp);
}


void CProcessor::stop(){

    this->setPlayer(STOP);
    this->initialisation();
    //_mutex.unlock();
}
void CProcessor::kill(){
}
void CProcessor::clear(){
    vector<std::queue<int> >::iterator it;
    for(it= _process_stack.begin();it!=_process_stack.end();it++){
        while (!it->empty() )
            it->pop();
    }
    vector<bool>::iterator it2;
    for(it2= _process_waiting_execution.begin();it2!=_process_waiting_execution.end();it2++){
        *it2 = false;
    }
    for(int i =0;i<(int)_compo.size();i++){
        if(_compo[i].second==ALIVE){
            _compo[i].first->setState(COperator::NOTRUNNABLE);
        }
    }
}

void CProcessor::initialisation(){
    this->clear();
    for(int i =0;i<(int)_compo.size();i++){
        if(_compo[i].second==ALIVE){
            _compo[i].first->initState();
            _compo[i].first->updateState();
        }
    }
}
void CProcessor::executingLoop(){


    while (this->getPlayer()==START){
        int idop = getOperator();
        if(idop==-1)
            break;
        executeOperator(idop);
    }

}
int CProcessor::getOperator(){

    if(_process_stack[_current_process].empty()==true){
        return -1;
    }
    else{
        int id= _process_stack[0].front();
        _process_stack[0].pop();
        if(_compo[id].first->executionCondition()==true)
            return id;
        else
            return getOperator();

    }
    //    if(_process_stack[_current_process].empty()==true){
    //        if(_current_process==0)
    //            return -1;
    //        else if(this->operatorInExecution()==true){
    //            return -1;
    //        }
    //        else{
    //            //CMachineSingleton::getInstance()->getProcessor()->updateUnitProcess(_current_process,COperator::NOTRUNNABLE);

    //            _process_waiting_execution[_current_process]=false;
    //            _current_process = _process_mother[_current_process];
    //            return getOperator();
    //        }
    //    }
    //    else{
    //        int id= _process_stack[_current_process].front();
    //        _process_stack[_current_process].pop();
    //        if(id<0)
    //        {
    //            _current_process = -id;
    //            CMachineSingleton::getInstance()->getProcessor()->updateUnitProcess(_current_process,COperator::RUNNING);
    //            return getOperator();
    //        }else{
    //            return id;
    //        }
    //    }
}

void CProcessor::addOperatorWaitingExecution(int opid){

    _process_stack[0].push(opid);
    //    _process_stack[_id_to_unit_process[opid]].push(opid);

    //    if(_process_waiting_execution[_id_to_unit_process[opid]]==false){
    //        _process_waiting_execution[_id_to_unit_process[opid]]=true;
    //        _process_stack[_process_mother[_id_to_unit_process[opid]]].push(-_id_to_unit_process[opid]);
    //    }
}
//bool CProcessor::operatorInExecution(){
//    return false;
//}

bool CProcessor::executingLoopOneEvent(){
    int idop;
    do{
        idop = getOperator();
        if(idop == -1)
            return false;
    }while(executeOperator(idop)==false);
    return true;
}


bool CProcessor::executeOperator(int opid){

    if(_compo[opid].second==ALIVE &&  _compo[opid].first->executionCondition()==true){
        _compo[opid].first->setState(COperator::RUNNING);
        CMachineSingleton::getInstance()->getProcessor()->updateOperator(opid,COperator::RUNNING);
        _compo[opid].first->exec();
        if(_compo[opid].first->getError()==false){
            try{

                _compo[opid].first->updateMarkingAfterExecution();
            }
            catch(const CErrorDumpData& err)
            {
                cerr<<"DUMP FAILED between\n operator "<<err.getOperatorPlugOutId()<<" and plug out :"<<err.gettPlugOutIndex()<<
                      "\n operator "<<err.getOperatorPlugInId()<<" and plug in :"<<err.gettPlugInIndex();
                this->stop();

            }
        }else{
            _compo[opid].first->setError(true);
        }
        return true;
    }
    else
        return false;
    return true;
}


COperator::Id CProcessor::newOperator(COperator::Key opkey,COperatorStructure & structure){

    try{
        COperator* op;
        CGlossary* inst = CGlossarySingletonServer::getInstance();
        op = inst->createOperator(opkey);
        if(op==NULL)
            return -1;
        op->setProcessor(this);
        op->createPlug();
        op->setId(_compo.size());
        _id_to_unit_process.push_back(0);
        structure = op->structurePlug();
        _compo.push_back(make_pair(op,DEAD));
        return _compo.size()-1;
    }
    catch (exception e )
    {
        std::cerr<<" in the Operator Factory, I cannot create this operator: "<< opkey<<endl;
        return -1;
    }



}

void CProcessor::desactivateOperator(COperator::Id opid){
    _compo[opid].second = DEAD;
}
void CProcessor::activateOperator(COperator::Id opid){
    _compo[opid].second = ALIVE;

    int b = _process_stack[_current_process].size();
    _compo[opid].first->initState();
    if(this->getPlayer()==PAUSE||this->getPlayer()==START){
        _compo[opid].first->updateState();
    }


    int a = _process_stack[_current_process].size();
    if(this->getPlayer()==START&& b==0&& a>=1)
        this->start();


}

void CProcessor::connectPlugOut2PlugIn(COperator::Id idop1,int plugindex1, COperator::Id idop2,int plugindex2){

    //    cout<<"[SERVER] connect operator1 "<<idop1<<" and plug out "<<int1<<" and connect operator2 "<<idop2<<" and plug in "<<int2<<endl;

    try{

        if(idop1>=(int)_compo.size()||idop2>=(int)_compo.size()){
            qDebug() << "kernel: Connexions error1 from "<< idop1 <<":"<<plugindex1 << "to " <<idop2 <<":"<<plugindex2;;
            throw CErrorBuilder("Over range in the operator vector ", "Cprocessor");
        }
        if(plugindex2>=(int)_compo[idop2].first->plugIn().size()){
            qDebug() << "kernel: Connexions error2 from "<< idop1 <<":"<<plugindex1 << "to " <<idop2 <<":"<<plugindex2 << "size: " <<(int)_compo[idop2].first->plugIn().size();
            throw CErrorBuilder("Over range in the plug vector ", "Cprocessor");
        }
        if(plugindex1>=(int)_compo[idop1].first->plugOut().size()){
            qDebug() << "kernel: Connexions error3 from "<< idop1 <<":"<<plugindex1 << "to " <<idop2 <<":"<<plugindex2;;
            throw CErrorBuilder("Over range in the plug vector ", "Cprocessor");
        }
        _compo[idop2].first->plugIn()[plugindex2]->connect(_compo[idop1].first->plugOut()[plugindex1]);
        _compo[idop2].first->connectPlugIn(plugindex2);
        _compo[idop2].first->connectPlugOut(plugindex1);


        int b = _process_stack[_current_process].size();
        _compo[idop2].first->updateState();
        if(this->getPlayer()==PAUSE||this->getPlayer()==START){
            if(_compo[idop1].first->plugOut()[plugindex1]->getState()!=CPlug::EMPTY)
                _compo[idop1].first->plugOut()[plugindex1]->send();
        }
        int a = _process_stack[_current_process].size();
        if(this->getPlayer()==START&& b==0&& a>=1)
            this->start();

    }
    catch(const CErrorBuilder & err)
    {
        //        cerr<<err;
        qDebug() << "kernel: Connexions error4 from "<< idop1 <<":"<<plugindex1 << "to " <<idop2 <<":"<<plugindex2 << "size: " <<(int)_compo[idop2].first->plugIn().size();

    }
}
bool CProcessor::isStartSleeping(){
    int b = _process_stack[_current_process].size();
    if(b==0||this->getPlayer()==START){
        return true;
    }
    else
        return false;
}
void CProcessor::deconnectPlugOut(COperator::Id idop,int plugindex){
    if(plugindex>=(int)_compo[idop].first->plugIn().size())
        plugindex = plugindex- (int)_compo[idop].first->plugIn().size();
    try{
        if((int)_compo.size() > idop)
            _compo[idop].first->plugOut()[plugindex]->deconnect();
    }
    catch(const CErrorBuilder & err)
    {
        cerr<<err;
    }
}
void CProcessor::deconnectPlugIn(COperator::Id idop,int plugindex){
    try{
        if((int)_compo.size() > idop)
            _compo[idop].first->plugIn()[plugindex]->deconnect();
    }
    catch(const CErrorBuilder & err)
    {
        cerr<<err;
    }
}
void CProcessor::        setPlugIn(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state){

    qDebug() << "setPlugIn: " << opid << ":" <<plugkeyin;

    _compo[opid].first->plugIn()[plugkeyin]->getData()->dumpReception(data);

    delete data;
    _compo[opid].first->plugIn()[plugkeyin]->setState(state);
    int b = _process_stack[_current_process].size();
    _compo[opid].first->updateState();
    int a = _process_stack[_current_process].size();
    if(this->getPlayer()==START&& b==0&& a>=1)
        this->start();
}

void CProcessor::clearComposition(){
    for(int i =0;i<(int)_compo.size();i++)
        delete _compo[i].first;
    _compo.clear();

}

CProcessor::CComposition & CProcessor::composition(){
    return _compo;
}
ostream & operator<<( ostream & os, const CProcessor& v){
    for(int i =0;i<(int)v._compo.size();i++)
    {
        if(v._compo[i].second==CProcessor::ALIVE)
        {
            //cout<<"OPERATOR "<<i<<endl;
            //cout<<v._compo[i].first;
        }
    }
    return os;
}
void CProcessor::setPlayer(Player player){
    _player = player;
}

CProcessor::Player CProcessor::getPlayer(){
    return _player;
}
void CProcessor::connectPlugInFromOutside(  COperator::Id  opid, COperator::PlugKey plugkeyin){
    _compo[opid].first->plugIn()[plugkeyin]->_connectedfromoutside = true;
}

void CProcessor::disconnectPlugInFromOutside(  COperator::Id  opid, COperator::PlugKey plugkeyin){
    _compo[opid].first->plugIn()[plugkeyin]->_connectedfromoutside =false;
}

int CProcessor::newUnitProcessor(){
    _process_stack.push_back(std::queue<int>());
    _process_waiting_execution.push_back(false);
    _process_mother.push_back(0);
    return (int)_process_mother.size()-1;
}
void CProcessor::linkUnitProcess(int unitprocessid, int unitprocessmotherid){
    _process_mother[unitprocessid] = unitprocessmotherid;
}

void CProcessor::unlinkUnitProcess(int unitprocessid){
    _process_mother[unitprocessid] = 0;
}

void CProcessor::addOperatorToUnitProcess(int idop,int idunitprocess){
    _id_to_unit_process[idop]=idunitprocess;
}

void CProcessor::removeOperatorToUnitProcess(int idop){
    _id_to_unit_process[idop]=0;
}
void CProcessor::updateOperator(COperator::Id opid,COperator::State state){
    CMachineSingleton::getInstance()->getProcessor()->updateOperator(opid,state);
    if(state==COperator::RUNNABLE&& _id_to_unit_process[opid]!=_current_process){
        CMachineSingleton::getInstance()->getProcessor()->updateUnitProcess(_id_to_unit_process[opid],state);
    }
}
void CProcessor::actionOperator(COperator::Id opid, string actionkey){
    _compo[opid].first->action(actionkey);
}

//void CProcessor::setState(State state){
//    _state = state;
//}

//CProcessor::State CProcessor::getState(){
//    return _state;
//}
//void CProcessor::getOperatorInformation(COperator::Id opid,COperator::Information & info){
//    this->_compo[opid].first->getInformation(info);
//}
