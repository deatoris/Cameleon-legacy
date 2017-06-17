#ifndef OPERATORBREAKPOINT_H
#define OPERATORBREAKPOINT_H

#include<COperator.h>
#include <CData.h>


class OperatorBreakPoint :public COperator
{
public:
    OperatorBreakPoint();
    virtual void exec();
    virtual COperator * clone();
};


#endif // OPERATORBREAKPOINT_H
