#ifndef OPERATORSORTVECTOR_H
#define OPERATORSORTVECTOR_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
class OperatorSortVector : public COperator
{
public:
    OperatorSortVector();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORSORTVECTOR_H
