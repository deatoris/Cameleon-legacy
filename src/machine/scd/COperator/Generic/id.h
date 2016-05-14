#ifndef ID_H
#define ID_H

#include<COperator.h>
#include<CData.h>

class id :public COperator
{
public:
    id();
    virtual void exec();
    virtual COperator * clone();
    virtual void action(string key);
};

#endif // ID_H
