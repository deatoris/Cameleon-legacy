#ifndef FROMSTRING_H
#define FROMSTRING_H

#include<COperator.h>
#include<CData.h>

class fromstring :public COperator
{
public:
    fromstring();
    virtual void exec();
    virtual COperator * clone();
};

#endif // FROMSTRING_H
