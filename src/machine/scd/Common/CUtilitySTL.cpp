#include "CUtilitySTL.h"
#include<cmath>
using namespace std;
void SCD::UtilityString::IntFixedDigit2String(int value,int digitnumber, string & s)
{
    long int  number =pow (10.,digitnumber);
    long int tempvalue = value/number;
    value-=tempvalue*number;
    s.clear();
    for(int i =digitnumber-1;i>=0;i--)
    {
        long int number =pow (10.,i);
        long int tempvalue = value/number;
        value-=tempvalue*number;
        string str;
        Any2String(tempvalue,str);
        s+=str;
    }
}
string SCD::UtilityString::getline(istream& in,string del)throw(std::string){


    string str;
    string temp;
    int index=0;
    char c;
    while(index<(int)del.size()){
        c = in.get();
        if (in.good()){
            if(c==del.operator [](index))
            {
                temp+=c;
                index++;
            }
            else
            {
                if(index!=0){
                    str+=temp;
                    temp.clear();
                    index=0;
                }
                if(c==del.operator [](index))
                {
                    temp+=c;
                    index++;
                }
                else
                    str+=c;
            }
        }
        else
            throw "[ERROR] Reach the end of the istream without finding the delimeter";

    }
    return str;
}

