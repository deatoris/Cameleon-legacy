#ifndef OPERATORMAXNUMBER_H
#define OPERATORMAXNUMBER_H
#include<COperator.h>
#include<DataNumber.h>
class OperatorMaxNumber : public COperator
{
public:
    OperatorMaxNumber();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORMAXNUMBER_H
