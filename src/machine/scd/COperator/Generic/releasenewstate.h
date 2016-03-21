#ifndef RELEASENEWSTATE_H
#define RELEASENEWSTATE_H

#include<COperator.h>
#include<CData.h>

class releasenewstate :public COperator
{
public:
    releasenewstate();
    virtual void exec();
    virtual COperator * clone();
    virtual bool executionCondition();
};

#endif // RELEASENEWSTATE_H
