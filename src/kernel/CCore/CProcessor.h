/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CPROCESSOR_H
#define CPROCESSOR_H

#include<COperator.h>
#include<CPlug.h>

#include<queue>
//#include<QMutex>
class CProcessor
{
public:


    enum Player{
        START,
        STOP,
        PAUSE,
        NEXT
    };
    enum OpState{
        ALIVE,
        DEAD
    };
    CProcessor();
    ~CProcessor();


    virtual void setNbrThread(int nbrthread);
    void addOperatorWaitingExecution(int opid);

    virtual void initialisation();
    virtual bool executeOperator(int opid);
    virtual void executingLoop();
    virtual bool executingLoopOneEvent();
    void setPlayer(Player state);
    Player getPlayer();
    void actualizeState();

    //Interface with the rest
    virtual void start();
    virtual void pause();
    virtual void next();
    virtual void stop();
    virtual void kill();
    COperator::Id newOperator(COperator::Key opkey,COperatorStructure & structure);
    void activateOperator(COperator::Id opid);
    void desactivateOperator(COperator::Id opid);
    void connectPlugOut2PlugIn(COperator::Id idop1,int plugindex1, COperator::Id idop2,int plugindex2);
    void deconnectPlugIn(COperator::Id idop,int plugindex);

    void connectPlugInFromOutside(  COperator::Id  opid1, COperator::PlugKey plugkeyin);
    void disconnectPlugInFromOutside(  COperator::Id  opid1, COperator::PlugKey plugkeyin);

    void deconnectPlugOut(COperator::Id idop,int plugindex);
    void clearComposition();
    void setPlugIn(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state);
    typedef vector<pair<COperator *,OpState> > CComposition;
    CComposition&  composition();
    friend ostream & operator<<( ostream & os, const CProcessor& v);
    //virtual bool operatorInExecution();
    void clear();
    int getOperator();
    int newUnitProcessor();
    void linkUnitProcess(int unitprocessid, int unitprocessmotherid);
    void unlinkUnitProcess(int unitprocessid);
    void addOperatorToUnitProcess(int idop,int idunitprocess);
    void removeOperatorToUnitProcess(int idop);

    void updateOperator(COperator::Id opid,COperator::State state);
    void actionOperator(COperator::Id opid, string actionkey);
    bool isStartSleeping();
 protected:
    vector<std::queue<int> > _process_stack;
    vector<bool> _process_waiting_execution;
    vector<int> _process_mother;
    int _current_process;
    vector<int> _id_to_unit_process;

    CComposition _compo;

    Player _player;


};


#endif // CPROCESSOR_H
