#ifndef OPERATORGETROWTABLE_H
#define OPERATORGETROWTABLE_H

#include<COperator.h>

class OperatorGetRowTable: public COperator
{
public:
    OperatorGetRowTable();
    virtual void exec();
    void initState();
    virtual COperator * clone();
};
#endif // OPERATORGETROWTABLE_H
