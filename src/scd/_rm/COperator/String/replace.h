#ifndef REPLACE_H
#define REPLACE_H

#include<COperator.h>
#include<DataString.h>
class OperatorReplaceGeneric : public COperator
{
public:
    OperatorReplaceGeneric();
    virtual void exec();
    virtual COperator * clone();
};

#endif // REPLACE_H
