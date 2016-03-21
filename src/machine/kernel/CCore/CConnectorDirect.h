/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CCONNECTORDIRECT_H
#define CCONNECTORDIRECT_H

#include<CError.h>
#include<CData.h>

#include<CPlug.h>

class CConnectorReceiverDirectPlug2Plug;




class CConnectorSenderDirectPlug2Plug //: public CConnectorSender
{
public:
    CConnectorSenderDirectPlug2Plug(CPlug* plug);
    ~CConnectorSenderDirectPlug2Plug();
    void connectReceiver(CConnectorReceiverDirectPlug2Plug * connectorreceiver ) ;
    void deconnect();
    void sendData(CData* data,CPlug::State plug)throw(std::string);
    CConnectorReceiverDirectPlug2Plug *  getConnectorReceiverPlug2Plug();
    void  setConnectorReceiverPlug2Plug(CConnectorReceiverDirectPlug2Plug * connectorreceiver);
    CPlug* getPlug();
    bool isConnected();
    void setConnected(bool connected);

    static void connect(CConnectorSenderDirectPlug2Plug * sender, CConnectorReceiverDirectPlug2Plug * receiver)throw(CErrorBuilder);
    static void deconnect(CConnectorSenderDirectPlug2Plug * sender, CConnectorReceiverDirectPlug2Plug * receiver)throw(CErrorBuilder);
private:
    CConnectorReceiverDirectPlug2Plug *  _connectorreceiver;
    CPlug* _plug;
    bool _connected;
};

class CConnectorReceiverDirectPlug2Plug//: public CConnectorReceiver
{
public:
    CConnectorReceiverDirectPlug2Plug(CPlug * plug);
    virtual ~CConnectorReceiverDirectPlug2Plug();
    virtual void connectSender(CConnectorSenderDirectPlug2Plug * connectorsender );
    virtual void deconnect();
    virtual void receiveData(CData* data,CPlug::State plug)=0;


    CConnectorSenderDirectPlug2Plug *  getConnectorSenderDirectPlug2Plug();
    void  setConnectorSenderDirectPlug2Plug(CConnectorSenderDirectPlug2Plug * connectorsender);
    CPlug* getPlug();

protected:
    CConnectorSenderDirectPlug2Plug *_connectorsender;
    CPlug* _plug;
};


class CConnectorReceiverDirectPlugOut2PlugIn: public CConnectorReceiverDirectPlug2Plug
{
public:
    CConnectorReceiverDirectPlugOut2PlugIn(CPlug * plug);
    virtual ~CConnectorReceiverDirectPlugOut2PlugIn();
    virtual void receiveData(CData* data,CPlug::State plug)throw(std::string);
};

class CConnectorReceiverDirectPlugIn2PlugOut: public CConnectorReceiverDirectPlug2Plug
{
public:
    CConnectorReceiverDirectPlugIn2PlugOut(CPlug * plug);
    virtual ~CConnectorReceiverDirectPlugIn2PlugOut();
    virtual void receiveData(CData* data,CPlug::State plug);
};
//class CPlug;

//class CConnectorReceiverDirectPlugOut2PlugIn;




//class CConnectorSenderDirectPlug2Plug : public CConnectorSender
//{
//public:
//    CConnectorSenderDirectPlug2Plug(CPlug* plug);
//    ~CConnectorSenderDirectPlug2Plug();
//    void connectReceiver(CConnectorReceiver * connectorreceiver );
//    void deconnect();
//    void sendData(CData* data,CPlug::PlugState plug);
//    CConnectorReceiver *  getConnectorReceiver();
//private:
//    CConnectorReceiver *  _connectorreceiver;
//        CPlug* _plug;
//};


//class CConnectorReceiverDirectPlugOut2PlugIn: public CConnectorReceiver
//{
//public:
//    CConnectorReceiverDirectPlugOut2PlugIn(CPlug * plug);
//    ~CConnectorReceiverDirectPlugOut2PlugIn();
//    virtual void connectSender(CConnectorSender * connectorsender );
//    virtual void deconnect();
//    virtual void receiveData(CData* data,CPlug::PlugState plug);

////    int getIndexPlug();
//private:
//    CConnectorSender *_connectorsender;
//    CPlug* _plug;
//};

//class CConnectorReceiverDirectPlugIn2PlugOut: public CConnectorReceiver
//{
//public:
//    CConnectorReceiverDirectPlugIn2PlugOut(CPlug* plug);
//    ~CConnectorReceiverDirectPlugIn2PlugOut();
//    virtual void connectSender(CConnectorSender * connectorsender);
//    virtual void deconnect();
//    virtual void receiveData(CData* data,CPlug::PlugState plug);
//private:
//    CConnectorSender * _connectorsender;
//    CPlug* _plug;
//};
#endif // CCONNECTORDIRECT_H
