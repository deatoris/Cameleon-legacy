#ifndef RESIZE_H
#define RESIZE_H

#include<COperator.h>

class resize : public COperator
{
public:
    resize();
    virtual void exec();
    virtual COperator * clone();
};

#endif // RESIZE_H
