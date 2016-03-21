#ifndef TOSTRING_H
#define TOSTRING_H

#include<COperator.h>
#include<CData.h>

class tostring :public COperator
{
public:
    tostring();
    virtual void exec();
    virtual COperator * clone();
};

#endif // TOSTRING_H
