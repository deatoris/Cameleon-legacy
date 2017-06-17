#include "DataVector.h"
#include<CGlossary.h>
#include<CUtilitySTL.h>
#include<fstream>
ostream& operator << (ostream& out, const vector<shared_ptr<CData> > & v){
    out <<(int)v.size()<<endl;
    for ( int i =0 ;(int) i < (int)v.size(); i++ ){
        out << v[i]->getKey() << "<$>" << v[i]->toString() <<"<$>"<<endl;
    }
    return out;
}

istream& operator >> (istream& in, vector<shared_ptr<CData> >& v){
    v.clear();
    int size;
    in >>size;

    for ( int i =0 ; i < size; i++ ){
        in.get();
        string datatype;
        datatype=SCD::ConvertString::getline(in,"<$>");
        string value;
        shared_ptr<CData> d(CGlossarySingletonClient::getInstance()->createData(datatype));
        value=SCD::ConvertString::getline(in,"<$>");
        d->fromString(value);
        v.push_back(d);
    }
    return in;
}

DataVector::DataVector()
    :CDataByFile<vector<shared_ptr<CData> > >()
{

    this->_key = DataVector::KEY;
    this->setExtension(".vec");
    this->setMode(CData::BYADDRESS);
}


string DataVector::KEY ="DATAVECTOR";

DataVector * DataVector::clone()
{
    return new DataVector;
}

//void DataVector::setDataByFile(vector<shared_ptr<CData> > * type){

//    string file = this->getFile();
//    ofstream  out(file.c_str());
//    if (out.fail())
//    {
//        cout<<"vector<shared_ptr<CData> >: cannot open file: "<<file<<endl;
//    }
//    else
//    {
//        out<<*(type);
//    }

//}
vector<shared_ptr<CData> > *  DataVector::cloneData(vector<shared_ptr<CData> > * type){
    vector<shared_ptr<CData> > *  _d = new vector<shared_ptr<CData> >;
    vector<shared_ptr<CData> >::iterator it;
    for (it =type->begin() ; it != type->end(); it++ ){
        _d->push_back((*it));
    }
    return _d;
}

//vector<shared_ptr<CData> > * DataVector::getDataByFile(){
//    vector<shared_ptr<CData> > * t = new vector<shared_ptr<CData> >;
//    string file = this->getFile();
//    ifstream  in(file.c_str());
//    if (in.fail())
//    {
//        cout<<"DataVector: cannot open file: "<<file<<endl;
//    }
//    else
//    {
//        in>>*(t);
//    }
//    return t;
//}
