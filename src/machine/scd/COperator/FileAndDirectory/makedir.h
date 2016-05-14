#ifndef MAKEDIR_H
#define MAKEDIR_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class MakeDir : public COperator
{
public:
    MakeDir();
    virtual void exec();
    virtual COperator * clone();
};

#endif // MAKEDIR_H
