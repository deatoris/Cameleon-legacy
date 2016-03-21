/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "Createdb.h"
#include "DataDB.h"
#include <DataString.h>
#include <DataBoolean.h>
#include <QtSql>
CreateDB::CreateDB(){
//    this->structurePlug().addPlugIn(DataString::KEY,"type.str");
//    this->structurePlug().addPlugIn(DataString::KEY,"hostname.str");
    this->structurePlug().addPlugIn(DataString::KEY,"dbname.str");
    this->structurePlug().addPlugIn(DataString::KEY,"filepath.str");
//    this->structurePlug().addPlugIn(DataString::KEY,"user.str");
//    this->structurePlug().addPlugIn(DataString::KEY,"password.str");
    this->structurePlug().addPlugOut(DataDB::KEY,"database.db");

    this->path().push_back("DataBase");
    this->setKey("SQLITEDB");
    this->setName("SQLITEDB");
    this->setInformation("Create a sqlite data base");
}

void CreateDB::exec(){
    DataString* dbname = dynamic_cast<DataString *>(this->plugIn()[0]->getData());
    DataString* dbpath = dynamic_cast<DataString *>(this->plugIn()[1]->getData());

    DataDB * datadb = dynamic_cast<DataDB *>(this->plugOut()[0]->getData());

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString qdbpath = dbpath->getValue().c_str();
    QString qdbname = dbname->getValue().c_str();
    QString qfullname = qdbpath+"/"+qdbname;
    db.setDatabaseName(qfullname);
    if(!db.open()){
        this->error("Can't create database!");
        return;
    }
    if(!db.isValid()){
        this->error("Database not valid!");
        return;
    }
    if(db.isOpenError()){
        this->error("Database open error!");
        return;
    }
    if(!db.isDriverAvailable("QSQLITE")){
        this->error("Driver not available!");
        return;
    }

//    QSqlQuery query;
//    query.exec("create table person (id int primary key, name varchar(20), address varchar(200), typeid int)");
//    query.exec("insert into person values(1, 'Alice', '<qt>123 Main Street<br/>Market Town</qt>', 101)");
//    query.exec("insert into person values(2, 'Bob', '<qt>PO Box 32<br/>Mail Handling Service <br/>Service City</qt>', 102)");
//    query.exec("insert into person values(3, 'Carol', '<qt>The Lighthouse<br/>Remote Island</qt>', 103)");
//    query.exec("insert into person values(4, 'Donald', '<qt>47338 Park Avenue<br/>Big City</qt>', 101)");
//    query.exec("insert into person values(5, 'Emma', '<qt>Research Station<br/>Base Camp<br/>Big Mountain</qt>', 103)");

//    db.commit();
    datadb->setValue(db);
}

COperator * CreateDB::clone(){
    return new CreateDB();
}
