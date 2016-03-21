#ifndef uid_H
#define uid_H

#include<COperator.h>

class uid : public COperator
{
public:
    uid();
    virtual void exec();
    virtual COperator * clone();
};

#endif // uid_H
