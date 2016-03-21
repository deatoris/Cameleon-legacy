#ifndef OPERATORGENERATERANDOMFILEDATA_H
#define OPERATORGENERATERANDOMFILEDATA_H


#include<COperator.h>
#include<CData.h>

class OperatorGenerateRandomFileData :public COperator
{
public:
    OperatorGenerateRandomFileData();
    virtual void exec();
    virtual COperator * clone();
};


#endif // OPERATORGENERATERANDOMFILEDATA_H
