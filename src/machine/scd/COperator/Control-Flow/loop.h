#ifndef LOOP_H
#define LOOP_H

#include<COperator.h>
#include<CData.h>


class loop :public COperator
{
public:
    loop();
    virtual void exec();
    virtual COperator * clone();
    void updateMarkingAfterExecution();

private:
    int count;
};

#endif // LOOP_H
