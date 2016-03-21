#include "DataDictionnary.h"

DataDictionnary::DataDictionnary()
    :CDataByFile<pair<vector<string>,pair<string,string> > >()
{
    this->_key = DataDictionnary::KEY;
}
string DataDictionnary::KEY ="DATADictionnary";
DataDictionnary * DataDictionnary::clone(){
    return new DataDictionnary();
}
pair<vector<string>,pair<string,string> > * DataDictionnary::cloneData(pair<vector<string>, pair<string, string> > *t){
    pair<vector<string>,pair<string,string> > * tt = new pair<vector<string>,pair<string,string> >();
    *tt=make_pair(t->first,t->second);
    return tt;
}
