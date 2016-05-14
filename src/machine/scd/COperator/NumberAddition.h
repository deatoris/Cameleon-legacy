#ifndef NUMBERADDITION_H
#define NUMBERADDITION_H

#include<COperator.h>
#include<DataNumber.h>
class NumberAddition : public COperator
{
public:
    NumberAddition();
    virtual void exec();
    virtual COperator * clone();
};


#endif // NUMBERADDITION_H
