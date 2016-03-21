#ifndef GETMONTH_H
#define GETMONTH_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataDateTime.h>
class getmonth : public COperator
{
public:
    getmonth();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETMONTH_H
