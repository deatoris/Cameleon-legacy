#ifndef ERASE_H
#define ERASE_H

#include<COperator.h>

class erase : public COperator
{
public:
    erase();
    virtual void exec();
    void initState();
    virtual COperator * clone();
};


#endif // ERASE_H
