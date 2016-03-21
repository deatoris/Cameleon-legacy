#ifndef DATADATETIME_H
#define DATADATETIME_H
#include <QtCore>
#include "table.h"
#include<CDataByValue.h>

using namespace std;
ostream& operator << (ostream& out, const QDateTime & v);
istream& operator >> (istream& in, QDateTime & v);

class DataDateTime : public CDataByValue<QDateTime >
{
public:
    DataDateTime();
    static string KEY;
    virtual DataDateTime * clone();
};
#endif // DATADATETIME_H
