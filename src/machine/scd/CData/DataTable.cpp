#include "DataTable.h"

DataTable::DataTable()
    :CDataByFile<Table >()
{
    _data=shared_ptr<Table >(new Table);
    this->_key = DataTable::KEY;
    this->setMode(BYADDRESS);
    this->setExtension(".tab");
}

string DataTable::KEY ="DataTable";

DataTable * DataTable::clone()
{
    return new DataTable;
}

void DataTable::setDataByFile(shared_ptr<Table > type){

    string file = this->getFile();
    ofstream  out(file.c_str());
    if (out.fail())
    {
        cout<<"map<string,shared_ptr<CData>  >: cannot open file: "<<file<<endl;
    }
    else
    {
        out<<*(type.get());
    }
}
void DataTable::setDataByCopy(shared_ptr<Table > type){

    _data = shared_ptr<Table>(new Table(*(type.get())));
}
shared_ptr<Table > DataTable::getDataByFile(){
    shared_ptr<Table   > t(new Table);
    string file = this->getFile();
    ifstream  in(file.c_str());
    if (in.fail())
    {
        cout<<"DataTable: cannot open file: "<<file<<endl;
    }
    else
    {
        in>>*(t.get());
    }
    return t;
}
