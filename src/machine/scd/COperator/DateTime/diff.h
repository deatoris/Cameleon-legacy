#ifndef DIFF_H
#define DIFF_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataDateTime.h>
class diff : public COperator
{
public:
    diff();
    virtual void exec();
    virtual COperator * clone();
};
#endif // DIFF_H
