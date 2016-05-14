#ifndef OPERATORBASENAMESTRING_H
#define OPERATORBASENAMESTRING_H

#include<COperator.h>
#include<DataString.h>
class OperatorBaseNameString : public COperator
{
public:
    OperatorBaseNameString();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORBASENAMESTRING_H
