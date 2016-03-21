#ifndef OPERATORLOADMAP_H
#define OPERATORLOADMAP_H
#include<COperator.h>
class OperatorLoadMap: public COperator
{
public:
    OperatorLoadMap();
    virtual void exec();
    virtual COperator * clone();
};
#endif // OPERATORLOADMAP_H
