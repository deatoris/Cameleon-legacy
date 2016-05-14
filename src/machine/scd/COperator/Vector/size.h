#ifndef SIZE_H
#define SIZE_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
class size : public COperator
{
public:
    size();
    virtual void exec();
    virtual COperator * clone();
};
#endif // SIZE_H
