/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CCOLLECTOR_H
#define CCOLLECTOR_H
#include"COperator.h"
#include<CData.h>

#include<CControl.h>
#include <QtPlugin>

#include<vector>
#include<string>
using namespace std;

class CDictionnary
{
private:
    vector<CData*> _v_data;
    vector<COperator*> _v_op;


    vector<CControl*> _v_control;

    string _namedictionnary;
    string version;
    string _information;
public:
    virtual ~CDictionnary();
    void setVersion(string version);
    string getVersion();
    string getNameDictionnary();
    void setNameDictionnary(string namedictionnary);
    void registerData(CData * data);
    void registerOperator(COperator * op);


    void registerControl(CControl * control);

    virtual void collectData()=0;
    virtual void collectOperator()=0;
    virtual void collectControl()=0;
    vector<CData*> & datas();
    vector<COperator*> & operators();
    vector<CControl*> & controls();

    void setInformation(string info);
    string getInformation();

};
#endif // CCOLLECTOR_H
