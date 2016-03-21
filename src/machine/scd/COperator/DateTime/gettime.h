#ifndef GETTIME_H
#define GETTIME_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataDateTime.h>
class gettime : public COperator
{
public:
    gettime();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETTIME_H
