#ifndef CSDOPERATORHIDESHOWINTERFACE_H
#define CSDOPERATORHIDESHOWINTERFACE_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class CSDOperatorHideShowInterface : public COperator
{
public:
    CSDOperatorHideShowInterface();
    virtual void exec();
    virtual COperator * clone();
};

#endif // CSDOPERATORHIDESHOWINTERFACE_H
