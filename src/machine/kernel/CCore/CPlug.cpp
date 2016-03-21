/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CPlug.h"
#include<CGlossary.h>
#include"COperator.h"
#include<CConnectorDirect.h>

#include<CMachine.h>

CPlug::CPlug(COperator* op,  CData::Key  typedata,int plugindex)
    :_state(EMPTY),_index(plugindex),_operator(op),_data(NULL),_connectedfromoutside(false),_connectedwithoperator(false),_plug_connected(NULL)
{
    CData * d = CGlossarySingletonServer::getInstance()->createData(typedata);
    d->setPlug(this);
    this->setData(d);
}
CPlug::~CPlug()
{
    if(_data!=NULL)
        delete _data;
}
COperator * CPlug::getOperator(){
    return _operator;
}
void CPlug::setOperator(COperator* op){
    _operator = op;
}
int CPlug::getIndex(){
    return _index;
}

CData* CPlug::getData(){
    return _data;
}
void CPlug::setData(CData* data){
    if(_data!=NULL&&data!=_data)
        delete _data;
    _data = data;
}



void CPlug::setState(State state){
    _state = state;
    update();
}


bool CPlug::isDataAvailable(){
    return _data->isDataAvailable();
}

bool CPlug::isConnected(){
    if(_connectedfromoutside==true)
        return true;
    else
        return _connectedwithoperator;
}

CPlug::State CPlug::getState(){
    return _state;
}








void CPlug::connect(CPlug *plug ){
    this->_plug_connected = plug;
    this->_connectedwithoperator=true;
    this->_plug_connected->_plug_connected = this;
    this->_plug_connected->_connectedwithoperator =true;
}
void CPlug::deconnect(){
    if(this->_connectedwithoperator){
        this->_connectedwithoperator=false;
        this->_plug_connected->_connectedwithoperator =false;
        if(_inout==IN){
            this->setState(EMPTY);
            if(this->_plug_connected->getState()!=EMPTY)
                this->_plug_connected->setState(NEW);
        }else{
            this->_plug_connected->setState(EMPTY);
            if(this->getState()!=EMPTY)
                this->setState(NEW);
        }


        this->_plug_connected->_plug_connected = NULL;
        this->_plug_connected=NULL;
    }
}





CPlugIn::CPlugIn(COperator* op,  CData::Key  typedata,int plugindex)
    :CPlug(op, typedata, plugindex)
{
    _inout = IN;
}


CPlugOut::CPlugOut(COperator* op,  CData::Key  typedata,int plugindex)
    :CPlug(op, typedata, plugindex)
{
    _inout = OUT;
}
void CPlugIn::send(){
    if(this->_connectedwithoperator){
        _plug_connected->setState(_state);
        _plug_connected->_operator->updateState();
    }
}

void CPlugOut::send(){
    if(this->_connectedwithoperator){
        _plug_connected->getData()->dumpReception(this->getData());
        _plug_connected->setState(_state);
        _plug_connected->_operator->updateState();
    }
}
void CPlugOut::update(){
    CMachineSingleton::getInstance()->getProcessor()->updatePlugOut(this->getOperator()->getId(),this->getIndex(),this->getData(),this->getState());

}void CPlugIn::update(){
    CMachineSingleton::getInstance()->getProcessor()->updatePlugIn(this->getOperator()->getId(),this->getIndex(),this->getData(),this->getState());

}
