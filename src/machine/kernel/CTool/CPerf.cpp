/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CPerf.h"
#include "CLogger.h"
CPerf* CPerf::perfInstance = NULL;

CPerf::CPerf()
{
//    CLogger::getInstance()->registerComponent("PERF");
}

CPerf* CPerf::getInstance(){
    if(NULL != perfInstance){
        return perfInstance;
    }else{
        perfInstance = new CPerf();
        return perfInstance;
    }
}
void CPerf::probeStart(QString name){
    if(mapNameTime.contains(name)){
        QTime* t = mapNameTime.value(name);
        t->restart();
    }else{
        QTime* telement = new QTime();
        mapNameTime.insert(name,telement);
        telement->start();
    }
}

int CPerf::probeEnd(QString name){
    if(mapNameTime.contains(name)){
        QTime* t = mapNameTime.value(name);
        int i = t->elapsed();
        CLogger::getInstance()->log("PERF",CLogger::INFO,name+" "+printableProbe(i));
        return i;
    }
    return -1;
}

QString CPerf::printableProbe(int lastPerf){
    QString sperf = "";
    long perfSecondes = lastPerf/1000;
    long perfMin = perfSecondes/60;
    long perfH = perfMin/60;
    if(perfMin > 1){
        QString str;
        str = QString::number(perfMin);
        sperf += str;
        sperf += "min";
        return sperf;
    }
    if(perfH > 1){
        QString str;
        str = QString::number(perfH);
        sperf += str;
        sperf += "h";
        return sperf;
    }
    if(perfSecondes < 1){
        QString str;
        str = QString::number(perfSecondes);
        sperf += str;
        sperf += "ms";
        return sperf;
    }
    if(perfSecondes >= 1){
        QString str;
        str = QString::number(perfSecondes);
        sperf += str;
        sperf += "s";
        return sperf;
    }
    QString str;
    str = QString::number(lastPerf);
    sperf += str;
    sperf += "ms";
    return sperf;
}
