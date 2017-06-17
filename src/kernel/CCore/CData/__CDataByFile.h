/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CDataByPointer_H
#define CDataByPointer_H
#include<COperator.h>

#ifndef NOQT
#include<CMachine.h>
#include<QMutex>
#endif
#include <tr1/memory>
#include<tr1/shared_ptr.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
//Requirement of template parameter Type
// void load(string)
// void save(string)
template<typename Type>
class CDataByPointer : public CData
{
public:

    CDataByPointer();
    string getFile();
    void setFile(string file);
    virtual CDataByPointer<Type> * clone()=0;
    void fromString(string data);
    string toString();


    void setData(Type * type);
    Type* getData();
    virtual void setDataByFile(Type* type)=0;
    virtual Type* getDataByFile()=0;
    virtual Type* copy(Type* type)=0;

    CData * morpherMode(Mode mode, string filename );
    virtual void dumpReceptionCast(CData * data)throw(std::string);
    std::tr1::shared_ptr<Type> __dataShared();
    void releaseData();
protected:
#ifndef NOQT
    QMutex _mutex;
#endif
    string _file;
    std::tr1::shared_ptr<Type> _data;
    Type * _data_pointer;
};




template<typename Type>
CDataByPointer<Type>::CDataByPointer()
    :CData()
{
    this->_mode= BYFILE;
}
template<typename Type>
Type * CDataByPointer<Type>::getData(){
#ifndef NOQT
    QMutexLocker locker(&_mutex);
#endif
    if(_mode==BYFILE){
        _data = std::tr1::shared_ptr<Type>(this->getDataByFile());
    }
    if(_mode==BYADDRESSNOSHAREDPOINTER){
        return _data_pointer;
    }
    else
    {
        return _data.get();
    }
}
template<typename Type>
void CDataByPointer<Type>::setData(Type* t){
#ifndef NOQT
    QMutexLocker locker(&_mutex);
#endif
    if(_mode==BYFILE){
        this->setDataByFile(t);
    }else if(_mode==BYSHAREDADDRESSS||_mode==BYCLONE){
        _data = std::tr1::shared_ptr<Type>(t);
    }else{
        _data_pointer =t;
    }
}

template<typename Type>
void CDataByPointer<Type>::dumpReceptionCast(CData * data)throw(std::string)
{
    this->setIsDataAvailable(true);
    if(CDataByPointer<Type> * d = dynamic_cast<CDataByPointer<Type> *>(data) ){
        if(d->getMode()==BYFILE){
            this->_mode = BYFILE;
            this->setFile(d->getFile());
        }
        else if (d->getMode()==BYSHAREDADDRESSS)
        {
            this->_mode = BYSHAREDADDRESSS;
            this->_data =d->__dataShared();
        }
        else if (d->getMode()==BYCLONE){
            this->_mode = BYCLONE;
            this->_data =std::tr1::shared_ptr<Type>(d->copy(d->getData()));
        }
        else{
            this->_mode = BYADDRESSNOSHAREDPOINTER;
            string msg = "For the data-dump between operators, we have incompatibale data,\n *input data = "+data->getKey()+"\n *output data = "+this->getKey();
            throw(msg);
        }

    }else
    {
        string msg = "For the data-dump between operators, we have incompatibale data,\n *input data = "+data->getKey()+"\n *output data = "+this->getKey();
        throw(msg);
    }
}


template<typename Type>
string CDataByPointer<Type>::getFile()
{
    if(_file=="")
    {
#ifndef NOQT
        _file = CMachineSingleton::getInstance()->getTmpDir()+"/";
#endif
        if(this->getPlug()!=NULL){
            _file += this->_plug->getOperator()->getKey();
            int plugkey = this->_plug->getIndex();
            std::ostringstream oss;
            oss << plugkey;
            _file += oss.str();
            int opid = this->_plug->getOperator()->getId();
            std::ostringstream oss2;
            oss2 << opid;
            _file += oss2.str();
        }else{
            int i = rand();
            std::ostringstream oss;
            oss << i;
            _file += oss.str();
        }
        _file+= this->getExtension();
    }
    return _file;
}
template<typename Type>
void CDataByPointer<Type>::setFile(string file)
{
    _file = file;
}
template<typename Type>
void CDataByPointer<Type>::fromString(string data){
    this->setFile(data);
}
template<typename Type>
string CDataByPointer<Type>::toString(){
    return this->getFile();
}
template<typename Type>
std::tr1::shared_ptr<Type> CDataByPointer<Type>::__dataShared(){
    if(_mode==BYFILE){
        _data.reset(this->getDataByFile());
    }
    return _data;

}
template<typename Type>
void CDataByPointer<Type>::releaseData(){
    if(_mode!=BYADDRESSNOSHAREDPOINTER)
        _data.reset();
}

template<typename Type>
CData * CDataByPointer<Type>::morpherMode( Mode mode ,string filename){
    CDataByPointer<Type> * dout =this->clone();
    dout->setMode(mode);
    if(filename!=""){
        dout->setFile(filename);
    }
    dout->setData(this->getData());
    return dout;
}

#endif // CDataByPointer_H
