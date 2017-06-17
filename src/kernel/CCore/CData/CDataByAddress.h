/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CDATABYADDRESS_H
#define CDATABYADDRESS_H
//#include<COperator.h>
//#include <tr1/memory>
//#include<tr1/shared_ptr.h>
//using namespace std;
////Requirement of template parameter Type
//// void fromString(string)
//// void save(string)
//template<typename Type>
//class CDataByAddress : public CData
//{
//public:
//    CDataByAddress();
//    CData * copy();
//    CData * clone();
//    void fromString(string data);
//    string toString();
//    virtual void dumpReception(CData * data);
//    void setDataByFile(std::tr1::shared_ptr<Type> type);
//    virtual std::tr1::shared_ptr<Type> getDataByFile();
//    std::tr1::shared_ptr<Type> & data();

//protected:
//    std::tr1::shared_ptr<Type> _data;
//};

//template<typename Type>
//CDataByAddress<Type>::CDataByAddress()
//    :CData(),_data(new Type)
//{

//}
//template<typename Type>
//void CDataByAddress<Type>::dumpReception(CData * data){
//    if(CDataByAddress<Type> * d = dynamic_cast<CDataByAddress<Type> *>(data) )
//        this->setData(d->getData());
//}


//template<typename Type>
//CData * CDataByAddress<Type>::copy()
//{
//    CDataByAddress<Type> * data = static_cast<CDataByAddress<Type> *>(this->clone());



//    data->setData(this->getData());
//    return data;
//}
//template<typename Type>
//CData * CDataByAddress<Type>::clone()
//{
//    CDataByAddress<Type> * data =new CDataByAddress<Type>;
//    data->setData(this->getData());
//    return data;
//}

//template<typename Type>
//void CDataByAddress<Type>::fromString(string data){
//    std::istringstream iss(data );
//    _data = std::tr1::shared_ptr<Type>(new Type);
//    iss >> *_data.get();

//}
//template<typename Type>
//string CDataByAddress<Type>::toString(){
//    std::ostringstream oss;
//    oss << *_data.get();
//    return oss.str();
//}
//template<typename Type>
//void CDataByAddress<Type>::setData(std::tr1::shared_ptr<Type> data){
//    _data = data;
//}
//template<typename Type>
//std::tr1::shared_ptr<Type> CDataByAddress<Type>::getData(){
//    return _data;
//}template<typename Type>
//std::tr1::shared_ptr<Type> & CDataByAddress<Type>::data(){
//    return _data;
//}

#endif // CDATABYADDRESS_H
