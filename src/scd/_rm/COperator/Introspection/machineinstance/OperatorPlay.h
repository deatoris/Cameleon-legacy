#ifndef OPERATORPLAY_H
#define OPERATORPLAY_H

#include<COperator.h>
#include <CData.h>


class OperatorPlay :public COperator
{
public:
    OperatorPlay();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORPLAY_H
