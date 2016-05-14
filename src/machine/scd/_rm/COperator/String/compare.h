#ifndef COMPARE_H
#define COMPARE_H

#include<COperator.h>
#include<DataString.h>
#include<DataBoolean.h>
class compare : public COperator
{
public:
    compare();
    virtual void exec();
    virtual COperator * clone();
};

#endif // COMPARE_H
