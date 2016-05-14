#ifndef SCDOPERATORLOADBYFILEDGENERIC_H
#define SCDOPERATORLOADBYFILEDGENERIC_H
#include<COperator.h>


class SCDOperatorLoadByFiledGeneric: public COperator
{
public:
    SCDOperatorLoadByFiledGeneric();
    virtual void exec();
    virtual COperator * clone();
};

#endif // SCDOPERATORLOADBYFILEDGENERIC_H
