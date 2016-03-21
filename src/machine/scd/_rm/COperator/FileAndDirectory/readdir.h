#ifndef READDIR_H
#define READDIR_H
#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class readdir : public COperator
{
public:
    readdir();
    virtual void exec();
    virtual COperator * clone();
    virtual void initState();
};
#endif // READDIR_H
