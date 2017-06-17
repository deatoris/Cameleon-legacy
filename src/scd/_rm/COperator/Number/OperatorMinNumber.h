#ifndef OPERATORMINNUMBER_H
#define OPERATORMINNUMBER_H

#include<COperator.h>
#include<DataNumber.h>
class OperatorMinNumber : public COperator
{
public:
    OperatorMinNumber();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORMINNUMBER_H
