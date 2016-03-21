#ifndef INSTRING_H
#define INSTRING_H

#include<COperator.h>
#include<DataString.h>
#include<DataBoolean.h>
class instring : public COperator
{
public:
    instring();
    virtual void exec();
    virtual COperator * clone();
};

#endif // INSTRING_H
