#ifndef DATABOOLEAN_H
#define DATABOOLEAN_H
#include<CDataByValue.h>
class DataBoolean : public CDataByValue<bool>
{
public:
    static string KEY;
    DataBoolean();
    virtual DataBoolean * clone();
};

#endif // DATABOOLEAN_H
