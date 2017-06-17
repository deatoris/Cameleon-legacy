#include "DataDateTime.h"

ostream& operator << (ostream& out, const QDateTime & v){
    out <<v.toString("dd.MM.yyyy hh:mm:ss.zzz").toStdString()<<endl;
    return out;
}

istream& operator >> (istream& in, QDateTime & v){
    string time;
    in >> time;
    v.fromString(time.c_str(),"dd.MM.yyyy hh:mm:ss.zzz");
    return in;
}

DataDateTime::DataDateTime()
    :CDataByValue<QDateTime >()
{
    this->_key = DataDateTime::KEY;
}

string DataDateTime::KEY ="DataDateTime";

DataDateTime * DataDateTime::clone()
{
    return new DataDateTime;
}
