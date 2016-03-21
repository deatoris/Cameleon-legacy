#ifndef DIV_H
#define DIV_H

#include<COperator.h>
#include<DataNumber.h>
class numberdiv : public COperator
{
public:
    numberdiv();
    virtual void exec();
    virtual COperator * clone();
};
#endif // DIV_H
