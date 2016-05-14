#ifndef GETYEAR_H
#define GETYEAR_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataDateTime.h>
class getyear : public COperator
{
public:
    getyear();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GETYEAR_H
