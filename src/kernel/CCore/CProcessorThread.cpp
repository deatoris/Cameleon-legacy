/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CProcessorThread.h"

#include<CMachine.h>
#include<algorithm>
#include<CError.h>
#include<COperatorThread.h>
#include<CLogger.h>
#define MAX_THREAD 16
CProcessorThread::CProcessorThread()
    :_nbr_thread_running(0),_nbrthread(1)
{
    connect(this,SIGNAL(tryExecutedOperatorSignal()),this,SLOT(tryExecutedOperatorSlot()));
    for(int i=0;i<MAX_THREAD;i++){

//        COperatorThread * op = new COperatorThread;
//        _v_object.push_back(op);
        QThread* thread = new QThread;
        thread->start();
        _v_thread.push_back(thread);
        _v_thread_running.push_back(false);
    }
}
void CProcessorThread::setNbrThread(int ){
    //TODO set with the right number of thread
    _nbrthread = 1;
}
void CProcessorThread::endExec(int opid,int threadid){
//    string stt="End execute op "+_compo[opid].first->getKey() +" with id"+pop::ConvertString::Any2String(opid);
//    CLogger::getInstance()->log("MACHINE",CLogger::INFO,stt.c_str());
    //    this->_compo[opid].first->moveToThread(CMachineSingleton::getInstance());
    _nbr_thread_running--;
    _v_thread_running[threadid]=false;
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
    }
    if(_compo[opid].first->getError()==true){
        CMachineSingleton::getInstance()->getProcessor()->errorOperator(opid,_compo[opid].first->getErrorMsg());
        _compo[opid].first->setError(false);
    }
    if(this->getPlayer()==STOP&&_nbr_thread_running==0){
        endExecutingLoop();
    }
    else if(this->getPlayer()==PAUSE&&_nbr_thread_running==0){
        CMachineSingleton::getInstance()->getProcessor()->pauseSend();
    }
    else if(this->getPlayer()==START){
        emit tryExecutedOperatorSignal();
    }
    else if(this->getPlayer()==NEXT){
        CMachineSingleton::getInstance()->getProcessor()->nextSend();
    }
}
void CProcessorThread::endExec(int opid){
//    string stt="End execute op "+_compo[opid].first->getKey() +" with id"+pop::ConvertString::Any2String(opid);
//    CLogger::getInstance()->log("MACHINE",CLogger::INFO,stt.c_str());
    _nbr_thread_running--;
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
    }
    if(_compo[opid].first->getError()==true){
        CMachineSingleton::getInstance()->getProcessor()->errorOperator(opid,_compo[opid].first->getErrorMsg());
        _compo[opid].first->setError(false);
    }
    if(this->getPlayer()==STOP&&_nbr_thread_running==0){
        endExecutingLoop();
    }
    else if(this->getPlayer()==PAUSE&&_nbr_thread_running==0){
        CMachineSingleton::getInstance()->getProcessor()->pauseSend();
    }
    else if(this->getPlayer()==START){
        emit tryExecutedOperatorSignal();
    }
    else if(this->getPlayer()==NEXT){
        CMachineSingleton::getInstance()->getProcessor()->nextSend();
    }
}
void CProcessorThread::tryExecutedOperatorSlot(){
    if(this->getPlayer()==START&&_nbr_thread_running<_nbrthread){
        int idop = getOperator();
        if(idop==-1 && _nbr_thread_running==0)
            this->endExecutingLoop();
        else if(idop!=-1)
            executeOperator(idop);
    }

}

bool CProcessorThread::executeOperator(int opid){
    if(_compo[opid].first->getError()==true){
        CMachineSingleton::getInstance()->getProcessor()->errorOperator(opid,_compo[opid].first->getErrorMsg());
        _compo[opid].first->setError(false);
    }
    else {
        if(this->_compo[opid].second==ALIVE &&  this->_compo[opid].first->executionCondition()==true){
//            string stt="Execute op "+_compo[opid].first->getKey() +" with id"+pop::ConvertString::Any2String(opid);
//            CLogger::getInstance()->log("MACHINE",CLogger::INFO,stt.c_str());
            _nbr_thread_running++;
            this->_compo[opid].first->setState(COperator::RUNNING);
            CMachineSingleton::getInstance()->getProcessor()->updateOperator(opid,COperator::RUNNING);

            if(this->_compo[opid].first->getExecutedThread()==COperator::OTHERTHREAD){
                for(int threadid =0;threadid<_nbrthread;threadid++){
                    if(_v_thread_running[threadid]==false){
                        _v_thread_running[threadid]=true;


//                        _v_object[threadid]->setOperator(this->_compo[opid].first);
//                        _v_object[threadid]->moveToThread(_v_thread[threadid]);
//                        QMetaObject::invokeMethod(_v_object[threadid], "startExec",Q_ARG(int, opid),Q_ARG(int,threadid));
                        this->_compo[opid].first->moveToThread(_v_thread[threadid]);
                        QMetaObject::invokeMethod(this->_compo[opid].first, "startExec",Q_ARG(int, opid),Q_ARG(int,threadid));
                        threadid=_nbrthread;
                    }

                }
            }
            else{
                CMachineSingleton::getInstance()->getProcessor()->executedOperatorInClient(this->_compo[opid].first);
            }
        }
        emit tryExecutedOperatorSignal();
    }
    return true;
}
void CProcessorThread::executingLoop(){

    emit tryExecutedOperatorSignal();
}
bool CProcessorThread::endExecutingLoop(){
    this->actualizeState();
    if(this->getPlayer()==STOP)
    {
        CProcessor::stop();
        CMachineSingleton::getInstance()->getProcessor()->stopSend();
    }
    return true;
}
void CProcessorThread::start(){
    this->setPlayer(START);
    this->executingLoop();
}

void CProcessorThread::stop(){
    //    cout<<"[SERVER] stop"<<endl;
    this->setPlayer(STOP);
    if(_nbr_thread_running==0){
        CProcessor::stop();
        CMachineSingleton::getInstance()->getProcessor()->stopSend();
    }
}
void CProcessorThread::pause(){
    //    cout<<"[SERVER] stop"<<endl;
    this->setPlayer(PAUSE);
    if(_nbr_thread_running==0){
        CMachineSingleton::getInstance()->getProcessor()->pauseSend();
    }
}
