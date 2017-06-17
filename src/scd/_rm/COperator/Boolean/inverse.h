#ifndef INVERSE_H
#define INVERSE_H

#include<COperator.h>
#include<DataBoolean.h>
class inverse : public COperator
{
public:
    inverse();
    virtual void exec();
    virtual COperator * clone();
};


#endif // INVERSE_H
