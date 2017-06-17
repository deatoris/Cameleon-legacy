#ifndef DATABYFILETODATABYADDRESS_H
#define DATABYFILETODATABYADDRESS_H

#include<COperator.h>
#include<CData.h>

class DataByFileToDataByAddress :public COperator
{
public:
    DataByFileToDataByAddress();
    virtual void exec();
    virtual COperator * clone();
};


#endif // DATABYFILETODATABYADDRESS_H
