/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "DBTables.h"

#include "DataVector.h"
#include "DataString.h"
#include "DataDB.h"
#include <QtSql>
DBTables::DBTables(){
    this->structurePlug().addPlugIn(DataDB::KEY,"database.db");
    this->structurePlug().addPlugOut(DataVector::KEY,"result.vec");
    this->path().push_back("DataBase");
    this->setKey("tables");
    this->setName("tables");
    this->setInformation("get list tables on the database");
}

void DBTables::exec(){
    DataDB* db = dynamic_cast<DataDB *>(this->plugIn()[0]->getData());
//    DataVector::DATAVECTOR  vec= dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->getData();
    vector<shared_ptr<CData> > * vec = new vector<shared_ptr<CData> > ;

    QSqlDatabase datab = db->getValue();
    if(!datab.isOpen()){
        this->error("DB not opened!");
        return;
    }
    for(int i=0;i<datab.tables().count();i++){
        string str = datab.tables().value(i).toStdString();
        DataString* s = new DataString();

        s->setValue(str);
        shared_ptr<CData> ss(s);
        vec->push_back(ss);
    }

    //    list << result;
    //    DataString* st = new DataString();
    //    st->setValue(result.toStdString());
    //    v->push_back(shared_ptr<CData>(st));

    //}
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(vec);


}

COperator * DBTables::clone(){
    return new DBTables();
}
