#include "DataMap.h"
#include<CGlossary.h>
#include<istream>
#include <fstream>
#include<CUtilitySTL.h>
ostream& operator << (ostream& out, const map<string,shared_ptr<CData>  > & m){
    out <<(int)m.size()<<endl;
    map<string,shared_ptr<CData>  >::const_iterator it;
    for (it =m.begin() ; it != m.end(); it++ ){
        out << (*it).second->getKey() << endl;
        out << (*it).first << "<$>" << (*it).second->toString() <<"<$>"<<endl;
    }
    return out;
}

istream& operator >> (istream& in, map<string,shared_ptr<CData>  > & m){
    m.clear();
    int size;
    in >>size;
    for ( int i =0 ; i < size; i++ ){
        string datatype2;
        in>>datatype2;
        in.get();
        CData * d2 = CGlossarySingletonClient::getInstance()->createData(datatype2);
        string data1,data2;
        data1=SCD::UtilityString::getline(in,"<$>");
        data2=SCD::UtilityString::getline(in,"<$>");
        in.get();
        d2->fromString(data2);
        m[data1]=shared_ptr<CData>(d2);
    }
    return in;
}

DataMap::DataMap()
    :CDataByFile<map<string,shared_ptr<CData>  > >()
{
    _data=shared_ptr<map<string,shared_ptr<CData>  > >(new map<string,shared_ptr<CData>  >);
    this->_key = DataMap::KEY;
    this->setExtension(".map");
    this->setMode(CData::BYCOPY);
}


string DataMap::KEY ="DATAMAP";

DataMap *DataMap::clone()
{
    return new DataMap;
}

void DataMap::setDataByFile(shared_ptr<map<string,shared_ptr<CData>  > > type){

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
void DataMap::setDataByCopy(shared_ptr<map<string,shared_ptr<CData>  > > type){
    _data->clear();
    map<string,shared_ptr<CData>  >::iterator it;
//    for (it =type->begin() ; it != type->end(); it++ ){
//        shared_ptr<CData> d
//        cout<< (*it).first<<" "<<*((*it).second.get())<<endl;
//    }

    for (it =type->begin() ; it != type->end(); it++ ){
        shared_ptr<CData> c ((*it).second->clone());
        c->dumpReception((*it).second.get());
        _data->operator []( (*it).first) = c;
    }

}
shared_ptr<map<string,shared_ptr<CData>  > > DataMap::getDataByFile(){
    shared_ptr<map<string,shared_ptr<CData>  > > t(new map<string,shared_ptr<CData>  >);
    string file = this->getFile();
    ifstream  in(file.c_str());
    if (in.fail())
    {
        cout<<"DataMap: cannot open file: "<<file<<endl;
    }
    else
    {
        in>>*(t.get());
    }
    return t;
}
