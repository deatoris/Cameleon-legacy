/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CProcessorObject.h"
#include<CMachine.h>
#define MAXOP 5

CProcessorObject::CProcessorObject()
    :_processor(new CProcessorThread())
{
    //create the unit process zero
    _v_listening_unit_process.push_back(make_pair(QTime(),MAXOP));
    _v_listening_unit_process.rbegin()->first.start();
    _v_listening_unit_process_is_filter.push_back(false);
    _filtersignal =FILTERING;
    _modesignal   =IDE;
    _alllistening =true;
    _time_ms=400;
}
Mode CProcessorObject::getMode(){
    return _modesignal;
}

void CProcessorObject::setMode(Mode mode){
    _modesignal = mode;
}

CProcessorObject::~CProcessorObject(){
    delete _processor;
}

CProcessorObject::ModeFilter CProcessorObject::getFilterSignal(){
    return _filtersignal;
}

void CProcessorObject::setFilterSignal(ModeFilter filtersignal){
    _filtersignal = filtersignal;
}

//BUILDER SLOTS
void CProcessorObject::createOperatorSlot(COperator::Key opkey){
    this->createOperatorReceive(opkey);
}


void CProcessorObject::activateOperatorSlot(COperator::Id opid){
    this->activateOperatorReceive(opid);
}

void CProcessorObject::desactivateOperatorSlot(COperator::Id opid){
    this->desactivateOperatorReceive(opid);
}

void CProcessorObject::connectPlugOut2PlugInSlot(  COperator::Id  opid1, COperator::PlugKey plugkey1, COperator::Id opid2, COperator::PlugKey plugkey2){
    this->connectPlugOut2PlugInReceive(opid1,plugkey1,opid2,plugkey2);

}

void CProcessorObject::disconnectPlugInSlot(COperator::Id  opid, COperator::PlugKey plugkeyin){
    this->disconnectPlugInReceive( opid,  plugkeyin);
}

void CProcessorObject::createUnitProcessSlot(){
    this->createUnitProcess();
}
void CProcessorObject::linkUnitProcessSlot(int unitprocessid, int unitprocessmotherid){
    this->linkUnitProcess(unitprocessid,  unitprocessmotherid);
}

void CProcessorObject::unlinkUnitProcessSlot(int unitprocessid){
    this->unlinkUnitProcess(unitprocessid);
}

void CProcessorObject::addOperatorToUnitProcessSlot(int opid , int unitprocessid){
    this->addOperatorToUnitProcess(opid , unitprocessid);
}

void CProcessorObject::removeOperatorToUnitProcessSlot(int opid ){
    this->removeOperatorToUnitProcess(opid);
}
void CProcessorObject::setNbrThreadSlot(int nbrthread){
    this->setNbrThread(nbrthread);
}

//PLAYER SLOTS
void CProcessorObject::clearSlot(){
    this->clearReceive();
}
#include "CLogger.h"
void CProcessorObject::startSlot(){
    CLoggerInstance::getInstance()->log("CProcessorObject::startSlot()");
    this->startReceive();
}

void CProcessorObject::nextSlot(){
    this->nextReceive();
}

void CProcessorObject::pauseSlot(){
    this->pauseReceive();
}

void CProcessorObject::stopSlot(){
    this->stopReceive();
}

void CProcessorObject::killSlot(){
    this->killReceive();
}
void CProcessorObject::setPlugInSlot(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state){
    this->setPlugInReceive(opid,  plugkeyin,  data, state);
}
void CProcessorObject::actionOperatorSlot(COperator::Id opid, string actionkey){
    qDebug() << "Kernel trace: "<< opid << " " << actionkey.c_str();
    actionOperator(opid,  actionkey);
}

void CProcessorObject::endExecutedOperatorInClientSlot(COperator * op){
    this->endExecutedOperatorInClient(op);
}

