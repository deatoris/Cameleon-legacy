#ifndef PRIORITY_H
#define PRIORITY_H

#include<COperator.h>
#include<CData.h>

class priority :public COperator
{
public:
    priority();
    virtual void exec();
    virtual COperator * clone();
};
#endif // PRIORITY_H
