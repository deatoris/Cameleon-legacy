/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CDATABYPOINTER_H
#define CDATABYPOINTER_H


#include"COperator.h"
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <limits>

using namespace std;

#include <CMachine.h>

class RC
{
    private:
    int count; // Reference count

    public:
    void AddRef()
    {
        // Increment the reference count
        count++;
    }

    int Release()
    {
        // Decrement the reference count and
        // return the reference count.
        return --count;
    }
    void releaseReferenceCounting()
    {
        count=numeric_limits<int>::max() ;
    }
};
template < typename T > class shared_ptr
{
private:
    T*    pData;       // pointer
    RC* reference; // Reference count

public:
    shared_ptr() : pData(0), reference(0)
    {
        // Create a new reference
        reference = new RC();
        // Increment the reference count
        reference->AddRef();
    }

    shared_ptr(T* pValue) : pData(pValue), reference(0)
    {
        // Create a new reference
        reference = new RC();
        // Increment the reference count
        reference->AddRef();
    }

    shared_ptr(const shared_ptr<T>& sp) : pData(sp.pData), reference(sp.reference)
    {
        // Copy constructor
        // Copy the data and reference pointer
        // and increment the reference count
        reference->AddRef();
    }

    ~shared_ptr()
    {
        // Destructor
        // Decrement the reference count
        // if reference become zero delete the data
        if(reference->Release() == 0)
        {
            delete pData;
            delete reference;
        }
    }

    T& operator* ()
    {
        return *pData;
    }

    T* operator-> ()
    {
        return pData;
    }
    T* get(){
          return pData;
    }
    const T& operator* ()const
    {
        return *pData;
    }

    const T* operator-> ()const
    {
        return pData;
    }
    const T* get()const
    {
          return pData;
    }
    void releaseReferenceCounting(){
        reference->releaseReferenceCounting();
    }

    shared_ptr<T>& operator = (const shared_ptr<T>& sp)
    {
        // Assignment operator
        if (this != &sp&& pData != sp.pData) // Avoid self assignment
        {
            // Decrement the old reference count
            // if reference become zero delete the old data
            if(reference->Release() == 0)
            {
                delete pData;
                delete reference;
            }
            // Copy the data and reference pointer
            // and increment the reference count
            pData = sp.pData;
            reference = sp.reference;
            reference->AddRef();
        }
        return *this;
    }
};

template<typename Type>
class CDataByFile : public CData
{
public:

    CDataByFile();
    string getFile() const;
    void setFile(string file);
    virtual CDataByFile<Type> * clone()=0;
    void fromString(string data);
    virtual string toString()const;


    void setData(shared_ptr<Type> type);
    shared_ptr<Type> getData();


    CData * morpherMode(Mode mode, string filename );
    virtual void dumpReceptionCast(CData * data)throw(CException);


    virtual void setDataByFile(shared_ptr<Type> ){}
    virtual void setDataByCopy(shared_ptr<Type> ){}
    virtual shared_ptr<Type> getDataByFile(){return _data;}
    shared_ptr<Type> _data;
protected:
    mutable string _file;

};


template<typename Type>
CData * CDataByFile<Type>::morpherMode( Mode mode ,string filename){
    CDataByFile<Type> * dout =this->clone();
    dout->setMode(mode);
    if(filename!=""){
        dout->setFile(filename);
    }
    dout->setData(this->getData());
    return dout;
}



template<typename Type>
CDataByFile<Type>::CDataByFile()
    :CData()
{
    this->_mode= BYFILE;
}
template<typename Type>
void CDataByFile<Type>::dumpReceptionCast(CData * data)throw(CException)
{
    this->setIsDataAvailable(true);
    if(CDataByFile<Type> * d = dynamic_cast<CDataByFile<Type> *>(data) ){
        if(d->getMode()==BYFILE){
            this->setFile(d->getFile());
        }
        else if (d->getMode()==BYADDRESS)
        {
            this->_mode = BYADDRESS;
            this->setData(d->getData());

        }if (d->getMode()==BYCOPY){
            this->_mode = BYCOPY;
            this->setData(d->getData());
        }

    }else
    {
        string msg = "For the data-dump between operators, we have incompatibale data,\n *input data = "+data->getKey()+"\n *output data = "+this->getKey();
        throw(msg);
     }
}


template<typename Type>
string CDataByFile<Type>::getFile()const
{
    if(_file=="")
    {

        _file = CMachineSingleton::getInstance()->getTmpPath()+"/";
        if(this->_plug!=NULL){
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
void CDataByFile<Type>::setFile(string file)
{
    _file = file;
}
template<typename Type>
void CDataByFile<Type>::fromString(string data){
    this->setFile(data);
}
template<typename Type>
string CDataByFile<Type>::toString()const{
    return this->getFile();
}
template<typename Type>
void CDataByFile<Type>::setData(shared_ptr<Type> type){

    if(_mode==BYFILE){
        this->setDataByFile(type);
    }else if(_mode==BYADDRESS){
        _data = type;
    }else if(_mode==BYVALUE){
        *_data = *type;
    }
    else{
        this->setDataByCopy(type);
    }

}
//template<typename Type>
//void CDataByFile<Type>::setDataPointer(Type *type){
//    setData(shared_ptr<Type>(type));
//}
//template<typename Type>
//Type * CDataByFile<Type>::getDataPointer(){
//    _data = getData();
//    return _data.get();
//}
//template<typename Type>
//void CDataByFile<Type>::releaseDataPointer(){
//    _data.reset();
//}

template<typename Type>
shared_ptr<Type> CDataByFile<Type>::getData(){

    if(_mode==BYFILE){
        return this->getDataByFile();
    }else{
        return _data;
    }
}








#endif // CDATABYPOINTER_H
