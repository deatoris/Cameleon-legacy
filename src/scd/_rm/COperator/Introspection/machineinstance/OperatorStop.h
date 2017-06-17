#ifndef OPERATORSTOP_H
#define OPERATORSTOP_H

#include<COperator.h>
#include <CData.h>


class OperatorStop :public COperator
{
public:
    OperatorStop();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSTOP_H
