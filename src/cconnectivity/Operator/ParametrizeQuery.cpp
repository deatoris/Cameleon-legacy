/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "ParametrizeQuery.h"
#include "DataTable.h"
#include "DataString.h"
#include "DataVector.h"
#include "DataDB.h"
#include <QtSql>
ParametrizeQuery::ParametrizeQuery(){
    this->structurePlug().addPlugIn(DataString::KEY,"query.str");
    this->structurePlug().addPlugIn(DataVector::KEY,"params<str>.vec");
    this->structurePlug().addPlugIn(DataDB::KEY,"database.db");
    this->structurePlug().addPlugOut(DataTable::KEY,"result.table");
    this->path().push_back("DataBase");
    this->setKey("ParametrizeQuery");
    this->setName("ParametrizeQuery");
    this->setInformation("execute the given query string with given parameters to the db and return the result in a table form.");
}

void ParametrizeQuery::exec(){
    DataString* querystr = dynamic_cast<DataString *>(this->plugIn()[0]->getData());
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[1]->getData())->getData();
    DataDB* db = dynamic_cast<DataDB *>(this->plugIn()[2]->getData());
    shared_ptr<Table> tab= dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->getData();

    QSqlDatabase datab = db->getValue();
    if(!datab.isOpen()){
        this->error("DB not opened!");
        return;
    }
    QSqlQuery query(datab);
    vector<shared_ptr<CData> >::iterator it;

    cout << "myvector contains:";
    int k = 0;
    for ( it=vec->begin() ; it < vec->end(); it++ ){
        shared_ptr<CData> sh = *it;
        DataString* s = dynamic_cast<DataString *>(sh.get());
        query.bindValue(k,s->getValue().c_str());
        k++;
    }
    bool ex = query.exec(querystr->getValue().c_str());
    if(ex){
        int row = 0;
        while (query.next()) {
            tab->resize(row+1,query.record().count());
            for(int i=0;i<query.record().count();i++){
                string str = query.value(i).toString().toStdString();
                tab->operator ()(row,i)= str;
            }
            row++;
        }

        dynamic_cast<DataTable *>(this->plugOut()[0]->getData())->setData(tab);
    }else{
        QString s = query.lastError().text();
        this->error("SQLITE ERROR - "+s.toStdString());
        return;
    }
    datab.commit();
}

COperator * ParametrizeQuery::clone(){
    return new ParametrizeQuery();
}
