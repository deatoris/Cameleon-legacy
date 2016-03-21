#ifndef POP_H
#define POP_H

#include <QtCore>
#include<COperator.h>
#include<CDataGeneric.h>
#include<DataVector.h>
#include<DataNumber.h>
class pop : public COperator
{
public:
    pop();
    virtual void exec();
    virtual COperator * clone();
};
#endif // POP_H
