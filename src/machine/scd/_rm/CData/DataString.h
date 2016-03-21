#ifndef CSDDATASTRING_H
#define CSDDATASTRING_H
#include<CDataByValue.h>
#include<string>
using namespace std;
class DataString: public CDataByValue<string>
{
public:
    static string KEY;
    DataString();
    DataString * clone();
//    virtual string toString();
//    virtual void fromString(string str);
};

#endif // CSDDATASTRING_H
