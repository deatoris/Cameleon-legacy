#ifndef OPERATORLOADTABLE_H
#define OPERATORLOADTABLE_H

#include<COperator.h>
class OperatorLoadTable: public COperator
{
public:
    OperatorLoadTable();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORLOADTABLE_H
