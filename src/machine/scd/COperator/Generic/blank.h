#ifndef BLANK_H
#define BLANK_H

#include<COperator.h>

class blank : public COperator
{
public:
    blank();
    virtual void exec();
    virtual COperator * clone();
};


#endif // BLANK_H
