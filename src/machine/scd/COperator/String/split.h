#ifndef SPLIT_H
#define SPLIT_H
#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class split : public COperator
{
public:
    split();
    virtual void exec();
    virtual COperator * clone();
};
#endif // SPLIT_H
