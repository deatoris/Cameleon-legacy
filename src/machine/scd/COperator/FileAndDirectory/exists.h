#ifndef EXISTS_H
#define EXISTS_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataBoolean.h>
class exists : public COperator
{
public:
    exists();
    virtual void exec();
    virtual COperator * clone();
};

#endif // EXISTS_H
