#ifndef OPERATORSAVEMAP_H
#define OPERATORSAVEMAP_H

#include<COperator.h>
class OperatorSaveMap: public COperator
{
public:
    OperatorSaveMap();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSAVEMAP_H
