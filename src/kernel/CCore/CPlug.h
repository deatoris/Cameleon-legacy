/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CPLUG_H
#define CPLUG_H

#include <vector>
using namespace std;
#include<CData.h>

class COperator;
class CData;



class CPlug
{
public:
    enum State
    {
        NEW,
        OLD,
        EMPTY
    };
    enum INOUT{
        IN,
        OUT
    };

    CPlug(COperator* op, CData::Key datatype,int plugindex);
    virtual ~CPlug();
    COperator * getOperator();
    void setOperator(COperator * operaTor);
    CData* getData();
    void setData(CData* data);
    int getIndex();
    void setState(State state);
    State getState();
    virtual void update()=0;
    virtual void send()=0;
    virtual bool isDataAvailable();
    bool isConnected();
    void connect(CPlug * plug );
    void deconnect();
    INOUT _inout;
    State _state;
    int _index;
    COperator * _operator;
    CData* _data;
    bool _connectedfromoutside;
    bool _connectedwithoperator;
    CPlug *_plug_connected;
};


class CPlugOut;
class CPlugIn : public CPlug
{
public:
    CPlugIn(COperator* op, CData::Key datatype,int plugindex);
    virtual void update();
    virtual void send();
};

class CPlugOut : public CPlug
{
public:
    CPlugOut(COperator * op, CData::Key datatype,int plugindex);
    virtual void update();
    virtual void send();
};


#endif // CPLUG_H
