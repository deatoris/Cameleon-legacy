#ifndef SETCOLUMNNAME_H
#define SETCOLUMNNAME_H

#include<COperator.h>

class setTableHeaderCol: public COperator
{
public:
    setTableHeaderCol();
    virtual void exec();
    virtual COperator * clone();
};
#endif // SETCOLUMNNAME_H
