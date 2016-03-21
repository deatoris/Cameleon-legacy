#ifndef Aasync_H
#define Aasync_H

#include<COperator.h>
#include<CData.h>

class async :public COperator
{
public:
    async();
    virtual void exec();
    virtual COperator * clone();
    void action(string key);
    bool executionCondition();
    void updateMarkingAfterExecution();
};

#endif // Aasync_H
