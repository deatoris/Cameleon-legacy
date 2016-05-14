#include "DataBoolean.h"


DataBoolean::DataBoolean()
    :CDataByValue<bool>()
{
    this->_key = DataBoolean::KEY;
}
string DataBoolean::KEY ="DATABOOL";
DataBoolean * DataBoolean::clone()
{
    return new DataBoolean;
}

