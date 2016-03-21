#ifndef ISFILE_H
#define ISFILE_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataBoolean.h>
class isFile : public COperator
{
public:
    isFile();
    virtual void exec();
    virtual COperator * clone();
};

#endif // ISFILE_H
