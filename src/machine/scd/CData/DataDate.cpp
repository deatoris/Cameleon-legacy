#include "DataDate.h"

ostream& operator << (ostream& out, const QDate & v){
    out <<v.toString("dd.MM.yyyy").toStdString()<<endl;
    return out;
}

istream& operator >> (istream& in, QDate & v){
    string time;
    in >> time;
    v.fromString(time.c_str(),"dd.MM.yyyy");
    return in;
}

DataDate::DataDate()
    :CDataByValue<QDate >()
{
    this->_key = DataDate::KEY;
}

string DataDate::KEY ="DataDate";

DataDate * DataDate::clone()
{
    return new DataDate;
}
