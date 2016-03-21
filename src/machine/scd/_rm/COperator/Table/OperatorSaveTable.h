#ifndef OPERATORSAVETABLE_H
#define OPERATORSAVETABLE_H

#include<COperator.h>
class OperatorSaveTable: public COperator
{
public:
    OperatorSaveTable();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSAVETABLE_H
