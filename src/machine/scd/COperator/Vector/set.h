#ifndef SET_H
#define SET_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
class set : public COperator
{
public:
    set();
    virtual void exec();
    virtual COperator * clone();
};

#endif // SET_H
