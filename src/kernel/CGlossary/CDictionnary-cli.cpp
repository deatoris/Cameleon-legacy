/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CDictionnary-cli.h"

CDictionnary::~CDictionnary()
{

    for(int i=0;i<(int)_v_data.size();i++)
        delete _v_data[i];
    for(int i=0;i<(int)_v_op.size();i++)
        delete _v_op[i];
}
void CDictionnary::registerData(CData * data){
    _v_data.push_back(data);
}

void CDictionnary::setInformation(string info){
    _information = info;
}

string CDictionnary::getInformation(){
    return _information;
}
void CDictionnary::registerOperator(COperator * op){
    _v_op.push_back(op);
}
vector<CData*> & CDictionnary::datas(){
    return _v_data;
}

vector<COperator*> & CDictionnary::operators(){
    return _v_op;
}


void CDictionnary::setVersion(string version){
    this->version = version;
}

string CDictionnary::getVersion(){
    return version;
}

string CDictionnary::getNameDictionnary(){
    return _namedictionnary;
}

void CDictionnary::setNameDictionnary(string namedictionnary){
    _namedictionnary = namedictionnary;
}
