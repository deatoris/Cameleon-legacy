#ifndef COUNTER_H
#define COUNTER_H

#include<COperator.h>
#include<CData.h>

class counter :public COperator
{
public:
    counter();
    virtual void exec();
    virtual COperator * clone();
    virtual void initState();

private:
    int count;
};

#endif // COUNTER_H
