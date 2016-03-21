#ifndef MSECTOSTRING_H
#define MSECTOSTRING_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataDateTime.h>
#include<DataString.h>
class msectostring : public COperator
{
public:
    msectostring();
    virtual void exec();
    virtual COperator * clone();
    string getPrintableProbe(int lastPerf);
};

#endif // MSECTOSTRING_H
