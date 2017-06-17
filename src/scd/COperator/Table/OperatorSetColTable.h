#ifndef OPERATORSETCOLTABLE_H
#define OPERATORSETCOLTABLE_H

#include<COperator.h>

class OperatorSetColTable: public COperator
{
public:
    OperatorSetColTable();
    virtual void exec();
    virtual COperator * clone();
};
#endif // OPERATORSETCOLTABLE_H
