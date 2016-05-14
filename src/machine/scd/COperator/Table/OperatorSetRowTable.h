#ifndef OPERATORSETROWTABLE_H
#define OPERATORSETROWTABLE_H

#include<COperator.h>

class OperatorSetRowTable: public COperator
{
public:
    OperatorSetRowTable();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSETROWTABLE_H
