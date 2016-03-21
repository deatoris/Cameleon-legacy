#ifndef setTableElementTABLEELEMENT_H
#define setTableElementTABLEELEMENT_H

#include<COperator.h>

class setTableElement: public COperator
{
public:
    setTableElement();
    virtual void exec();
    virtual COperator * clone();
};
#endif // setTableElementTABLEELEMENT_H
