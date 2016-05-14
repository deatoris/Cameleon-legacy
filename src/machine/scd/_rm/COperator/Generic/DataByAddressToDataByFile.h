#ifndef DATABYADDRESSTODATABYFILE_H
#define DATABYADDRESSTODATABYFILE_H

#include<COperator.h>
#include<CData.h>

class DataByAddressToDataByFile :public COperator
{
public:
    DataByAddressToDataByFile();
    virtual void exec();
    virtual COperator * clone();
};

#endif // DATABYADDRESSTODATABYFILE_H
