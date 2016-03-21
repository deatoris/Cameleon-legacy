#ifndef OPERATORSIZESTRING_H
#define OPERATORSIZESTRING_H

#include<COperator.h>
class OperatorSizeString: public COperator
{
public:
    OperatorSizeString();
    virtual void exec();
    virtual COperator * clone();
};
#endif // OPERATORSIZESTRING_H
