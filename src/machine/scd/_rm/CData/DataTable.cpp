#include "DataTable.h"

DataTable::DataTable()
    :CDataByFile<Table >()
{
    this->_key = DataTable::KEY;
    this->setExtension(".tab");
}

string DataTable::KEY ="DataTable";

DataTable * DataTable::clone()
{
    return new DataTable;
}

//void DataTable::setDataByFile(Table * type){

//    string file = this->getFile();
//    ofstream  out(file.c_str());
//    if (out.fail())
//    {
//        cout<<"map<string,shared_ptr<CData>  >: cannot open file: "<<file<<endl;
//    }
//    else
//    {
//        out<<*(type);
//    }
//}
Table *  DataTable::cloneData(Table * type){

    return new Table(*type);
}
//Table * DataTable::getDataByFile(){
//    Table *  t = new Table;
//    string file = this->getFile();
//    ifstream  in(file.c_str());
//    if (in.fail())
//    {
//        cout<<"DataTable: cannot open file: "<<file<<endl;
//    }
//    else
//    {
//        in>>*(t);
//    }
//    return t;
//}
