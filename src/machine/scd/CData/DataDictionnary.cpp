#include "DataDictionnary.h"

DataDictionnary::DataDictionnary()
    :CDataByValue<pair<vector<string>,pair<string,string> > >()
{
    this->_key = DataDictionnary::KEY;
    this->setMode(CData::BYADDRESS);
}
string DataDictionnary::KEY ="DATADictionnary";
DataDictionnary * DataDictionnary::clone(){
    return new DataDictionnary();
}
