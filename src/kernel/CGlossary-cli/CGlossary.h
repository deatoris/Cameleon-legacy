/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGLOSSARY_H
#define CGLOSSARY_H
#include<CSingleton.h>
#include<CFactoryData.h>
#include<CFactoryOperator.h>

#include<vector>
class CDictionnary;

class CGlossary
{
private:
    vector<CDictionnary *> _v_dic;
    CFactoryData _f_data;
    CFactoryOperator _f_operator;

public:
    CGlossary();
    ~CGlossary();
    CFactoryData & factoryData();
    CFactoryOperator & factoryOperator();
    vector<pair<vector<string>,string> > getOperatorsByName();
    CData * createData(CData::Key key);
    vector<string> getDataKeys();
    vector<string> getOperatorKeys();
    COperator * createOperator(COperator::Key key)throw(exception);
    COperator * getStructureOperator(COperator::Key key);
    void registerDictionnay(CDictionnary * dic);
    void unregisterDictionnay(string namedictionnary);

    vector<CDictionnary *> getDictionaries();
};
typedef CSingleton<CGlossary,0> CGlossarySingletonServer;
typedef CSingleton<CGlossary,1> CGlossarySingletonClient;
//typedef CSingleton<CGlossary> CGlossarySingleton;
#endif // CGLOSSARY_H


