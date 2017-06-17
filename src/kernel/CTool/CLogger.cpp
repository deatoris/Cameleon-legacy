/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CLogger.h"
CLogger* CLogger::loggerInstance = NULL;
CLoggerInstance* CLoggerInstance::loggerInstance = NULL;

CLoggerInstance::CLoggerInstance(){
    if(QFile::exists("../log/cameleon.log")){
        QFile::remove("../log/cameleon.log");
    }
    instancelogger.setFileName("../log/cameleon.log");
    if (!instancelogger.open(QFile::WriteOnly | QFile::Text)) {
        qDebug() << "ERROR: can't open instance logger file ";
    }
}

void CLoggerInstance::log(QString message){
    QString messageToPrint = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz")+" - INSTANCE - "+message;
    QTextStream out(&instancelogger);
    out << messageToPrint+"\n";
}

CLoggerInstance* CLoggerInstance::getInstance(){
    if(NULL != loggerInstance){
        return loggerInstance;
    }else{
        loggerInstance = new CLoggerInstance();
        return loggerInstance;
    }
}

CLogger::CLogger()
{
    path = "";
    active = false;
}

bool CLogger::getActive(){
    return active;
}

void CLogger::setActive(bool a){
    this->active = a;
}

void CLogger::clear(){
    mapComponentLevel.clear();
}

CLogger* CLogger::getInstance(){
    if(NULL != loggerInstance){
        return loggerInstance;
    }else{
        loggerInstance = new CLogger();
        return loggerInstance;
    }
}

void CLogger::start(){
    if(QFile::exists(path+"/cameleon.log")){
        QFile::remove(path+"/cameleon.log");
    }
    logger.setFileName(path+"/cameleon.log");
    ////qDebug << "";
    ////qDebug << "==========================";
    ////qDebug << "start logger " << path << "/cameleon.log";
    if (!logger.open(QFile::WriteOnly | QFile::Text)) {
        ////qDebug << "ERROR: can't open logger file from " << path;
    }
}

QString CLogger::toString(LVL level){
    if(level == ERROR){
        return "ERRO";
    }else if(level == WARNING){
        return "WARN";
    }else if(level == INFO){
        return "INFO";
    }else if(level == DEBUG){
        return "DEBG";
    }else if(level == SHUTDOWN){
        return "DOWN";
    }
    return "DOWN";
}

CLogger::LVL CLogger::fromString(QString level){
    if(level.compare("ERRO") == 0){
        return ERROR;
    }else if(level.compare("WARN") == 0){
        return WARNING;
    }else if(level.compare("INFO") == 0){
        return INFO;
    }else if(level.compare("DEBG") == 0){
        return DEBUG;
    }else if (level.compare("DOWN") == 0){
        return SHUTDOWN;
    }else
        return ERROR;
}

void CLogger::log(QString name, LVL level, QString message){
    if(active){
        QMutexLocker locker(&_mutex);
        QString messageToPrint;
        if(mapComponentLevel.contains(name)
                && mapComponentLevel.value(name) >= level
                && mapComponentLevel.value(name) != CLogger::SHUTDOWN){
            messageToPrint = toString(level)+" - "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz")+" - "+name+" - "+message;
            QTextStream out(&logger);
            out << messageToPrint+"\n";
            ////qDebug << messageToPrint;
            emit log(messageToPrint);
        }
    }
}

void CLogger::registerComponent(QString name){
    if(!mapComponentLevel.contains(name)){
        mapComponentLevel.insert(name, CLogger::ERROR);
        CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Init component "+name);

    }
}

void CLogger::setComponentLevel(QString name, LVL level){
    if(!mapComponentLevel.contains(name)){
        mapComponentLevel.insert(name, level);
    }else{
        mapComponentLevel.remove(name);
        mapComponentLevel.insert(name, level);
    }
}

void CLogger::setPath(QString path){
    this->path = path;
}

QMap<QString, CLogger::LVL> CLogger::getMap(){
    return mapComponentLevel;
}
