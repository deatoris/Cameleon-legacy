/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CCOMMUNICATIONPROCESSORPROXY_H
#define CCOMMUNICATIONPROCESSORPROXY_H
#include<QObject>
#include<CProcessor.h>
#include<CProcessorThread.h>
#include<QTime>
#include<CMode.h>
class CProcessorObject : public QObject
{
    Q_OBJECT
private:
    CProcessorThread  * _processor;
public:
    CProcessorObject();
    ~CProcessorObject();

    enum ModeFilter{
        FILTERING,
        NOFILTERING,
        FILTEREDSEND
    };
    Mode getMode();
    void setMode(Mode mode);


signals:
    void createOperatorReceptionSignal(COperator::Id opid,COperatorStructure   opstructure);
    void updateOperatorSignal(COperator::Id opid,COperator::State state);
    void updateUnitProcessSignal(int unitprocessid,COperator::State state);
    void createUnitProcessSignal(int unitprocessid);
    void updatePlugInSignal(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state);
    void updatePlugOutSignal(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state);
    void errorOperatorSignal(COperator::Id opid,string msg);
    void stopSignal();
    void nextSignal();
    void pauseSignal();

    void updateOperatorStructureSignal(COperator::Id opid,COperatorStructure   opstructure);
    void executedOperatorInClientSignal(COperator * op);

    void progressionExecutionSignal(double ratio);

public slots:

    void clearSlot();
    void startSlot();
    void nextSlot();
    void pauseSlot();
    void stopSlot();
    void killSlot();

    void createOperatorSlot(COperator::Key opkey);
//    void endOperatorReceive(COperator::Id opid);
    void activateOperatorSlot(COperator::Id opid);
    void desactivateOperatorSlot(COperator::Id opid);
    void connectPlugOut2PlugInSlot(  COperator::Id  opid1, COperator::PlugKey plugkey1, COperator::Id opid2, COperator::PlugKey plugkey2);
    void disconnectPlugInSlot(COperator::Id  opid, COperator::PlugKey plugkeyin);
    void connectPlugOutFromOutsideSlot(  COperator::Id  opid1, COperator::PlugKey plugkeyout);
    void connectPlugInFromOutsideSlot(  COperator::Id  opid1, COperator::PlugKey plugkeyin);
    void disconnectPlugOutFromOutsideSlot(  COperator::Id  , COperator::PlugKey );
    void disconnectPlugInFromOutsideSlot(  COperator::Id  opid1, COperator::PlugKey plugkeyin);
    void createUnitProcessSlot();
    void linkUnitProcessSlot(int unitprocessid, int unitprocessmotherid);
    void unlinkUnitProcessSlot(int unitprocessid);
    void addOperatorToUnitProcessSlot(int opid, int unitprocessid );
    void removeOperatorToUnitProcessSlot(int opid );
    void setNbrThreadSlot(int nbrthread);

    void setPlugInSlot(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state);
    void connectListening(COperator::Id opid, COperator::PlugKey plugkeyout);
    void disconnectListening(COperator::Id opid, COperator::PlugKey plugkeyout);
    void setTmpPathSlot(string str);
    void actionOperatorSlot(COperator::Id opid, string actionkey);


    void endExecutedOperatorInClientSlot(COperator * op);
    //From Operator


private:
    ModeFilter _filtersignal;
    Mode _modesignal;
    bool _alllistening;
    int _time_ms;

    vector<vector<bool> > _v_listening_control_plug_out;
    vector<vector<pair<QTime,int> > > _v_listening_plug_out;
    vector<vector<pair<QTime,int> > > _v_listening_plug_in;
    vector<pair<QTime,int> > _v_listening_op;
    vector<pair<QTime,int> > _v_listening_unit_process;
    vector<vector<bool > > _v_listening_plug_in_is_filter;
    vector<vector<bool > > _v_listening_plug_out_is_filter;
    vector<bool  > _v_listening_op_is_filter;
    vector<bool  > _v_listening_unit_process_is_filter;
public:
    //BUILDER the compostion
    virtual void createOperatorReceive(COperator::Key opkey);
//    virtual void endOperatorReceive(COperator::Id opid);
    virtual void createOperatorSend(COperator::Id opid,COperatorStructure   opstructure);
    virtual void activateOperatorReceive(COperator::Id opid);
    virtual void desactivateOperatorReceive(COperator::Id opid);
    virtual void connectPlugOut2PlugInReceive(  COperator::Id  opid1, COperator::PlugKey plugkey1, COperator::Id opid2, COperator::PlugKey plugkey2);
    virtual void disconnectPlugInReceive(COperator::Id  opid, COperator::PlugKey plugkeyin);
    virtual void clearReceive();
    virtual void createUnitProcess();
    virtual void linkUnitProcess(int unitprocessid, int unitprocessmotherid);
    virtual void unlinkUnitProcess(int unitprocessid);
    virtual void addOperatorToUnitProcess(int opid , int unitprocessid);
    virtual void removeOperatorToUnitProcess(int opid );
    virtual void setNbrThread(int nbrthread);

    virtual void updateOperatorStructure(COperator::Id opid,COperatorStructure   opstructure);


    //PLAYER the composition
    virtual void startReceive();
    virtual void nextReceive();
    virtual void nextSend();
    virtual void pauseReceive();
    virtual void pauseSend();
    virtual void stopReceive();
    virtual void stopSend();
    virtual void killReceive();


    //INTERACTION client server
    virtual void setPlugInReceive(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state);
    virtual void updateOperator(COperator::Id opid,COperator::State state);
    virtual void updateUnitProcess(int unitprocessid,COperator::State state);
    virtual void updatePlugIn(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state);
    virtual void updatePlugOut(COperator::Id opid,COperator::PlugKey plugkeyn, CData * data, CPlug::State state);
    virtual void errorOperator(COperator::Id opid,string msg);
    virtual void actionOperator(COperator::Id opid, string actionkey);
    virtual void executedOperatorInClient(COperator * op);
    virtual void endExecutedOperatorInClient(COperator * op);
    virtual void progressionExecution(double ratio);
    //Filter signal
    void setFilterSignal(ModeFilter filtersignal);
    ModeFilter getFilterSignal();


    CProcessorThread * getProcessor();
    QMutex _mutex;
};

#endif // CCOMMUNICATIONPROCESSORPROXY_H
