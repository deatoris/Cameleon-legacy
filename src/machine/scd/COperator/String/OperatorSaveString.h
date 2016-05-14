#ifndef OPERATORSAVESTRING_H
#define OPERATORSAVESTRING_H

#include<COperator.h>
class OperatorSaveString: public COperator
{
public:
    OperatorSaveString();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSAVESTRING_H
