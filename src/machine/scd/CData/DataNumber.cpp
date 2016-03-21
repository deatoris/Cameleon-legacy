#include "DataNumber.h"


DataNumber::DataNumber()
    :CDataByValue<double>()
{
    this->_key = DataNumber::KEY;
}
string DataNumber::KEY ="DATANUMBER";
DataNumber *DataNumber::clone()
{
    return new DataNumber;
}
