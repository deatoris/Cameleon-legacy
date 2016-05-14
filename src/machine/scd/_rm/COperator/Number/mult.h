#ifndef MULT_H
#define MULT_H

#include<COperator.h>
#include<DataNumber.h>
class mult : public COperator
{
public:
    mult();
    virtual void exec();
    virtual COperator * clone();
};

#endif // MULT_H
