#ifndef OPERATORRMROWTABLE_H
#define OPERATORRMROWTABLE_H

#include<COperator.h>

class OperatorRmRowTable: public COperator
{
public:
    OperatorRmRowTable();
    virtual void exec();
    void initState();
    virtual COperator * clone();
};
#endif // OPERATORRMROWTABLE_H
