#ifndef GETDAY_H
#define GETDAY_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataDateTime.h>
class getday : public COperator
{
public:
    getday();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETDAY_H
