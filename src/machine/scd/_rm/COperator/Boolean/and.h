#ifndef AND_H
#define AND_H

#include<COperator.h>
#include<DataBoolean.h>
class And : public COperator
{
public:
    And();
    virtual void exec();
    virtual COperator * clone();
};

#endif // AND_H
