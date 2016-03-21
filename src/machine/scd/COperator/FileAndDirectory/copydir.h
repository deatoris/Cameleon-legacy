#ifndef COPYDIR_H
#define COPYDIR_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class copydir : public COperator
{
public:
    copydir();
    virtual void exec();
    virtual COperator * clone();
    bool cpDir(QString srcPath, QString dstPath);
};
#endif // COPYDIR_H
