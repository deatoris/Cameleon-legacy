#ifndef LOCKER_H
#define LOCKER_H

#include<COperator.h>
#include<CData.h>


class locker :public COperator
{
public:
    locker();
    virtual void exec();
    virtual COperator * clone();
    void action(string key);
    virtual bool executionCondition();
    virtual void updateMarkingAfterExecution();
};
#endif // LOCKER_H
