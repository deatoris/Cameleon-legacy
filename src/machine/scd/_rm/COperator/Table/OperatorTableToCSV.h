#ifndef OPERATORTABLETOCSV_H
#define OPERATORTABLETOCSV_H

#include<COperator.h>
class OperatorTableToCSV: public COperator
{
public:
    OperatorTableToCSV();
    virtual void exec();
    virtual COperator * clone();
};

#endif // OPERATORTABLETOCSV_H
