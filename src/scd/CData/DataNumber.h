#ifndef DataNumber_H
#define DataNumber_H
#include<CDataByValue.h>

class DataNumber : public CDataByValue<double>
{
public:
    static string KEY;
    DataNumber();
    virtual DataNumber * clone();
};
#endif // DataNumber_H
