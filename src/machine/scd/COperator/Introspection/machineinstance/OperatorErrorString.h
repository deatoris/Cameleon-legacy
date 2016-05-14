#ifndef OPERATORERRORSTRING_H
#define OPERATORERRORSTRING_H

#include<COperator.h>
class OperatorErrorString: public COperator
{
    Q_OBJECT
public slots:
        void errorOperatorSlot(COperator::Id opid,string msg);
public:
    OperatorErrorString();
    virtual void exec();
    virtual COperator * clone();

        virtual bool executionCondition();
};

#endif // OPERATORERRORSTRING_H
