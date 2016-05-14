#ifndef GETSIZE_H
#define GETSIZE_H
#include<COperator.h>
class getSizeTable: public COperator
{
public:
    getSizeTable();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETSIZE_H
