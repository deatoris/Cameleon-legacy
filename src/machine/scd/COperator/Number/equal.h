#ifndef EQUAL_H
#define EQUAL_H
#include<COperator.h>
#include<DataNumber.h>
#include<DataBoolean.h>
class equalnumber : public COperator
{
public:
    equalnumber();
    virtual void exec();
    virtual COperator * clone();
};

#endif // EQUAL_H
