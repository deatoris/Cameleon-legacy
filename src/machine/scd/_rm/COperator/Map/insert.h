#ifndef INSERT_H
#define INSERT_H

#include<COperator.h>
#include<DataMap.h>
#include<DataString.h>
#include<CData.h>
class insert : public COperator
{
public:
    insert();
    virtual void exec();
    virtual COperator * clone();
};

#endif // INSERT_H
