#include "DataString.h"

DataString::DataString()
    :CDataByValue<string>()
{
    this->_key = DataString::KEY;

}
string DataString::KEY ="DATASTRING";
DataString * DataString::clone(){
    return new DataString();
}

string DataString::toString()
{
    return *_data.get();
}
void DataString::fromString(string str)
{
    *_data = str;
}
