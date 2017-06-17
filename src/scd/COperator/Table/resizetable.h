#ifndef RESIZETABLE_H
#define RESIZETABLE_H
#include<COperator.h>
class ResizeTable: public COperator
{
public:
    ResizeTable();
    virtual void exec();
    virtual COperator * clone();
};

#endif // RESIZETABLE_H
