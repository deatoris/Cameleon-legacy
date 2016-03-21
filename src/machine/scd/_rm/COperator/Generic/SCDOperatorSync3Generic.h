#ifndef SCDOPERATORSYNC3GENERIC_H
#define SCDOPERATORSYNC3GENERIC_H

#include<COperator.h>
#include<CData.h>


class SCDOperatorSync3Generic :public COperator
{
public:
    SCDOperatorSync3Generic();
    virtual void exec();
    virtual COperator * clone();
    virtual bool executionCondition();
};
#endif // SCDOPERATORSYNC3GENERIC_H
