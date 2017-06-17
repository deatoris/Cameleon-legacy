#ifndef CSDOPERATORMERGE_H
#define CSDOPERATORMERGE_H

#include<COperator.h>
#include<CData.h>


class CSDOperatorMerge :public COperator
{
public:
    CSDOperatorMerge();
    virtual void exec();
    virtual COperator * clone();
    virtual bool executionCondition();
};
#endif // CSDOPERATORMERGE_H
