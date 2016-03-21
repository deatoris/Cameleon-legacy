/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CDataGeneric.h"
#include<CGlossary.h>
#include<CPlug.h>
CDataGeneric::CDataGeneric()
{
}
string CDataGeneric::KEY="CDATAGENERIC";
CData::Key CDataGeneric::getKey(){
    return KEY;
}
CData * CDataGeneric::clone(){
    return new CDataGeneric();
}

CData * CDataGeneric::copy(){
    return new CDataGeneric();
}

void CDataGeneric::dumpReception(CData * data){
    if(dynamic_cast<CDataGeneric *>(data)==false)
    {
        try
        {


            CData* d (CGlossarySingletonServer::getInstance()->createData(data->getKey()));
            CPlug * plug = this->getPlug();
            d->setPlug(plug);
            plug->setData(d);
            d->dumpReception(data);

        } catch ( const std::exception & e )
        {
            std::cerr << e.what()<<" in the Data Factory for the type"<<data->getKey()<<endl;
        }
    }
}

void CDataGeneric::readXML(QXmlStreamReader* xmlReader){

}

void CDataGeneric::writeXML(QXmlStreamWriter* xmlWriter){

}
