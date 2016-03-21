/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CDATABYVALUE_H
#define CDATABYVALUE_H


#include<CDataByFile.h>

#include<typeinfo>
#include <fstream>

template<typename T>
class CDataByValue : public CDataByFile<T>
{
public:
    typedef T Type;
    CDataByValue()
        :CDataByFile<T>()
    {
        this->_data = shared_ptr<Type>(new Type());
        this->_key = typeid(CDataByValue<Type>).name();
        this->setMode(CData::BYCOPY);
    }
    Type getValue()
    {
        return *this->_data.get();
    }

    Type &value()
    {
        return *this->_data.get();
    }

    template<typename Parm>
    void setValue(Parm value)
    {
        *this->_data.get() = value;
    }
    virtual CDataByValue<T> * clone()=0;

    virtual void dumpReceptionCast(CData * data)throw(CException)
    {
        this->setIsDataAvailable(true);
        if(CDataByValue * datav = dynamic_cast<CDataByValue *>(data))
            this->setValue(datav->getValue());
        else
        {
            string msg = "For the data-dump between operators, we have incompatibale data,\n *input data = "+data->getKey()+"\n *output data = "+this->getKey();
            throw(msg);
        }
    }
    virtual string toString()const
    {
        std::ostringstream oss;
        oss << *this->_data.get();
        return oss.str();
    }
    virtual void fromString(string str)
    {
        std::istringstream iss(str );
        iss >>  *(this->_data.get());
    }
    virtual void setDataByFile(shared_ptr<Type> ){}
    virtual void setDataByCopy(shared_ptr<Type> ){}
    virtual shared_ptr<Type> getDataByFile(){return this->_data ;}
};
#endif // CDATABYVALUE_H
