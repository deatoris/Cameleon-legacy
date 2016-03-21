#ifndef CURRENT_H
#define CURRENT_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataDateTime.h>
class current : public COperator
{
public:
    current();
    virtual void exec();
    virtual COperator * clone();
};


#endif // CURRENT_H
