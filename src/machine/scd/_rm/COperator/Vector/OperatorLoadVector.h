#ifndef OPERATORLOADVECTOR_H
#define OPERATORLOADVECTOR_H

#include<COperator.h>
class OperatorLoadVector: public COperator
{
public:
    OperatorLoadVector();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORLOADVECTOR_H
