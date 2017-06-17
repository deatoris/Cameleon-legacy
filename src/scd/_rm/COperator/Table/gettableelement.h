#ifndef GETTABLEELEMENT_H
#define GETTABLEELEMENT_H

#include<COperator.h>

class getTableElement: public COperator
{
public:
    getTableElement();
    virtual void exec();
    virtual COperator * clone();
};
#endif // GETTABLEELEMENT_H
