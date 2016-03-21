#ifndef NUMBERTOSTRING_H
#define NUMBERTOSTRING_H

#include<COperator.h>
#include<DataNumber.h>
#include<DataString.h>
class NumberToString : public COperator
{
public:
    NumberToString();
    virtual void exec();
    virtual COperator * clone();
};
#endif // NUMBERTOSTRING_H
