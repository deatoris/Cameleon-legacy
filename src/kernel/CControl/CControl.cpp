/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CControl.h"
#include<CClient.h>
int CControl::_idglobal=0;
CControl::CControl(QWidget * parent)
    : QWidget(parent)
{
    _id=_idglobal;
    _idglobal++;
    dataout="";
    //fixed size?
    //
    //    QHBoxLayout* layout = new QHBoxLayout();
    //    this->setLayout(layout);
    //    layout->addWidget(new QTextEdit());
    //    layout->addWidget(new QPushButton("ok"));
    menu = 0;
    //    this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

}

QMenu* CControl::getMenu(){
    return menu;
}

int CControl::getId(){
    return _id;
}
void CControl::createPlug(){
    _v_plug_in.resize(_structure.plugIn().size(),make_pair(false,0));
    _v_plug_in_firstime.resize(_structure.plugIn().size(),true);

    _v_plug_in_processor.resize(_structure.plugIn().size(),make_pair(false,make_pair(0,0)));
    _v_plug_out.resize(_structure.plugOut().size(),make_pair(false,make_pair(0,0)));
}


void CControl::connectPlugOut(int myindexplugout, COperator::Id opid, int plugin){
    ////qDebug<<"CControl ConnectPlug out"<<opid<<" plug id "<<plugin<<" myplug index out"<<myindexplugout<<endl;
    //getchar();
    if(_v_plug_out[myindexplugout].first == false){
        _v_plug_out[myindexplugout].first = true;
        _v_plug_out[myindexplugout].second.first  = opid;
        _v_plug_out[myindexplugout].second.second  = plugin;
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->connectPlugInFromOutsideSend(opid,plugin);

    }
    else{
        ////qDebug<<"[ERROR] Plug out already connected for the control"<<endl;
        //getchar();
    }


}
void CControl::apply(){

}
vector<string> & CControl::path(){
    return _path;
}

void CControl::deconnectPlugOut(int myindexplugout){
    ////qDebug<<"CControl deConnectPlug out "<< myindexplugout<<endl;
    if(_v_plug_out[myindexplugout].first == true){
        _v_plug_out[myindexplugout].first = false;
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->disconnectPlugInFromOutsideSend(_v_plug_out[myindexplugout].second.first,_v_plug_out[myindexplugout].second.second);
    }
    else{
        ////qDebug<<"[ERROR] Plug out already deconnected for the control"<<endl;
        //getchar();
    }
}

void CControl::sendPlugOutControl(int indexplugout,CData* data,CPlug::State state){
    //#107 - un control est adapté uniquement s'il n'a qu'un seul connecteur out
    if(indexplugout==0) dataout = data->toString().c_str();
    if(_v_plug_out[indexplugout].first == true){
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->setPlugInSend(_v_plug_out[indexplugout].second.first,_v_plug_out[indexplugout].second.second,data,state);
    }
    else{
        ////qDebug<<"[ERROR][SEND] Plug out is not connected"<<endl;
    }
}
bool CControl::isPlugOutConnected(int indexplugout){
    return _v_plug_out[indexplugout].first;
}


void CControl::connectPlugIn(int myindexplugin, COperator::Id opid, int plugout){
    ////qDebug<<"CControl ConnectPlug in op "<<opid<<" plug id "<<plugout<<" and plugin control"<<myindexplugin<<endl;

    if(_v_plug_in_processor[myindexplugin].first == false){
        _v_plug_in_processor[myindexplugin].first = true;
        _v_plug_in_processor[myindexplugin].second.first  = opid;
        _v_plug_in_processor[myindexplugin].second.second  = plugout;
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->connectPlugOutFromOutsideSend(opid,plugout);
        _v_plug_in[myindexplugin].second = CClientSingleton::getInstance()->getCInterfaceClient2Server()->addListenerPlugOutOperator2PlugInControl(opid,plugout,this,myindexplugin);
        _v_plug_in[myindexplugin].first = true;

    }
    else{
        ////qDebug<<"[ERROR] Plug in already connected for the control"<<endl;
        //getchar();
    }

}
void CControl::deconnectPlugIn(int myindexplugin){
    ////qDebug<<"CControl deConnectPlug in";
    if(_v_plug_in[myindexplugin].first == true){
        _v_plug_in_firstime[myindexplugin] = true;
        _v_plug_in_processor[myindexplugin].first = false;
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->disconnectPlugOutFromOutsideSend(_v_plug_in_processor[myindexplugin].second.first,_v_plug_in_processor[myindexplugin].second.second);
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->removeListenerPlugOutOperator2PlugInControl(_v_plug_in[myindexplugin].second);
        _v_plug_in[myindexplugin].first = false;
    }
    else{
        ////qDebug<<"[ERROR] Plug in already deconnected for the control"<<endl;
        //getchar();
    }

}

