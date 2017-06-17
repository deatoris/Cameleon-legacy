#ifndef OPERATORITERATORVECTOR_H
#define OPERATORITERATORVECTOR_H

#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
class OperatorIteratorVector : public COperator
{
public:
    OperatorIteratorVector();
    virtual void exec();
    virtual COperator * clone();
    void initState();
     bool executionCondition();
     void updateMarkingAfterExecution();
private:
     bool _stilldata;
     DataVector::DATAVECTOR _vec;
     int _index;
     bool _lastshoot;
};

#endif // OPERATORITERATORVECTOR_H
