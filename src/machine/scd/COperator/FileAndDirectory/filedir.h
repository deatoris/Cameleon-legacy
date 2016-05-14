#ifndef FILEDIR_H
#define FILEDIR_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
class filedir : public COperator
{
public:
    filedir();
    virtual void exec();
    virtual COperator * clone();
};


#endif // FILEDIR_H
