#ifndef COPERATORASSERT_H
#define COPERATORASSERT_H

#include<COperator.h>
#include<CData.h>

class COperatorAssert :public COperator
{
public:
    COperatorAssert();
    virtual void exec();
    virtual COperator * clone();
};

#endif // COPERATORASSERT_H
