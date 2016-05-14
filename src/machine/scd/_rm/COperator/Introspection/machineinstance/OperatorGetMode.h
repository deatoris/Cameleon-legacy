#ifndef GETMODE_H
#define GETMODE_H

#include<COperator.h>
#include <CData.h>


class GetMode :public COperator
{
public:
    GetMode();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETMODE_H
