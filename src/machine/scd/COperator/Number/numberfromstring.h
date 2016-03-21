#ifndef NUMBERFROMSTRING_H
#define NUMBERFROMSTRING_H

#include<COperator.h>
#include<DataNumber.h>
#include<DataString.h>
class NumberFromString : public COperator
{
public:
    NumberFromString();
    virtual void exec();
    virtual COperator * clone();
};

#endif // NUMBERFROMSTRING_H
