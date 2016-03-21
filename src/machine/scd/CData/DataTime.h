#ifndef DATATIME_H
#define DATATIME_H

#include <QtCore>
#include "table.h"
#include<CDataByValue.h>

using namespace std;
ostream& operator << (ostream& out, const QTime & v);
istream& operator >> (istream& in, QTime & v);

class DataTime : public CDataByValue<QTime >
{
public:
    DataTime();
    static string KEY;
    virtual DataTime * clone();
};
#endif // DATATIME_H
