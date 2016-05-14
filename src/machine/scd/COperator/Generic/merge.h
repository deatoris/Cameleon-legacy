#ifndef MERGE_H
#define MERGE_H

#include<COperator.h>
#include<CData.h>


class OperatorMergeGeneric :public COperator
{
public:
    OperatorMergeGeneric();
    virtual void exec();
    virtual COperator * clone();
    virtual bool executionCondition();
    void updateMarkingAfterExecution();
    virtual void action(string key);
};

#endif // MERGE_H
