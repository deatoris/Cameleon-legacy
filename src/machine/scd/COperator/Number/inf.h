#ifndef INF_H
#define INF_H

#include<COperator.h>
#include<DataNumber.h>
#include<DataBoolean.h>
class inf : public COperator
{
public:
    inf();
    virtual void exec();
    virtual COperator * clone();
};

#endif // INF_H
