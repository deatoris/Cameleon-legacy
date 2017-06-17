#ifndef GETCOLUMNNAME_H
#define GETCOLUMNNAME_H

#include<COperator.h>

class getTableHeaderCol: public COperator
{
public:
    getTableHeaderCol();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETCOLUMNNAME_H
