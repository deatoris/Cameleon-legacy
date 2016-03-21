#ifndef EXITLOOP_H
#define EXITLOOP_H

#include<COperator.h>
#include<CData.h>


class exitloop :public COperator
{
public:
    exitloop();
    virtual void exec();
    virtual COperator * clone();
    bool executionCondition();

};
#endif // EXITLOOP_H
