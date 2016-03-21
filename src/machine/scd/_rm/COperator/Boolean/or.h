#ifndef OR_H
#define OR_H

#include<COperator.h>
#include<DataBoolean.h>
class Or : public COperator
{
public:
    Or();
    virtual void exec();
    virtual COperator * clone();
};


#endif // OR_H
