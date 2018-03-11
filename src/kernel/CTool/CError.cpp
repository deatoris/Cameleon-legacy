/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CError.h"
#include<ostream>
using namespace std;

CErrorBuilder::CErrorBuilder( string msg,string  file )
    :_msg(msg),_file(file)
{
}

CErrorBuilder::~CErrorBuilder() throw()
{

}

std::string CErrorBuilder::msg() const throw()
{
    return _msg;
}
std::string CErrorBuilder::file() const throw()
{
    return _file;
}
ostream & operator<<( ostream & os, const CErrorBuilder& v){
    os<<"[SERVER][ERROR][BUILDER] "<<v._msg<<" in file "<<v._file<<endl;
    return os;
}

CErrorDumpData::CErrorDumpData(string msg, int operatorplugoutid, int plugoutindex ,int operatorpluginid, int pluginindex )
    :_msg(msg),_operatorplugoutid(operatorplugoutid), _plugoutindex(plugoutindex) ,_operatorpluginid(operatorpluginid), _pluginindex(pluginindex)
{}
std::string CErrorDumpData::msg() const throw()
{
    return _msg;
}

CErrorDumpData::~CErrorDumpData() throw(){

}
int  CErrorDumpData::gettPlugInIndex() const throw(){
    return _pluginindex;
}
int CErrorDumpData::gettPlugOutIndex() const throw(){
    return _plugoutindex;
}

int  CErrorDumpData::getOperatorPlugInId() const throw(){
    return _operatorpluginid;
}
int CErrorDumpData::getOperatorPlugOutId() const throw(){
    return _operatorplugoutid;
}
ostream & operator<<( ostream & os, const CErrorDumpData& v){
    os<<"[SERVER][ERROR][DUMP] :"<<v._msg<<" dump between operator "<<v._operatorpluginid<<" and plugin "<<v._pluginindex <<" and operator "<<v._operatorplugoutid<<" and plug out "<<v._plugoutindex <<endl;
    return os;
}
