#ifndef VALUE_H
#define VALUE_H

#include<COperator.h>
#include<DataMap.h>
#include<DataString.h>
#include<CData.h>
class value : public COperator
{
public:
    value();
    virtual void exec();
    virtual COperator * clone();
};

#endif // VALUE_H
