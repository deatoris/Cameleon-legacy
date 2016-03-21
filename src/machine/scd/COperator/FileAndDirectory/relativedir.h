#ifndef RELATIVEDIR_H
#define RELATIVEDIR_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
class relativedir : public COperator
{
public:
    relativedir();
    virtual void exec();
    virtual COperator * clone();
};

#endif // RELATIVEDIR_H
