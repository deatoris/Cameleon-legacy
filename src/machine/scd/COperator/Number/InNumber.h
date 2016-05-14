#ifndef INNUMB_H
#define INNUMB_H

#include<COperator.h>


class innumber : public COperator
{
public:
    innumber();
    virtual void exec();
    virtual COperator * clone();
};


#endif // INNUMB_H
