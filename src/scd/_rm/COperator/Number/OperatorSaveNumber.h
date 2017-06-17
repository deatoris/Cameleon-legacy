#ifndef OPERATORSAVENUMBER_H
#define OPERATORSAVENUMBER_H

#include<COperator.h>
class OperatorSaveNumber: public COperator
{
public:
    OperatorSaveNumber();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSAVENUMBER_H
