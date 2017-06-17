#ifndef CSDOPERATORPROJECTPATH_H
#define CSDOPERATORPROJECTPATH_H

#include<COperator.h>
#include<DataString.h>
class CSDOperatorProjectPath : public COperator
{
public:
    CSDOperatorProjectPath();
    virtual void exec();
    virtual COperator * clone();
};

#endif // CSDOPERATORPROJECTPATH_H
