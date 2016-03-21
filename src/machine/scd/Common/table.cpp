#include "table.h"
#include<CUtilitySTL.h>
Table::Table()
{}

void Table::setInformation(string info){
    _info = info;
}

Table::Table(int col,int raw, string value)
    :_data(raw,vector<string>(col,value)),_header(col)
{}
Table::Table(const Table &m)
    :_data(m.sizeRow(),vector<string>(m.sizeCol(),"")),_header(m.sizeCol())
{
    for(int r=0;r<(int)_data.size();r++)
        for(int c=0;c<(int)_data[0].size();c++)
        {
            this->_data[r][c]=m(c,r);
        }

    for(int c=0;c<(int)_data[0].size();c++)
    {
        this->setHeader(c,m.getHeader(c));
    }
}

void Table::setHeader(int col,string header){
    _header[col] = header;
}

string Table::getHeader(int col ) const{
    return _header[col];
}

int Table::sizeRow()const {
    return (int)_data.size();
}
int Table::sizeCol()const{
    if(_data.empty()==true)
        return 0;
    return (int)_data[0].size();
}

void Table::resize(int col,int raw)throw(std::string){
    if(col<0||raw<0){
        string msg = "Out of range 0<=coll and 0<=row";
        throw(msg);
    }
    else{
        _data.resize(raw);
        for(int k =0;k<sizeRow();k++){
            _data[k].resize(col);
        }
        int size= _header.size();
        _header.resize(col);
        for(int i=size;i<col;i++){
            _header[i]=SCD::UtilityString::Any2String(i);
        }

    }
}
const string & Table::operator ()(  pair<int,int>  col_raw)
const
{
    return  this->_data[col_raw.second][col_raw.first];
}
string & Table::operator ()( pair<int,int>  col_raw)
{
    return  this->_data[col_raw.second][col_raw.first];
}

const string & Table::operator ()( int  col,int raw)
const throw(std::string)
{
    if(isValid(col,raw)==true)
        return  this->_data[raw][col];
    else{
        string msg = "Out of range 0<=col<sizeCol and 0<=row<sizeRow";
        throw(msg);
    }
}
bool Table::isValid(int  col,int raw)const{
    if(col>=0&&col< this->sizeCol()&&raw>=0&&raw<sizeRow())
        return true;
    else
        return false;
}

string & Table::operator ()( int  col,int raw) throw(std::string)
{
    if(isValid(col,raw)==true)
        return  this->_data[raw][col];
    else{
        string msg = "Out of range 0<=col<sizeCol and 0<=row<sizeRow";
        throw(msg);
    }
}
void Table::load(string file)throw(std::string){
    ifstream  in(file.c_str());
    if (in.fail())
    {
        throw("Table: cannot open file: "+file);
    }
    else
    {
        in>>*this;
    }
}
void Table::save(string file)throw(std::string){
    ofstream  out(file.c_str());
    if (out.fail())
    {
        throw("Table: cannot open file: "+file);
    }
    else
    {
        out<<*this;
    }
}

void Table::deleteRow(int rowmin, int rowmax)throw(std::string){
    if(rowmin<0||rowmax>=this->sizeRow()||rowmin>rowmax)
        throw(string("Out of range in getTableRow, you must have 0<=rowmin<=rowmax<sizeRow"));
    this->_data.erase (this->_data.begin()+rowmin,this->_data.begin()+rowmax+1 );
}

void Table::deleteCol(int colmin,int colmax)throw(std::string){
    if(colmin<0||colmax>=this->sizeRow()||colmin>colmax)
        throw(string("Out of range in getTablecol, you must have 0<=colmin<=colmax<sizecol"));
    for(int i=0;i<sizeRow();i++)
        this->_data[i].erase ( this->_data[i].begin() + colmin,this->_data[i].begin() + colmax+1);
}
Table Table::getRow(int rowmin,int rowmax)const throw(std::string){
    if(rowmin<0||rowmax>=this->sizeRow()||rowmin>rowmax)
        throw(string("Out of range in getTableRow, you must have 0<=rowmin<=rowmax<sizeRow"));
    Table t(this->sizeCol(),rowmax-rowmin+1);
    for(int j=0;j<this->sizeCol();j++)
        for(int i=0;i<rowmax-rowmin+1;i++)
            t(j,i)=this->operator ()(j,rowmin+i);
    return t;
}

Table Table::getCol(int colmin,int colmax)const throw(std::string){
    if(colmin<0||colmax>=this->sizeRow()||colmin>colmax)
        throw(string("Out of range in getTablecol, you must have 0<=colmin<=colmax<sizecol"));
    Table t(colmax-colmin+1,this->sizeRow());
    for(int j=0;j<colmax-colmin+1;j++)
        for(int i=0;i<this->sizeRow();i++)
            t(j,i)=this->operator ()(j+colmin,i);
    return t;
}


void Table::setRow(int row,const Table &t)throw(std::string){
    if(row<0||row>this->sizeRow())
        throw(string("Out of range in insertTableRow, you must have 0<=row<sizeRow"));
    if(this->sizeCol()!=t.sizeCol())
        throw(string("Col size must be equal in insertTableRow"));


    this->resize(this->sizeCol(),this->sizeRow()+t.sizeRow());
    for(int i=row+t.sizeRow();i<this->sizeRow();i++)
        for(int j=0;j<this->sizeCol();j++)
            this->operator ()(j,i)=this->operator ()(j,i-t.sizeRow());


    for(int i=0;i<t.sizeRow();i++)
        for(int j=0;j<t.sizeCol();j++)
            this->operator ()(j,i+row) = t(j,i);

}
void Table::setCol(int col,const Table & t)throw(std::string){
    if(col<0||col>this->sizeCol())
        throw(string("Out of range in insertTableRow, you must have 0<=col<sizecol"));
    if(this->sizeRow()!=t.sizeRow())
        throw(string("Row size must be equal in insertTableRow"));

    this->resize(this->sizeCol()+t.sizeCol(),this->sizeRow());
    for(int i=0;i<this->sizeRow();i++)
        for(int j=col+t.sizeCol();j<this->sizeCol();j++)
            this->operator ()(j,i)=this->operator ()(j-t.sizeCol(),i);
    for(int i=0;i<t.sizeRow();i++)
        for(int j=0;j<t.sizeCol();j++)
            this->operator ()(j+col,i) = t(j,i);
}

ostream& operator << (ostream& out, const Table& m)
{
    out<<'#'<<m.sizeCol()<<" "<<m.sizeRow()<<endl;
    for(int j=0;j<m.sizeCol();j++)
    {
        out<<m.getHeader(j);
        out<<"<%>";
    }
    out<<endl;
    for(int i=0;i<m.sizeRow();i++)
    {
        for(int j=0;j<m.sizeCol();j++)
        {
            out<<m(j,i);
            out<<"<%>";
        }
        out<<endl;
    }

    return out;
}
istream& operator >> (istream& in, Table& m)
{


    m._data.clear();
    char c;
    in >>c;
    int sizex;
    in >> sizex;
    int sizey;
    in >> sizey;
    m.resize(sizex,sizey);
    in.get();
    for(int j=0;j<m.sizeCol();j++)
    {
        string str;
        str=SCD::UtilityString::getline(in,"<%>");
        m.setHeader(j,str);
    }

    for(int i=0;i<m.sizeRow();i++)
    {
        in.get();
        for(int j=0;j<m.sizeCol();j++)
        {
            string str;
            str=SCD::UtilityString::getline(in,"<%>");
            m(j,i)=str;
        }
    }

    return in;
}
