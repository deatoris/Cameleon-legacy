#ifndef SUP_H
#define SUP_H

#include<COperator.h>
#include<DataNumber.h>
#include<DataBoolean.h>

class sup : public COperator
{
public:
    sup();
    virtual void exec();
    virtual COperator * clone();
};

#endif // SUP_H
