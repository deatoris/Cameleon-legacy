#ifndef ASYMMETRIC_H
#define ASYMMETRIC_H

#include<COperator.h>
#include<CData.h>

class asymmetric :public COperator
{
public:
    asymmetric();
    virtual void exec();
    virtual COperator * clone();
    virtual bool executionCondition();
};


#endif // ASYMMETRIC_H
