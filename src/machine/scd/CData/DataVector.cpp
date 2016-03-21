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
        datatype=SCD::UtilityString::getline(in,"<$>");
        string value;
        shared_ptr<CData> d(CGlossarySingletonClient::getInstance()->createData(datatype));
        value=SCD::UtilityString::getline(in,"<$>");
        d->fromString(value);
        v.push_back(d);
    }
    return in;
}

DataVector::DataVector()
    :CDataByFile<vector<shared_ptr<CData> > >()
{
    _data=shared_ptr<vector<shared_ptr<CData> > >(new vector<shared_ptr<CData> >);
    this->_key = DataVector::KEY;
    this->setExtension(".vec");
    this->setMode(CData::BYCOPY);
}


string DataVector::KEY ="DATAVECTOR";

DataVector * DataVector::clone()
{
    return new DataVector;
}

void DataVector::setDataByFile(shared_ptr<vector<shared_ptr<CData> > > type){

    string file = this->getFile();
    ofstream  out(file.c_str());
    if (out.fail())
    {
        cout<<"vector<shared_ptr<CData> >: cannot open file: "<<file<<endl;
    }
    else
    {
        out<<*(type.get());
    }

}
void DataVector::setDataByCopy(shared_ptr<vector<shared_ptr<CData> > > type){
    _data->clear();
    vector<shared_ptr<CData> >::iterator it;
    for (it =type->begin() ; it != type->end(); it++ ){
        shared_ptr<CData> c ((*it)->clone());
        c->dumpReception((*it).get());
        _data->push_back(c);
    }
}

shared_ptr<vector<shared_ptr<CData> > > DataVector::getDataByFile(){
    shared_ptr<vector<shared_ptr<CData> > > t(new vector<shared_ptr<CData> >);
    string file = this->getFile();
    ifstream  in(file.c_str());
    if (in.fail())
    {
        cout<<"DataVector: cannot open file: "<<file<<endl;
    }
    else
    {
        in>>*(t.get());
    }
    return t;
}
