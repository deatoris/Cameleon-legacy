#ifndef DIVINT_H
#define DIVINT_H

#include<COperator.h>
#include<DataNumber.h>
class numberdivint : public COperator
{
public:
    numberdivint();
    virtual void exec();
    virtual COperator * clone();
};
#endif // DIVINT_H
