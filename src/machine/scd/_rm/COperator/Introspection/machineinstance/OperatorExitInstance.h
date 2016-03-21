#ifndef OPERATOREXITINSTANCE_H
#define OPERATOREXITINSTANCE_H

#include<COperator.h>
#include <CData.h>


class OperatorExitInstance :public COperator
{
public:
    OperatorExitInstance();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATOREXITINSTANCE_H
