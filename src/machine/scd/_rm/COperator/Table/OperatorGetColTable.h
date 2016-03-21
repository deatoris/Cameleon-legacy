#ifndef OPERATORGETCOLTABLE_H
#define OPERATORGETCOLTABLE_H

#include<COperator.h>

class OperatorGetColTable: public COperator
{
public:
    OperatorGetColTable();
    virtual void exec();
    void initState();
    virtual COperator * clone();
};
#endif // OPERATORGETCOLTABLE_H
