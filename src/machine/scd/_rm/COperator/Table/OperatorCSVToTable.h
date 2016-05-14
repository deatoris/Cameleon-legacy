#ifndef OPERATORCSVTOTABLE_H
#define OPERATORCSVTOTABLE_H

#include<COperator.h>
#include <QtCore>
class OperatorCSVToTable: public COperator
{
public:
    OperatorCSVToTable();
    virtual void exec();
    virtual COperator * clone();
};
#endif // OPERATORCSVTOTABLE_H
