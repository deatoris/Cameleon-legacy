#ifndef CONCAT_H
#define CONCAT_H

#include<COperator.h>
#include<DataString.h>
class concat : public COperator
{
public:
    concat();
    virtual void exec();
    virtual COperator * clone();
};

#endif // CONCAT_H
