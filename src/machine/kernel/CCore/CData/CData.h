/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CDATA_H
#define CDATA_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include<CException.h>
using namespace std;
class CPlug;
class CData
{
public:
    enum Mode{
        BYFILE,BYADDRESS,BYCOPY,BYVALUE
    };
    typedef string Key;
    CData();
    virtual ~CData();
    static Key KEY;
    virtual Key getKey()const;
    virtual CData * clone();
    virtual CData * copy();

    void setPlug(CPlug * plug);
    CPlug * getPlug();

    void dumpReception(CData * data)throw(std::string);

    virtual void dumpReceptionCast(CData * data)throw(CException);


    virtual void fromString(string );
    virtual string toString()const;
    void setExtension(string extension);
    string getExtension()const;
    bool isDataAvailable();
    void setIsDataAvailable(bool isdataavailable);
    void setMode(Mode mode);
    Mode getMode();
    virtual CData * morpherMode( Mode mode,string filename="" );
//    virtual   void releaseData();
//    virtual   void deleteData();

protected:
    string _extension;
    string _key;
    CPlug * _plug;
    Mode _mode;
    bool _isdata;
};
typedef CData DataGeneric;
#endif // CDATA_H
