#include "CClient.h"

CClient::CClient()
{
    _mode = IDE;
}
void CClient::registerGlossary(){

}

Mode CClient::getMode(){
    return _mode;
}

void CClient::setMode(Mode mode){
    _mode = mode;
}

CInterfaceClient * CClient::getCInterfaceClient2Server(){
    return _interfaceclient;
}
void CClient::setCInterfaceClient2Server(CInterfaceClient * interfaceclient ){
    _interfaceclient  = interfaceclient;
}

bool CClient::applyStopForEachAction(){
    return false;
}
