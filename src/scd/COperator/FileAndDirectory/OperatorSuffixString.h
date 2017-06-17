#ifndef OPERATORSUFFIXSTRING_H
#define OPERATORSUFFIXSTRING_H

#include<COperator.h>
#include<DataString.h>
class OperatorSuffixString : public COperator
{
public:
    OperatorSuffixString();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSUFFIXSTRING_H
