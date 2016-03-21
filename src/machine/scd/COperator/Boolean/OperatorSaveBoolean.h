#ifndef OPERATORSAVEBOOLEAN_H
#define OPERATORSAVEBOOLEAN_H

#include<COperator.h>
class OperatorSaveBoolean: public COperator
{
public:
    OperatorSaveBoolean();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSAVEBOOLEAN_H
