#ifndef OPERATORPROJECTTMPPATHSTRING_H
#define OPERATORPROJECTTMPPATHSTRING_H

#include<COperator.h>
#include<DataString.h>
class OperatorProjectTmpPath : public COperator
{
public:
    OperatorProjectTmpPath();
    virtual void exec();
    virtual COperator * clone();
};
#endif // OPERATORPROJECTTMPPATHSTRING_H
