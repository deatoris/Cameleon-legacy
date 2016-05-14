#ifndef RMFILE_H
#define RMFILE_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class rmfile : public COperator
{
public:
    rmfile();
    virtual void exec();
    virtual COperator * clone();
    bool cpDir(QString srcPath, QString dstPath);
};

#endif // RMFILE_H
