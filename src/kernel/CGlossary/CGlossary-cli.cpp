/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGlossary-cli.h"

//#include<../cconnectivityDictionnary.h>

#include <CSDDictionnary-cli.h>
//#include "PopulationDictionnary.h"
#include "CLogger.h"
#include<CDictionnary-cli.h>
//#include "OpenCVDictionnary.h"
CGlossary::CGlossary(){

    this->registerDictionnay(new CSDDictionnary);
    //this->registerDictionnay(new cconnectivityDictionary);
    CLoggerInstance::getInstance()->log("Population disabled");
    //this->registerDictionnay(new PopulationDictionnary);
    CLoggerInstance::getInstance()->log("Opencv disabled");
    //  this->registerDictionnay(new OpenCVDictionary);
    QString s;
    s = "Number of datas loaded " +  QString::number(_f_data.listKey().size())+"";
    CLoggerInstance::getInstance()->log(s);
    s = "Number of operators loaded " + QString::number(_f_operator.listKey().size())+"";
    CLoggerInstance::getInstance()->log(s);
}

CGlossary::~CGlossary(){
    for(int i=0;i<(int)_v_dic.size();i++)
        delete _v_dic[i];
}
CFactoryData & CGlossary::factoryData(){
    return _f_data;
}

CFactoryOperator & CGlossary::factoryOperator(){
    return _f_operator;
}

void CGlossary::registerDictionnay(CDictionnary * dic){
    this->_v_dic.push_back(dic);
    dic->collectData();
    vector<CData *> & v_data = dic->datas();
    for(int i=0;i<(int)v_data.size();i++)
    {
        _f_data.Register(v_data[i]->getKey(),v_data[i]);
    }
    dic->collectOperator();
    vector<COperator *> & v_operator  = dic->operators();
    for(int i=0;i<(int)v_operator.size();i++)
    {
        _f_operator.Register(v_operator[i]->getKey(),v_operator[i]);
    }
}

vector<CDictionnary *> CGlossary::getDictionaries(){
    return _v_dic;
}


CData * CGlossary::createData(CData::Key key) {
    return _f_data.createObject(key);
}
vector<string> CGlossary::getDataKeys(){
    return _f_data.listKey();
}

vector<string> CGlossary::getOperatorKeys(){
    return _f_operator.listKey();
}
COperator * CGlossary::createOperator(COperator::Key key)throw(exception) {

    return _f_operator.createObject(key);
}

vector<pair<vector<string>,string> > CGlossary::getOperatorsByName(){

    vector<pair<vector<string>,string> > v_list;
    for(int i=0;i<(int)_v_dic.size();i++)
    {
        vector<COperator *> & v_operator  = _v_dic[i]->operators();
        for(int j=0;j<(int)v_operator.size();j++)
        {
            vector<string> v;
            v.push_back(_v_dic[i]->getNameDictionnary());
            vector<string> path=v_operator[j]->getPath();
            for(int k=0;k<(int)path.size();k++)
            {
                v.push_back(path[k]);
            }
            v.push_back(v_operator[j]->getKey());
            v_list.push_back(make_pair(v,v_operator[j]->getName()));
        }

    }

    return v_list;
}
