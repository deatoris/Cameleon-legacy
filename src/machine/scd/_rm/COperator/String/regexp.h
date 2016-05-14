#ifndef REGEXP_H
#define REGEXP_H
#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include <DataVector.h>
class regexp : public COperator
{
public:
    regexp();
    virtual void exec();
    virtual COperator * clone();
};
#endif // REGEXP_H
