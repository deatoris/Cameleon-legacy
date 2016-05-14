#ifndef CUTILITYSTL_H
#define CUTILITYSTL_H
#include<string>
#include<istream>
#include <sstream>
#include <vector>
using namespace std;
namespace SCD
{

template <typename T>
int sgn(T val)
{
    return (val > T(0)) - (val < T(0));
}

class UtilityString
{
public:

    static string getline(istream& in,string del)throw(std::string);
    template<typename T>
    static bool String2Any(string s,  T & Dest )
    {

        std::istringstream iss(s );

        return iss >> Dest != 0;
    }
    template<typename T>
    static bool Any2String(T Value, string & s)
    {
        s.clear();
        std::ostringstream oss;
        bool temp = oss << Value;
        s= oss.str();
        return temp;
    }
    template<typename T>
    static string Any2String(T Value)
    {
        string s;
        s.clear();
        std::ostringstream oss;
        oss << Value;
        s= oss.str();
        return s;
    }
    static void IntFixedDigit2String(int value,int digitnumber, string & s);
};


template<class _T1, class _T2>
ostream& operator << (ostream &out, const pair<_T1,_T2> &arg)
{

    out << arg.first << "<P>" << arg.second<<"<P>";
    return out;
}
template<class _T1, class _T2>
istream& operator >>(istream& in, pair<_T1,_T2> &arg)
{
    string str;
    str = UtilityString::getline( in, "<P>" );
    UtilityString::String2Any(str,arg.first );
    str = UtilityString::getline( in,"<P>" );
    UtilityString::String2Any(str,arg.second );
    return in;
}

template<class _T1>
ostream& operator << (ostream &out, const vector<_T1> &arg)
{
    out<<(int)arg.size()<<"<V>";
    for(int i=0;i<(int)arg.size();i++)
        out << arg[i] << "<V>";
    return out;
}
template<class _T1>
istream& operator >> (istream &in,vector<_T1> &arg)
{
    arg.clear();
    string str;
    str = UtilityString::getline( in, "<V>" );
    int size;
    UtilityString::String2Any(str,size );
    arg.resize(size);
    for(int i=0;i<(int)arg.size();i++){
        _T1 t;
        str = UtilityString::getline( in, "<V>" );
        UtilityString::String2Any(str,t );
        arg.push_back(t);
    }
    return in;
}

}

#endif // CUTILITYSTL_H
