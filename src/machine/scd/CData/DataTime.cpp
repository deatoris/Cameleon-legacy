#include "DataTime.h"

ostream& operator << (ostream& out, const QTime & v){
    out <<v.toString("hh:mm:ss.zzz").toStdString()<<endl;
    return out;
}

istream& operator >> (istream& in, QTime & v){
    string time;
    in >> time;
    v.fromString(time.c_str(),"hh:mm:ss.zzz");
    return in;
}

DataTime::DataTime()
    :CDataByValue<QTime >()
{
    this->_key = DataTime::KEY;
}

string DataTime::KEY ="DataTime";

DataTime * DataTime::clone()
{
    return new DataTime;
}
