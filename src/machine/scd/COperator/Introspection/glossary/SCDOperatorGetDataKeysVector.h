#ifndef SCDOPERATORGETDATAKEYSVECTOR_H
#define SCDOPERATORGETDATAKEYSVECTOR_H
#include<COperator.h>

class SCDOperatorGetDataKeysVector : public COperator
{
public:
    SCDOperatorGetDataKeysVector();
    virtual void exec();
    virtual COperator * clone();
    bool executionCondition();
    void initState();
};

#endif // SCDOPERATORGETDATAKEYSVECTOR_H
