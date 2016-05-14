#ifndef OPERATORRMCOLTABLE_H
#define OPERATORRMCOLTABLE_H

#include<COperator.h>

class OperatorRmColTable: public COperator
{
public:
    OperatorRmColTable();
    virtual void exec();
    void initState();
    virtual COperator * clone();
};
#endif // OPERATORRMCOLTABLE_H
