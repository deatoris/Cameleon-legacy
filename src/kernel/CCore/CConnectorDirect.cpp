/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CConnectorDirect.h"
#include <CData.h>
#include "COperator.h"
CConnectorSenderDirectPlug2Plug::CConnectorSenderDirectPlug2Plug(CPlug * plug)
    :_plug(plug),_connected(false)
{}
CConnectorSenderDirectPlug2Plug::~CConnectorSenderDirectPlug2Plug()
{
    //this->deconnect();
}
bool CConnectorSenderDirectPlug2Plug::isConnected(){
    return _connected;
}
void CConnectorSenderDirectPlug2Plug::setConnected(bool connected){
    _connected =connected;
}
void CConnectorSenderDirectPlug2Plug::connectReceiver(CConnectorReceiverDirectPlug2Plug* connectorreceiver ){
    connect(this,connectorreceiver);
}
void CConnectorSenderDirectPlug2Plug::connect(CConnectorSenderDirectPlug2Plug * sender, CConnectorReceiverDirectPlug2Plug * receiver)throw(CErrorBuilder)
{
    if(sender->isConnected()==false)
    {

//        if(sender->getPlug()->getData()->getKey()!=receiver->getPlug()->getData()->getKey()
//                &&sender->getPlug()->getData()->getKey()!= DataGeneric::KEY
//                &&receiver->getPlug()->getData()->getKey()!= DataGeneric::KEY)
//        {
//            throw CErrorBuilder("Connot connected data type, " +
//                                sender->getPlug()->getData()->getKey()+
//                                " with data type, " +
//                                receiver->getPlug()->getData()->getKey(),
//                                "CConnectorDirect.cpp");
//            return;
//        }
        sender->setConnectorReceiverPlug2Plug(receiver);
        receiver->setConnectorSenderDirectPlug2Plug(sender);
        sender->setConnected(true);
    }


    else
    {
        throw CErrorBuilder("Connector already connected","CConnectorDirect.cpp");
    }
}
void CConnectorSenderDirectPlug2Plug::deconnect(CConnectorSenderDirectPlug2Plug * sender, CConnectorReceiverDirectPlug2Plug * )throw(CErrorBuilder)
{
    if(sender->isConnected()==true)
    {
        sender->setConnected(false);
    }
    else
    {
        throw CErrorBuilder("Connector already deconnected","CConnectorDirect.cpp");
    }

}



void CConnectorSenderDirectPlug2Plug::deconnect(){

    CConnectorSenderDirectPlug2Plug::deconnect(this,this->getConnectorReceiverPlug2Plug());

}

void CConnectorSenderDirectPlug2Plug::sendData(CData* data,CPlug::State plug)throw(std::string){
    if(this->isConnected()==true)
        this->_connectorreceiver->receiveData(data,plug);
}
CPlug* CConnectorSenderDirectPlug2Plug::getPlug(){
    return _plug;
}
CConnectorReceiverDirectPlug2Plug *  CConnectorSenderDirectPlug2Plug::getConnectorReceiverPlug2Plug(){
    return _connectorreceiver;
}
void  CConnectorSenderDirectPlug2Plug::setConnectorReceiverPlug2Plug(CConnectorReceiverDirectPlug2Plug * connectorreceiver){
    _connectorreceiver = connectorreceiver;
}



CConnectorReceiverDirectPlug2Plug::CConnectorReceiverDirectPlug2Plug(CPlug * plug)
    :_plug(plug)
{}
CConnectorReceiverDirectPlug2Plug::~CConnectorReceiverDirectPlug2Plug()
{
    //CConnectorSenderDirectPlug2Plug::deconnect(this->getConnectorSenderDirectPlug2Plug(),this);
}
void CConnectorReceiverDirectPlug2Plug::connectSender(CConnectorSenderDirectPlug2Plug* connectorsender ){

    CConnectorSenderDirectPlug2Plug::connect(connectorsender,this);


}

void CConnectorReceiverDirectPlug2Plug::deconnect(){
    CConnectorSenderDirectPlug2Plug::deconnect(this->getConnectorSenderDirectPlug2Plug(),this);

}
CPlug* CConnectorReceiverDirectPlug2Plug::getPlug(){
    return _plug;
}

CConnectorSenderDirectPlug2Plug *  CConnectorReceiverDirectPlug2Plug::getConnectorSenderDirectPlug2Plug(){
    return _connectorsender;
}

void  CConnectorReceiverDirectPlug2Plug::setConnectorSenderDirectPlug2Plug(CConnectorSenderDirectPlug2Plug * connectorsender){
    _connectorsender = connectorsender;
}




CConnectorReceiverDirectPlugOut2PlugIn::CConnectorReceiverDirectPlugOut2PlugIn(CPlug * plug)
    :CConnectorReceiverDirectPlug2Plug(plug)
{}
CConnectorReceiverDirectPlugOut2PlugIn::~CConnectorReceiverDirectPlugOut2PlugIn()

{
}

void CConnectorReceiverDirectPlugOut2PlugIn::receiveData(CData* data,CPlug::State state)throw(std::string)
{
//TODO
    _plug->getData()->dumpReception(data);
    _plug->setState(state);
    _plug->updateStateOperator();
}

CConnectorReceiverDirectPlugIn2PlugOut::CConnectorReceiverDirectPlugIn2PlugOut(CPlug * plug)
    :CConnectorReceiverDirectPlug2Plug(plug)
{}
CConnectorReceiverDirectPlugIn2PlugOut::~CConnectorReceiverDirectPlugIn2PlugOut()

{
}
void CConnectorReceiverDirectPlugIn2PlugOut::receiveData(CData* ,CPlug::State state){

    _plug->setState(state);
    _plug->updateStateOperator();

}