///////////////// FUNCTIONEL ////////////////////////////////
//BUILDER the compostion
void CProcessorObject::createOperatorReceive(COperator::Key opkey){
    //    QMutexLocker locker(&_mutex);
    COperatorStructure   opstructure;
    int idin1  =_processor->newOperator(opkey,opstructure);
    createOperatorSend(idin1,opstructure);

    //getchar();

}
void CProcessorObject::createOperatorSend(COperator::Id opid,COperatorStructure   opstructure){
    QMutexLocker locker(&_mutex);
    if(opid!=-1)
    {
        vector<bool > vbb(opstructure.plugOut().size(),false);
        _v_listening_control_plug_out.push_back(vbb);
        vector<pair<QTime,int> > v(opstructure.plugOut().size(),make_pair(QTime(),MAXOP));
        for(int i =0;i<(int)v.size();i++)
            v[i].first.start();
        _v_listening_plug_out.push_back(v);
        vector<bool > vf(opstructure.plugOut().size(),false);
        _v_listening_plug_out_is_filter.push_back(vf);

        vector<pair<QTime,int> > v2(opstructure.plugIn().size(),make_pair(QTime(),MAXOP));
        for(int i =0;i<(int)v2.size();i++)
            v2[i].first.start();
        _v_listening_plug_in.push_back(v2);
        vector<bool > vf2(opstructure.plugIn().size(),false);
        _v_listening_plug_in_is_filter.push_back(vf2);


        _v_listening_op.push_back(make_pair(QTime(),MAXOP));
        _v_listening_op.rbegin()->first.start();
        _v_listening_op_is_filter.push_back(false);

        emit createOperatorReceptionSignal(opid, opstructure);

    }
}
void CProcessorObject::activateOperatorReceive(COperator::Id opid){
    //    QMutexLocker locker(&_mutex);
    _processor->activateOperator(opid);
}

void CProcessorObject::desactivateOperatorReceive(COperator::Id opid){
    _processor->desactivateOperator(opid);
}

void CProcessorObject::connectPlugOut2PlugInReceive(  COperator::Id  opid1, COperator::PlugKey plugkey1, COperator::Id opid2, COperator::PlugKey plugkey2){
    _processor->connectPlugOut2PlugIn(opid1,plugkey1,opid2,plugkey2);
}

void CProcessorObject::disconnectPlugInReceive(COperator::Id  opid, COperator::PlugKey plugkeyin){
    _processor->deconnectPlugIn(opid, plugkeyin);
}
void CProcessorObject::setNbrThread(int nbrthread){
    _processor->setNbrThread(nbrthread);
}
void CProcessorObject::updateOperatorStructure(COperator::Id opid,COperatorStructure   opstructure){
    QMutexLocker locker(&_mutex);
    if(opid!=-1)
    {
        vector<bool > vbb(opstructure.plugOut().size(),false);
        _v_listening_control_plug_out[opid]=vbb;
        vector<pair<QTime,int> > v(opstructure.plugOut().size(),make_pair(QTime(),MAXOP));
        for(int i =0;i<(int)v.size();i++)
            v[i].first.start();
        _v_listening_plug_out[opid]=v;
        vector<bool > vf(opstructure.plugOut().size(),false);
        _v_listening_plug_out_is_filter[opid]=vf;

        vector<pair<QTime,int> > v2(opstructure.plugIn().size(),make_pair(QTime(),MAXOP));
        for(int i =0;i<(int)v2.size();i++)
            v2[i].first.start();
        _v_listening_plug_in[opid]=v2;
        vector<bool > vf2(opstructure.plugIn().size(),false);
        _v_listening_plug_in_is_filter[opid]=vf2;


        _v_listening_op[opid]=make_pair(QTime(),MAXOP);
        _v_listening_op[opid].first.start();
        _v_listening_op_is_filter[opid]=false;

        emit updateOperatorStructureSignal(opid,opstructure);

    }



}


void CProcessorObject::clearReceive(){
    QMutexLocker locker(&_mutex);
    _processor->clearComposition();
    _v_listening_control_plug_out.clear();
    _v_listening_plug_out.clear();
    _v_listening_plug_in.clear();
    _v_listening_op.clear();
    _v_listening_unit_process.clear();
    _v_listening_plug_out_is_filter.clear();
    _v_listening_plug_in_is_filter.clear();
    _v_listening_op_is_filter.clear();
    _v_listening_unit_process_is_filter.clear();
    //create the unit process zero
    _v_listening_unit_process.push_back(make_pair(QTime(),MAXOP));
    _v_listening_unit_process.rbegin()->first.start();
    _v_listening_unit_process_is_filter.push_back(false);
}

void CProcessorObject::createUnitProcess(){
    int unitprocessid  =_processor->newUnitProcessor();
    emit createUnitProcessSignal(unitprocessid);

    _v_listening_unit_process.push_back(make_pair(QTime(),MAXOP));
    _v_listening_unit_process.rbegin()->first.start();
    _v_listening_unit_process_is_filter.push_back(false);
}

void CProcessorObject::addOperatorToUnitProcess(int opid , int unitprocessid){
    _processor->addOperatorToUnitProcess(opid,unitprocessid);
}
void CProcessorObject::removeOperatorToUnitProcess(int opid ){
    _processor->removeOperatorToUnitProcess(opid);
}
void CProcessorObject::linkUnitProcess(int unitprocessid, int unitprocessmotherid){
    _processor->linkUnitProcess(unitprocessid,unitprocessmotherid);
}
void CProcessorObject::unlinkUnitProcess(int unitprocessid){
    _processor->unlinkUnitProcess(unitprocessid);
}

