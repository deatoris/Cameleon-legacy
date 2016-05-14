#ifndef OPERATORINSERTSTRING_H
#define OPERATORINSERTSTRING_H

#include<COperator.h>
class OperatorInsertString: public COperator
{
public:
    OperatorInsertString();
    virtual void exec();
    virtual COperator * clone();
};
#endif // OPERATORINSERTSTRING_H
