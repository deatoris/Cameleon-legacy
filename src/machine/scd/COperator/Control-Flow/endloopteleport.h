#ifndef ENDLOOPTELEPORT_H
#define ENDLOOPTELEPORT_H

#include<COperator.h>
#include<CData.h>


class endforteleport :public COperator
{
    bool _endloop;
public:
    endforteleport();
    virtual void exec();
    virtual COperator * clone();
    bool executionCondition();
    virtual void updateMarkingAfterExecution();
    void action(string key);
};
#endif // ENDLOOPTELEPORT_H
