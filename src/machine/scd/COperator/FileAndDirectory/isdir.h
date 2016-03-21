#ifndef ISDIR_H
#define ISDIR_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataBoolean.h>
class isDir : public COperator
{
public:
    isDir();
    virtual void exec();
    virtual COperator * clone();
};

#endif // ISDIR_H
