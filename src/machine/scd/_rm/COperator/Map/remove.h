#ifndef REMOVE_H
#define REMOVE_H

#include<COperator.h>
#include<DataMap.h>
#include<DataString.h>
#include<CData.h>
class removemapelement : public COperator
{
public:
    removemapelement();
    virtual void exec();
    virtual COperator * clone();
};

#endif // REMOVE_H
