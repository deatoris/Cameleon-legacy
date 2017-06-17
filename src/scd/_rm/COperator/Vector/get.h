#ifndef GET_H
#define GET_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
class get : public COperator
{
public:
    get();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GET_H
