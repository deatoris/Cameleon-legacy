#ifndef KEYS_H
#define KEYS_H

#include<COperator.h>
#include<DataMap.h>
#include<DataString.h>
#include<DataVector.h>
#include<CData.h>
class keys : public COperator
{
public:
    keys();
    virtual void exec();
    virtual COperator * clone();
};

#endif // KEYS_H
