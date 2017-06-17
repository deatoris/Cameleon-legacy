#ifndef DATADICTIONNARY_H
#define DATADICTIONNARY_H

#include<CDataByValue.h>
#include<string>
#include<vector>
#include<CUtilitySTL.h>
//first=datakeys, second.first=operatorkeys, second.second=controlkeys
using namespace SCD;
class DataDictionnary: public CDataByValue<pair<vector<string>,pair<string,string> > >
{
public:
    static string KEY;
    DataDictionnary();
    DataDictionnary * clone();
};
#endif // DATADICTIONNARY_H
