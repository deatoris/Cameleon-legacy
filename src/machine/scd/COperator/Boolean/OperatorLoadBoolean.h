#ifndef OPERATORLOADBOOLEAN_H
#define OPERATORLOADBOOLEAN_H
#include<COperator.h>
class OperatorLoadBoolean: public COperator
{
public:
    OperatorLoadBoolean();
    virtual void exec();
    virtual COperator * clone();
};


#endif // OPERATORLOADBOOLEAN_H
