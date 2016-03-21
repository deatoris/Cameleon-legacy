#ifndef DATADICTIONNARY_H
#define DATADICTIONNARY_H

#include<CDataByValue.h>
#include<string>
#include<vector>
#include<CUtilitySTL.h>
//first=datakeys, second.first=operatorkeys, second.second=controlkeys
using namespace SCD;
class DataDictionnary: public CDataByFile<pair<vector<string>,pair<string,string> > >
{
public:
    static string KEY;
    DataDictionnary();
    DataDictionnary * clone();
    pair<vector<string>,pair<string,string> > * cloneData(pair<vector<string>, pair<string, string> > *t);
};
#endif // DATADICTIONNARY_H