void CControl::updatePlugInControl(int ,CData* ){
    ////qDebug<<"[ERROR]"<<endl;
}

QString CControl::getData(){
    return dataout;
}

void CControl::updatePlugInControlComingFromMachine(int plugkey, CData * data, CPlug::State state){

    if(state==CPlug::NEW || (_v_plug_in_firstime[plugkey]==true&& state==CPlug::OLD))
    {
        _v_plug_in_firstime[plugkey] = false;
        if(this->structurePlug().plugIn()[plugkey].first == CData::KEY)
            updatePlugInControl(plugkey,data );
        else if(data->getKey()==this->structurePlug().plugIn()[plugkey].first)
            updatePlugInControl(plugkey,data );
        else
            this->error("Incompatible data type, from the composer the data type is "+ data->getKey()+" and the data type in the control's plug is "+this->structurePlug().plugIn()[plugkey].first );
    }
}

CControl::CControlStructure& CControl::structurePlug(){
    return _structure;
}
void CControl::setKey(CControl::Key key){
    _key =key;
}

CControl::Key CControl::getKey(){
    return _key;
}
void CControl::setName(string name){
    _name = name;
}

string CControl::getName(){
    return _name;
}
void CControl::setInformation(string information){
    _information = information;
}

string CControl::getInformation(){
    return _information;
}
string CControl::toString(){
    return "";
}

void CControl::fromString(string ){
    //TO implement
}

void CControl::error(string msg){
    emit errorMsg(this->_id, msg);
}

void CControl::setPath(vector<string> path){
    _path = path;
}
vector<string> CControl::getPath(){
    return _path;
}
void CControl::contextMenuEvent ( QContextMenuEvent * event ){
    QWidget::contextMenuEvent(event);
}

void CControl::dragEnterEvent ( QDragEnterEvent *  ){
}

void CControl::dragLeaveEvent ( QDragLeaveEvent *  ){

}

void CControl::dragMoveEvent ( QDragMoveEvent *  ){

}

void CControl::dropEvent ( QDropEvent *  ){

}

void CControl::enterEvent ( QEvent *  ){

}

void CControl::focusInEvent ( QFocusEvent *  ){

}

bool CControl::focusNextPrevChild ( bool  ){
    return true;
}

void CControl::focusOutEvent ( QFocusEvent *  ){

}

void CControl::hideEvent ( QHideEvent *  ){

}

void CControl::keyPressEvent ( QKeyEvent * keyEvent ){
    if(keyEvent->key() == Qt::Key_Return) this->apply();
}

void CControl::keyReleaseEvent ( QKeyEvent *  e){
    QWidget::keyReleaseEvent(e);
}

void CControl::leaveEvent ( QEvent * e ){
    QWidget::leaveEvent(e);
}

void CControl::mouseDoubleClickEvent ( QMouseEvent *  ){

}

void CControl::mouseMoveEvent ( QMouseEvent * event ){

    QWidget::mouseMoveEvent(event);
}

void CControl::mousePressEvent ( QMouseEvent * event ){
    QWidget::mousePressEvent(event);
}

void CControl::mouseReleaseEvent ( QMouseEvent *  event){
    QWidget::mouseReleaseEvent(event);
}

void CControl::moveEvent ( QMoveEvent *  ){

}

void CControl::paintEvent ( QPaintEvent *  ){
}

void CControl::resizeEvent ( QResizeEvent *  ){

}

void CControl::showEvent ( QShowEvent *  ){

}

void CControl::wheelEvent ( QWheelEvent *  ){

}
