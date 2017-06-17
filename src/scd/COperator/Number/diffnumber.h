#ifndef DIFFNUMBER_H
#define DIFFNUMBER_H

#include<COperator.h>
#include<DataNumber.h>
class diffnumber : public COperator
{
public:
    diffnumber();
    virtual void exec();
    virtual COperator * clone();
};

#endif // DIFFNUMBER_H
