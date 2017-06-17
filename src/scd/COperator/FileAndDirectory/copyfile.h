#ifndef COPYFILE_H
#define COPYFILE_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class copyfile : public COperator
{
public:
    copyfile();
    virtual void exec();
    virtual COperator * clone();
};

#endif // COPYFILE_H
