/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/

#include "COperator.h"
#include "CConnectorDirect.h"
#include <CFactoryData.h>
#include "CException.h"
//#include<CProcessor.h>
#include<CMachine.h>

#include<string>
#include<iostream>
#include <stdio.h>
using namespace std;



vector<pair<CData::Key,string> > & COperatorStructure::plugIn(){
    return this->_plug_in;
}

vector<pair<CData::Key,string> > &COperatorStructure::plugOut(){
    return this->_plug_out;
}

void COperatorStructure::addPlugIn(CData::Key datakey,string info  ){
    this->_plug_in.push_back(make_pair(datakey,info ));
}

void COperatorStructure::addPlugOut(CData::Key datakey ,string info  ){
    this->_plug_out.push_back(make_pair(datakey,info ));
}
void COperatorStructure::addAction( string key){
    _action_keys.push_back(key);
}

vector<string> COperatorStructure::getActionKeys(){
    return _action_keys;
}

COperator::Information COperatorStructure::getInformation(){
    return _information;
}

void COperatorStructure::setInformation(COperator::Information info){
    _information = info;
}

string COperatorStructure::getName(){
    return _name;
}

void COperatorStructure::setName(string name){
    _name = name;
}
string COperatorStructure::getKey(){
    return _key;
}

void COperatorStructure::setKey(string opkey){
    _key = opkey;
}
COperator::COperator()
    :_executedthread(OTHERTHREAD),_error(false)
{
    connect(this,SIGNAL(endExec(int,int)),CMachineSingleton::getInstance()->getProcessor()->getProcessor(),SLOT(endExec(int,int)));
    initState();
}
COperator::~COperator(){
    vector<CPlugOut* >::iterator itout;
    for(itout =_v_plug_out.begin();itout!=_v_plug_out.end();itout++){
        delete *itout;
    }

    vector<CPlugIn* >::iterator itin;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        delete *itin;
    }
}

void COperator::startExec(int opid, int threadid){
    this->exec();
    emit endExec(opid,threadid);
}



void COperator::deconnectAll(){
    vector<CPlugOut* >::iterator itout;
    for(itout =_v_plug_out.begin();itout!=_v_plug_out.end();itout++){
        (*itout)->deconnect();
    }

    vector<CPlugIn* >::iterator itin;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        (*itin)->deconnect();
    }
}
void COperator::setProcessor(CProcessor * processor){
    _processor = processor;
}

COperator::Key COperator::getKey(){
    return _key;
}
void COperator::setKey(Key key){
    _key=key;
    _structure.setKey(key);
}
string COperator::getName(){
    return _name;
}

void COperator::setName(string name){
    _structure.setName(name);
    _name = name;
}
void COperator::setPath(vector<string> path){
    _path = path;
}
vector<string> COperator::getPath(){
    return _path;
}

vector<string>& COperator::path(){
    return _path;
}
COperator::Id COperator::getId(){
    return _id;
}
void COperator::setId(Id id){
    _id  = id;
}
void COperator::error(string error){
    _msg=error;
    _error =true;    
}
string COperator::getErrorMsg(){
    return _msg;
}

bool COperator::getError(){
    return _error;
}
void COperator::setError(bool error){
    _error = error;
}

bool COperator::executionCondition(){

    vector<CPlugOut* >::iterator itout;

    //All ouput plug not NEW
    for(itout =_v_plug_out.begin();itout!=_v_plug_out.end();itout++){
        if((*itout)->getState()==CPlug::NEW&&(*itout)->isConnected()==true){
            return false;
        }
    }

    vector<CPlugIn* >::iterator itin;


    if(_v_plug_in.size()==0)
        return true;
    //All input plug not EMPTY except for an optionnal plug type and at least one at new
    bool atleastonenew=false;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        if((*itin)->getState()==CPlug::EMPTY)
            return false;
        if((*itin)->getState()==CPlug::NEW)
            atleastonenew = true;
    }
    return atleastonenew;
}


void COperator::updateMarkingAfterExecution(){
    vector<CPlugOut* >::iterator itout;
    //All ouput plug becomes NEW
    for(itout =_v_plug_out.begin();itout!=_v_plug_out.end();itout++){
        (*itout)->setState(CPlug::NEW);
        try{
            (*itout)->send();
        }
        catch(CException msg){
            this->error(msg.what());
        }
    }
    //All input plug becomes (NEW,OLD)->OLD
    vector<CPlugIn* >::iterator itin;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        (*itin)->setState(CPlug::OLD);
        (*itin)->send();
    }
    this->updateState();
}

void COperator::updateState(){

    if( this->executionCondition()==true&&this->getState()!=RUNNABLE)//_state != RUNNING &&
    {
        this->setState(RUNNABLE);
        _processor->addOperatorWaitingExecution(_id);
    }
    else
        this->setState(NOTRUNNABLE);

    CMachineSingleton::getInstance()->getProcessor()->getProcessor()->updateOperator(this->getId(),this->getState());
}


void COperator::initState(){
    vector<CPlugOut* >::iterator itout;
    for(itout =_v_plug_out.begin();itout!=_v_plug_out.end();itout++){
        (*itout)->setState(CPlug::EMPTY);
    }
    //All input plug becomes (NEW,OLD)->OLD
    vector<CPlugIn* >::iterator itin;
    for(itin =_v_plug_in.begin();itin!=_v_plug_in.end();itin++){
        if((*itin)->isConnected()==true)
            (*itin)->setState(CPlug::EMPTY);
    }
}
void COperator::action(string ){
//    this->error("You have to implement the member action ");
}

COperatorStructure& COperator::structurePlug(){
    return _structure;
}
void COperator::createPlug(){

    vector<pair<CData::Key,string> > & _plug_in_key =  _structure.plugIn();
    for(int i =0;i<(int)_plug_in_key.size();i++)
    {
        CPlugIn* plug(new CPlugIn(this,_plug_in_key[i].first,i));
        _v_plug_in.push_back(plug);
    }
    vector<pair<CData::Key,string> >& _plug_out_key =  _structure.plugOut();
    for(int i =0;i<(int)_plug_out_key.size();i++)
    {
        CPlugOut* plug(new CPlugOut(this,_plug_out_key[i].first,i));
        _v_plug_out.push_back(plug);
    }
}
vector<CPlugIn* > &COperator::plugIn(){
    return _v_plug_in;
}

vector<CPlugOut* > &COperator::plugOut(){
    return _v_plug_out;
}

void COperator::setState(State state){
    _state = state;
}

COperator::State COperator::getState(){
    return _state;
}
void COperator::setExecutedThread(ExecutedThread state){
    _executedthread = state;
}

COperator::ExecutedThread COperator::getExecutedThread(){
    return _executedthread;
}

void COperator::connectPlugIn(int ){

}

void COperator::connectPlugOut(int ){
}

void COperator::setInformation(Information info){
    _info = info;
    _structure.setInformation(info);
}

COperator::Information COperator::getInformation(){
    return _info;
}

ostream& operator<<(ostream& out, COperator* op)
{
    out<<"My State ";
    if(op->_state==COperator::RUNNING)
        out<<"RUNNING"<<endl;
    else if(op->_state==COperator::NOTRUNNABLE)
        out<<"NOTRUNNABLE"<<endl;
    else
        out<<"RUNNABLE"<<endl;

    for(int i =0;i<(int)op->_v_plug_in.size();i++){
        out<<"\t"<<(op->_v_plug_in[i]);
    }
    for(int i =0;i<(int)op->_v_plug_out.size();i++){
        out<<"\t"<<(op->_v_plug_out[i]);
    }
    return out;
}

