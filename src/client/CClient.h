#ifndef CCLIENT_H
#define CCLIENT_H
#include<QString>
#include<vector>
#include<CData.h>
using namespace std;
#include<QWidget>
#include<CInterfaceClient.h>
#include<CSingleton.h>
#include<QThread>
#include<CMode.h>



class CClient //: public QThread
{
signals:
    //send to CGlossary
    void loadDictionnaySignal(QString path);
//    void registerOperatorsByNameInSingleton();
//    void registerDatasInSingleton();
//    void registerConnectorsInSingleton();

    //send to CGlossary
    CInterfaceClient * _interfaceclient;
public:




    void registerGlossary();
    //QWidget * getWidgetGlossaryManager();

    CInterfaceClient * getCInterfaceClient2Server();
    void setCInterfaceClient2Server(CInterfaceClient * interfaceclient );
    CClient();

    Mode getMode();
    void setMode(Mode mode);

    bool applyStopForEachAction();

private:
    Mode _mode;
};

typedef CSingleton<CClient> CClientSingleton;


#endif // CCLIENT_H