//PLAYER the composition
void CProcessorObject::startReceive(){
    _filtersignal =FILTERING;
    _processor->start();
}
void CProcessorObject::nextReceive(){
    _filtersignal =NOFILTERING;
    _processor->next();

}
void CProcessorObject::nextSend(){
    emit nextSignal();
}

void CProcessorObject::pauseReceive(){
    _filtersignal =NOFILTERING;
    _processor->pause();
}
void CProcessorObject::pauseSend(){
    emit pauseSignal();
}

void CProcessorObject::stopReceive(){

    _filtersignal =NOFILTERING;
    _processor->stop();

}
void CProcessorObject::stopSend(){
    emit stopSignal();
}

void CProcessorObject::killReceive(){

    _processor->kill();
}
//void CProcessorObject::initFilterSignal(){

//}

//INTERACTION client server
void CProcessorObject::setPlugInReceive(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state){

    _processor->setPlugIn(opid,  plugkeyin,  data,  state);
}

void CProcessorObject::updateOperator(COperator::Id opid,COperator::State state){
    QMutexLocker locker(&_mutex);
    if(_modesignal==IDE){
        if(_filtersignal==FILTERING)
        {
            if(_v_listening_op[opid].second>=0)
            {
                _v_listening_op[opid].second--;
                _v_listening_op_is_filter[opid]=false;
                emit updateOperatorSignal(opid,state);
            }
            else
            {
                int time = _v_listening_op[opid].first.elapsed();
                if(time >=_time_ms)
                {
                    _v_listening_op[opid].first.restart();
                    _v_listening_op[opid].second=MAXOP;
                    _v_listening_op_is_filter[opid]=false;
                    emit updateOperatorSignal(opid,state);
                }
                else
                    _v_listening_op_is_filter[opid]=true;
            }
        }
        else if(_filtersignal==NOFILTERING)
        {
            _v_listening_op_is_filter[opid]=false;
            emit updateOperatorSignal(opid,state);
        }
        else
        {
            if(_v_listening_op_is_filter[opid]==true){
                _v_listening_op_is_filter[opid]=false;
                emit updateOperatorSignal(opid,state);
            }
        }
    }
}
void CProcessorObject::updateUnitProcess(int unit_processid,COperator::State state){
    QMutexLocker locker(&_mutex);
    if(_modesignal==IDE){
        if(_filtersignal==FILTERING)
        {
            if(_v_listening_unit_process[unit_processid].second>=0)
            {
                _v_listening_unit_process[unit_processid].second--;
                _v_listening_unit_process_is_filter[unit_processid]=false;
                emit updateUnitProcessSignal(unit_processid,state);
            }
            else
            {
                int time = _v_listening_unit_process[unit_processid].first.elapsed();
                if(time >=_time_ms)
                {
                    _v_listening_unit_process[unit_processid].first.restart();
                    _v_listening_unit_process[unit_processid].second=MAXOP;
                    _v_listening_unit_process_is_filter[unit_processid]=false;
                    emit updateUnitProcessSignal(unit_processid,state);
                }
                else
                    _v_listening_unit_process_is_filter[unit_processid]=true;
            }
        }
        else if(_filtersignal==NOFILTERING)
        {
            _v_listening_unit_process_is_filter[unit_processid]=false;
            emit updateUnitProcessSignal(unit_processid,state);
        }
        else
        {
            if(_v_listening_unit_process_is_filter[unit_processid]==true){
                _v_listening_unit_process_is_filter[unit_processid]=false;
                emit updateUnitProcessSignal(unit_processid,state);
            }
        }
    }
}
void CProcessorObject::progressionExecution(double ratio){
    emit progressionExecutionSignal(ratio);
}

