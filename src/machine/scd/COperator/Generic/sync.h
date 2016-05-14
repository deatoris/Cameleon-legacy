#ifndef SYNC_H
#define SYNC_H

#include<COperator.h>
#include<CData.h>


class syncGen :public COperator
{
public:
    syncGen();
    void exec();
    COperator * clone();
    bool executionCondition();
    void action(string key);
};

#endif // SYNC_H
