#ifndef OperatorIteratorOutVector_H
#define OperatorIteratorOutVector_H

#include<COperator.h>
#include<CData.h>
#include<DataVector.h>
#include<DataNumber.h>
class OperatorIteratorOutVector : public COperator
{
public:
    OperatorIteratorOutVector();
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

#endif // OperatorIteratorOutVector_H
