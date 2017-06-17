#ifndef TABLE_H
#define TABLE_H
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
using namespace std;

class Table
{
private:
    string _info;
    vector<vector<string> > _data;
    vector<string> _header;

public:
    void setInformation(string info);
    typedef pair<int,int> E;
    typedef string F;
    explicit Table();
    Table(int col,int raw,string value="");
    Table(const Table &m);
    int sizeCol()const;
    int sizeRow()const;
    void setHeader(int col,string header);
    string getHeader(int col )const;
    void resize(int col,int raw)throw(std::string);
    const string & operator ()( pair<int,int>  col_raw)const;
    string & operator ()( pair<int,int>  col_raw);

    const string & operator ()( int  col,int raw)const throw(std::string);
    string & operator ()( int  col,int raw)throw(std::string);
    bool isValid(int  col,int raw) const;
    void deleteRow(int rawmin,int rawmax)throw(std::string);
    void deleteCol(int colmin,int colmax)throw(std::string);
    Table getRow(int rowmin,int rowmax)const throw(std::string);
    Table getCol(int colmin,int colmax)const throw(std::string);
    void setRow(int row,const Table &t)throw(std::string);
    void setCol(int col,const Table &t)throw(std::string);

    friend ostream& operator << (ostream& out, const Table& m);
    friend istream& operator >> (istream& in, Table& m);
    void load(string file)throw(std::string);
    void save(string file)throw(std::string);
};

#endif // TABLE_H
