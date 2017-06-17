#ifndef COPY_H
#define COPY_H
#include<COperator.h>
#include <CData.h>


class Copy :public COperator
{
public:
    Copy();
    virtual void exec();
    virtual COperator * clone();
    virtual void action(string key);
};

#endif // COPY_H
