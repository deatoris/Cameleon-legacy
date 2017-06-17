#ifndef rmDir_H
#define rmDir_H

#include <QtCore>
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
class rmDir : public COperator
{
public:
    rmDir();
    virtual void exec();
    virtual COperator * clone();
    bool eraseDir(QString dirPath);
};

#endif // rmDir_H
