/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CData.h"
#include "CPlug.h"
#include"COperator.h"
CData::CData()
    :_key(CData::KEY),_plug(NULL),_mode(BYFILE),_isdata(false)
{

}

CData::~CData(){

}


void CData::setMode(Mode mode){
    _mode=mode;
}

CData::Mode CData::getMode(){
    return _mode;
}



CData::Key CData::getKey()const
{
    return _key;
}
string CData::KEY="CDATAGENERIC";
CData * CData::copy(){
    return this->clone();

}
CData * CData::morpherMode( Mode mode, string  ){
    CData * d = new CData;
    d->setMode(mode);
    return d;
}
void CData::dumpReceptionCast(CData * )throw(CException){
//    throw(std::string("bad dump cast"));
}

void CData::dumpReception(CData * data)throw(std::string){
    bool error=false;
    CData* d;
    if(this->getKey()==CData::KEY){
        d =data->clone();
        if(_plug!=NULL){
            CPlug * plug = this->getPlug();
            d->setPlug(plug);
            plug->setData(d);
        }
    }else{
        if(this->getKey()!= data->getKey()){

            bool gen=false;
            if(dynamic_cast<CPlugIn*>(this->getPlug())){
                if(this->getPlug()->getOperator()->structurePlug().plugIn()[this->getPlug()->getIndex()].first==CData::KEY  )
                    gen =true;
            }else{
                if(this->getPlug()->getOperator()->structurePlug().plugOut()[this->getPlug()->getIndex()].first==CData::KEY  )
                    gen =true;
            }
            if(gen==true){
            d =data->clone();
            if(_plug!=NULL){
                CPlug * plug = this->getPlug();
                d->setPlug(plug);
                plug->setData(d);
            }
            }
            else{
                error=true;
                this->getPlug()->getOperator()->error("Dump error between two diferrent data 1="+this->getKey() +" and 2=" +data->getKey());
            }
        }
        else
            d=this;
    }
    if(error==false){
//        this->getPlug()->getOperator()->setError(false);
        d->dumpReceptionCast(data);
    }
}

CData * CData::clone(){
    CData *d =  new CData;
    d->setMode(this->getMode());
    return d;
}


void CData::setPlug(CPlug* plug){
    _plug=plug;
}
CPlug* CData::getPlug(){
    return _plug;
}


void CData::setExtension(string extension){
    _extension = extension;
}


string CData::getExtension()const{
    return _extension;
}

void CData::fromString(string ){

}

string CData::toString()const{
    return "";
}

//void CData::releaseData(){

//}
//void CData::deleteData(){

//}
bool CData::isDataAvailable(){
    return _isdata;
}

void CData::setIsDataAvailable(bool isdataavailable){
    _isdata = isdataavailable;
}

//void CData::log(string msg){

//}

//void CData::error(string msg){

//}

//void CData::warning(string msg){}
