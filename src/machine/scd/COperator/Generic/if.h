#ifndef IF_H
#define IF_H

#include<COperator.h>
#include<CData.h>


class If :public COperator
{
public:
    If();
    virtual void exec();
    virtual COperator * clone();
    void updateMarkingAfterExecution();
    bool ifvalue;
};

#endif // IF_H
