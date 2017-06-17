#ifndef GETTYPE_H
#define GETTYPE_H

#include<COperator.h>
#include<CData.h>

class gettype :public COperator
{
public:
    gettype();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETTYPE_H
