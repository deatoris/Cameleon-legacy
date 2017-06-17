#ifndef OPERATORPOWNUMBER_H
#define OPERATORPOWNUMBER_H

#include<COperator.h>
#include<DataNumber.h>
class OperatorPowNumber : public COperator
{
public:
    OperatorPowNumber();
    virtual void exec();
    virtual COperator * clone();
};
#endif // OPERATORPOWNUMBER_H
