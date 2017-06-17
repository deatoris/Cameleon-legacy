#ifndef ABSNUMBER_H
#define ABSNUMBER_H

#include<COperator.h>
#include<DataNumber.h>
class OperatorAbsNumber : public COperator
{
public:
    OperatorAbsNumber();
    virtual void exec();
    virtual COperator * clone();
};
#endif // ABSNUMBER_H
