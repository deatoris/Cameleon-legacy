#ifndef ADD_H
#define ADD_H

#include<COperator.h>
#include<DataNumber.h>
class add : public COperator
{
public:
    add();
    virtual void exec();
    virtual COperator * clone();
};
#endif // ADD_H