void CProcessorObject::updatePlugIn(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state){
    QMutexLocker locker(&_mutex);
    if(_modesignal==IDE){
        if(_filtersignal==FILTERING)
        {
            if(_v_listening_plug_in[opid][plugkey].second>=0)
            {
                _v_listening_plug_in[opid][plugkey].second--;
                _v_listening_plug_in_is_filter[opid][plugkey]=false;
                emit updatePlugInSignal(opid,plugkey,data,state);
            }
            else
            {
                int time = _v_listening_plug_in[opid][plugkey].first.elapsed();
                if(time >=_time_ms)
                {
                    _v_listening_plug_in[opid][plugkey].first.restart();
                    _v_listening_plug_in[opid][plugkey].second=MAXOP;
                    _v_listening_plug_in_is_filter[opid][plugkey]=false;
                    emit updatePlugInSignal(opid,plugkey,data,state);
                }
                else
                    _v_listening_plug_in_is_filter[opid][plugkey]=true;
            }
        }
        else if(_filtersignal==NOFILTERING)
        {
            _v_listening_plug_in_is_filter[opid][plugkey]=false;
            emit updatePlugInSignal(opid,plugkey,data,state);
        }
        else
        {
            if(_v_listening_plug_in_is_filter[opid][plugkey]==true){
                _v_listening_plug_in_is_filter[opid][plugkey]=false;
                emit updatePlugInSignal(opid,plugkey,data,state);
            }
        }
    }
}
void CProcessorObject::updatePlugOut(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state){

    QMutexLocker locker(&_mutex);
    if(_modesignal==IDE || (_modesignal==GUI&&_v_listening_control_plug_out[opid][plugkey]==true) ){
        if(_filtersignal==FILTERING)
        {
            if(_v_listening_plug_out[opid][plugkey].second>=0)
            {
                _v_listening_plug_out[opid][plugkey].second--;
                _v_listening_plug_out_is_filter[opid][plugkey]=false;
                emit updatePlugOutSignal(opid,plugkey,data,state);
            }
            else
            {
                int time = _v_listening_plug_out[opid][plugkey].first.elapsed();
                if(time >=_time_ms)
                {
                    _v_listening_plug_out[opid][plugkey].first.restart();
                    _v_listening_plug_out[opid][plugkey].second=MAXOP;
                    _v_listening_plug_out_is_filter[opid][plugkey]=false;
                    emit updatePlugOutSignal(opid,plugkey,data,state);
                }
                else
                    _v_listening_plug_out_is_filter[opid][plugkey]=true;
            }
        }
        else  if(_filtersignal==NOFILTERING)
        {
            _v_listening_plug_out_is_filter[opid][plugkey]=false;
            emit updatePlugOutSignal(opid,plugkey,data,state);

        }
        else
        {
            if(_v_listening_plug_out_is_filter[opid][plugkey]==true){
                _v_listening_plug_out_is_filter[opid][plugkey]=false;
                emit updatePlugOutSignal(opid,plugkey,data,state);
            }
        }
    }
}

void CProcessorObject::connectListening(COperator::Id opid, COperator::PlugKey plugkeyout){
    QMutexLocker locker(&_mutex);
    _v_listening_control_plug_out[opid][plugkeyout]=true;
    if(plugkeyout>=0&&plugkeyout<(int)_processor->composition()[opid].first->plugOut().size())
        emit updatePlugOutSignal(opid,plugkeyout,_processor->composition()[opid].first->plugOut()[plugkeyout]->getData(),_processor->composition()[opid].first->plugOut()[plugkeyout]->getState());

}

void CProcessorObject::disconnectListening(COperator::Id opid, COperator::PlugKey plugkeyout ){
    QMutexLocker locker(&_mutex);
    _v_listening_control_plug_out[opid][plugkeyout]=false;
}
void CProcessorObject::connectPlugOutFromOutsideSlot(  COperator::Id  , COperator::PlugKey ){
    //nothing to do
}

void CProcessorObject::connectPlugInFromOutsideSlot(  COperator::Id  opid1, COperator::PlugKey plugkeyin){
    _processor->connectPlugInFromOutside(opid1,plugkeyin);
}

void CProcessorObject::disconnectPlugOutFromOutsideSlot(  COperator::Id  , COperator::PlugKey ){
    //nothing to do
}

void CProcessorObject::disconnectPlugInFromOutsideSlot(  COperator::Id  opid1, COperator::PlugKey plugkeyin){
    _processor->disconnectPlugInFromOutside(opid1,plugkeyin);
}


void CProcessorObject::errorOperator(COperator::Id opid, string msg){
    emit errorOperatorSignal(opid,msg);
}
CProcessorThread *CProcessorObject::getProcessor(){
    return _processor;
}
void CProcessorObject::setTmpPathSlot(string str){
    CMachineSingleton::getInstance()->setTmpPath(str);
}
void CProcessorObject::actionOperator(COperator::Id opid, string actionkey){
    _processor->actionOperator(opid,  actionkey);
}
void CProcessorObject::executedOperatorInClient(COperator * op){
    emit executedOperatorInClientSignal(op);
}

void CProcessorObject::endExecutedOperatorInClient(COperator * op){
    QMetaObject::invokeMethod(this->_processor, "endExec",Q_ARG(int, op->getId()));
}
