#ifndef GET_H
#define GET_H

#include <QtCore>
#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
#include<DataBoolean.h>
#include<DataString.h>
class get : public COperator
{
public:
    get();
    virtual void exec();
    virtual COperator * clone();
};

class compareVector : public COperator
{
public:
    compareVector();
    virtual void exec();
    virtual COperator * clone();
};

class concatVector : public COperator
{
public:
    concatVector();
    virtual void exec();
    virtual COperator * clone();
};

class findVector : public COperator
{
public:
    findVector();
    virtual void exec();
    virtual COperator * clone();
};

#endif // GET_H
