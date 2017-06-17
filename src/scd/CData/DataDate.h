#ifndef DATADATE_H
#define DATADATE_H

#include <QtCore>
#include "table.h"
#include<CDataByValue.h>

using namespace std;
ostream& operator << (ostream& out, const QDate & v);
istream& operator >> (istream& in, QDate & v);

class DataDate : public CDataByValue<QDate >
{
public:
    DataDate();
    static string KEY;
    virtual DataDate * clone();
};

#endif // DATADATE_H
