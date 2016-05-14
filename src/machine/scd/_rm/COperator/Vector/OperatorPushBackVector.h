#ifndef OPERATORPUSHBACKVECTOR_H
#define OPERATORPUSHBACKVECTOR_H

#include<COperator.h>
class OperatorPushBackVector : public COperator
{
public:
    OperatorPushBackVector();
    virtual void exec();
    virtual COperator * clone();
};


#endif // OPERATORPUSHBACKVECTOR_H
