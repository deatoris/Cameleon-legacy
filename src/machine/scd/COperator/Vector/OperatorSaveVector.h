#ifndef OPERATORSAVEVECTOR_H
#define OPERATORSAVEVECTOR_H

#include<COperator.h>
class OperatorSaveVector: public COperator
{
public:
    OperatorSaveVector();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSAVEVECTOR_H
