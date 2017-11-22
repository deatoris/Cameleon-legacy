#ifndef CINTERFACECLIENT2SERVERLOCAL_H
#define CINTERFACECLIENT2SERVERLOCAL_H

#include<CInterfaceClient.h>
#include<QObject>
#include<CControl.h>
#include<CProcessorObject.h>
class Compare
{
public:
    bool operator()(const pair<int,int> & x,const pair<int,int> & y);
};

class CInterfaceClient : public QObject
{
    Q_OBJECT
private:
    COperator::Id _opid;
    COperatorStructure  _opstructure;
    QMutex _mutex;
    QWaitCondition _condition;
    vector<pair<bool, pair<int,pair<int, pair<CControl *,int> > > > > _v_control;
    bool _alllistening;
    bool _isstop;
    int _unitprocessid;
signals:
    void createOperatorSignal(COperator::Key opkey);
//    void EndCreateOperatorSignal(COperator::Id opid);
    void activateOperatorSignal(COperator::Id opid);
    void desactivateOperatorSignal(COperator::Id opid);
    void connectPlugOut2PlugInSignal(  COperator::Id  opid1, COperator::PlugKey plugkey1, COperator::Id opid2, COperator::PlugKey plugkey2);
    void disconnectPlugInSignal(COperator::Id  opid, COperator::PlugKey plugkeyin);

    void connectPlugOutFromOutsideSignal(  COperator::Id  opid1, COperator::PlugKey plugkeyout);
    void connectPlugInFromOutsideSignal(  COperator::Id  opid1, COperator::PlugKey plugkeyin);
    void disconnectPlugOutFromOutsideSignal(  COperator::Id  opid1, COperator::PlugKey plugkeyout);
    void disconnectPlugInFromOutsideSignal(  COperator::Id  opid1, COperator::PlugKey plugkeyin);
    void setNbrThreadSignal(int opid );

    void createUnitProcessSignal();
    void linkUnitProcessSignal(int unitprocessid, int unitprocessmotherid);
    void unlinkUnitProcessSignal(int unitprocessid);
    void addOperatorToUnitProcessSignal(int opid, int unitprocessid );
    void removeOperatorToUnitProcessSignal(int opid );

    void clearSignal();
    void startSignal();
    void nextSignal();
    void pauseSignal();
    void stopSignal();
    void killSignal();
    void setPlugInSignal(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state);

    void connectListening(COperator::Id opid, COperator::PlugKey plugkeyout);
    void disconnectListening(COperator::Id opid, COperator::PlugKey plugkeyout);
    void setTmpPathSignal(string str);

    void actionOperatorSignal(COperator::Id opid, string actionkey);
    void endExecutedOperatorInClientSignal(COperator * op);
public slots:
    void createOperatorReceptionSlot(COperator::Id opid,COperatorStructure   opstructure);
    void updateOperatorSlot(COperator::Id opid,COperator::State state);
    void updateUnitProcessSlot(int unitprocessid,COperator::State state);
    void updatePlugInSlot(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state);
    void updatePlugOutSlot(COperator::Id opid,COperator::PlugKey plugkeyn, CData * data, CPlug::State state);
    void updateOperatorStructureSlot(COperator::Id opid,COperatorStructure   opstructure);

    void errorOperatorSlot(COperator::Id opid,string msg);
    void createUnitProcessSlot(int unitprocessmotherid);
    void stopSlot();
    void nextSlot();
    void pauseSlot();
    void executedOperatorInClientSlot(COperator * op);
    void progressionExecutionSlot(double ratio);

public:
    //CInterfaceClient2ServerLocal(CReceptionClient * receptionclient);
    CInterfaceClient(bool alllistening);
    void launchSignal();

    //INIT the communication
    virtual void connectWithServer2(CProcessorObject * processor);

    //BUILDER the compostion
    virtual void createOperatorSend(COperator::Key opkey,COperator::Id &opid,COperatorStructure &  opstructure);
//    virtual void EndCreateOperatorSend(COperator::Id opid);
    virtual void activateOperatorSend(COperator::Id opid);
    virtual void desactivateOperatorSend(COperator::Id opid);
    virtual void connectPlugOut2PlugInSend(  COperator::Id  opid1, COperator::PlugKey plugkeyout, COperator::Id opid2, COperator::PlugKey plugkeyin);
    virtual void disconnectPlugInSend(COperator::Id  opid, COperator::PlugKey plugkeyin);

    virtual void connectPlugOutFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyout);
    virtual void connectPlugInFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyin);
    virtual void disconnectPlugOutFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyout);
    virtual void disconnectPlugInFromOutsideSend(  COperator::Id  opid1, COperator::PlugKey plugkeyin);
    virtual void setNbrThreadSend(int nbrthread );
    virtual void clearSend();

    virtual void createUnitProcessSend(int& unitprocessid);
    virtual void linkUnitProcess(int unitprocessid, int unitprocessmotherid);
    virtual void unlinkUnitProcess(int unitprocessid);
    virtual void addOperatorToUnitProcessSend(int opid , int unitprocessid);
    virtual void removeOperatorToUnitProcessSend(int opid );

    //PLAYER the composition
    virtual void startSend();
    virtual void nextSend();
    virtual void nextReceive();
    virtual void pauseSend();
    virtual void pauseReceive();
    virtual void stopSend();
    virtual void stopReceive();
    virtual void killSend();

    //INTERACTION client server
    virtual void setPlugInSend(COperator::Id opid, COperator::PlugKey plugkeyin, CData * data, CPlug::State state);
    virtual void updateOperatorReceive(COperator::Id opid,COperator::State state);
    virtual void updateUnitProcessReceive(int unitprocessid,COperator::State state);
    virtual void updatePlugInReceive(COperator::Id opid,COperator::PlugKey plugkey, CData * data, CPlug::State state);
    virtual void updatePlugOutReceive(COperator::Id opid,COperator::PlugKey plugkeyn, CData * data, CPlug::State state);
    virtual   void updateOperatorStructureReceive(COperator::Id opid,COperatorStructure   opstructure);
     virtual   void progressionExecution(double ratio);
    //LISTENER
    virtual int addListenerPlugOutOperator2PlugInControl(COperator::Id opid,COperator::PlugKey plugkeyn,CControl * control,CControl::PlugKey plugkeycontrol);
    virtual void removeListenerPlugOutOperator2PlugInControl(int controlid);

    //Set
    virtual void setTmpPath(string str);

    virtual void actionOperator(COperator::Id opid, string actionkey);
    virtual void executedOperatorInClient(COperator * op);
};

#endif // CINTERFACECLIENT2SERVERLOCAL_H
