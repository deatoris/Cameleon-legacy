#ifndef READFILE_H
#define READFILE_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class readfile : public COperator
{
public:
    readfile();
    virtual void exec();
    virtual COperator * clone();
};
#endif // READFILE_H
