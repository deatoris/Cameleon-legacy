/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef COPERATOR_H
#define COPERATOR_H

#include <utility>
using namespace std;
#include<CPlug.h>

#include<QObject>


class COperatorStructure
{
public:
    vector<pair<CData::Key,string> > & plugIn();
    vector<pair<CData::Key,string> > & plugOut();
    void addPlugIn(CData::Key datakey,string info  );
    void addPlugOut(CData::Key datakey,string info  );
    void addAction( string key);
    vector<string> getActionKeys();
    string getInformation();
    void setInformation(string info);
    string getKey();
    void setKey(string opkey);
    string getName();
    void setName(string name);
private:
    vector<string> _action_keys;
    string _information;
    string _key;
    string _name;
    vector<pair<CData::Key,string> > _plug_in;
    vector<pair<CData::Key,string> > _plug_out;
};

class CProcessor;
class COperatorStructure;


class COperator       :public QObject
{
    Q_OBJECT
signals:
    void endExec(int opid,int threadid);
public slots:
    void startExec(int opid, int threadid);


public:
    typedef int Id;
    typedef string Key;
    typedef int PlugKey;
    enum State {
        RUNNING, RUNNABLE, NOTRUNNABLE
    };
    enum ExecutedThread{
        OTHERTHREAD,
        MACHINE,
        CLIENT
    };

    typedef string Information;


    COperator();
    virtual ~COperator();
    virtual void exec()=0;
    virtual COperator * clone()=0;
    virtual bool executionCondition();
    virtual void updateMarkingAfterExecution();
    virtual void initState();
    virtual void action(string key);

    void updateState();
    Key getKey();
    void setKey(Key opkey);
    string getName();
    void setName(string name);
    void setPath(vector<string> path);
    vector<string> getPath();
    vector<string> &path();
    void setInformation(Information info);
    Information getInformation();
    Id getId();
    void setId(Id id);

    COperatorStructure& structurePlug();
    void createPlug();
    void deconnectAll();
    void setState(State state);
    State getState();
    void setExecutedThread(ExecutedThread state);
    ExecutedThread getExecutedThread();

    void setProcessor(CProcessor * processor);

    vector<CPlugIn* > & plugIn();
    vector<CPlugOut* > & plugOut();
    friend ostream& operator<<(ostream& out, COperator* op);
    bool getError();
    void setError(bool error);
    void error(string error);
    string getErrorMsg();
    virtual void connectPlugIn(int index);
    virtual void connectPlugOut(int index);
protected:
    vector<CPlugIn *> _v_plug_in;
    vector<CPlugOut*> _v_plug_out;
    State _state;
    ExecutedThread _executedthread;
    bool _inexecution;
    COperatorStructure _structure;
    Information _info;
    vector<string> _path;
    string _name;
    Key _key;
    Id _id;

    CProcessor *_processor;

    //TODO in the next of version of cameleon throw exection
    bool _error;
    string _msg;
};





#endif // COPERATOR_H
