/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CArguments.h"
//#include "ControlEditorArgument.h"
#include "CGInstance.h"
#include "CGProject.h"
#include <iostream>

QString CArguments::getReadmeFromComposition(QString cmPath){
    QString readme = "No synopsis";

    QFile file(cmPath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return readme;
    }
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);

    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "readme"){
                        readme = xmlReader.attributes().value("text").toString();
                    }
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    file.close();
    return readme;
}
#include "CGProject.h"
QList<argument> CArguments::getArgumentsFromComposition(QString cmPath){
    QFile file(cmPath);
    QList<argument> l;
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return l;
    }

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);

    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {

                    if (xmlReader.name() == "readme"){
                        QString readme = xmlReader.attributes().value("text").toString();
                        CGProject::getInstance()->setReadme(readme);
                    }

                    //read input control and exposure parameters
                    //<controlconnector controlid="77" connectorid="0" side="OUT" name="edt" datatype="DATANUMBER" content="100"/>
                    if (xmlReader.name() == "controlconnector"){
                        QString name = xmlReader.attributes().value("name").toString();
                        QString datatype = xmlReader.attributes().value("datatype").toString();
                        QString content = xmlReader.attributes().value("content").toString();
                        if(name != ""){
                            argument a;
                            a.key = name;
                            a.value = content;
                            a.datatype = datatype;
                            l.append(a);
                        }
                    }


                    /*
                     2.2 / REFACTOR: argument a rebrancher
                     *if (xmlReader.name() == "control"){
                        QString name = xmlReader.attributes().value("type").toString();
                        QString content = xmlReader.attributes().value("content").toString();
                        string s = ControlEditorArgument::KEY;
                        if(name.compare(s.c_str()) == 0){
                            argument a;
                            QStringList listContent = content.split("<$>");
                            a.key = "-"+listContent[0];
                            a.value = listContent[1];
                            a.description = listContent[2];
                            if(listContent[3].compare("TRUE") == 0) a.optional = true;
                            if(listContent[3].compare("FALSE") == 0) a.optional = false;
                            l.append(a);
                        }
                    }
                    if (xmlReader.name() == "compositioncontrol"){
                        QString name = xmlReader.attributes().value("type").toString();
                        QString content = xmlReader.attributes().value("content").toString();
                        string s = ControlEditorArgument::KEY;
                        if(name.compare(s.c_str()) == 0){
                            argument a;
                            QStringList listContent = content.split("<$>");
                            a.key = "-"+listContent[0];
                            a.value = listContent[1];
                            a.description = listContent[2];
                            if(listContent[3].compare("TRUE") == 0) a.optional = true;
                            if(listContent[3].compare("FALSE") == 0) a.optional = false;
                            l.append(a);
                        }
                    }*/
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    file.close();
    return l;
}

argument CArguments::getArgument(QString key, QList<argument> fromArguments){
    //inner keys: compositionPath (must be valide path), help flag (true or false)
    argument a;
    a.key="NONE";
    foreach(argument arg, fromArguments){
        if(arg.key.compare(key) == 0){
            a = arg;
        }
    }
    return a;
}

QList<argument> CArguments::getArgumentsFromApplication(){
    QStringList argsL = qApp->arguments();
    QList<argument> l;
    /*
        lcm example.cm
        lcm example.cm message="hello world!" occures="10"
        lcm example.cm -h
        lcm -h
     */

    //arg 1 == -h et argL.size > 2
    //arg1 == *.cm et anyarg == -h

    QRegExp rx("*.cm");
    rx.setPatternSyntax(QRegExp::Wildcard);
    rx.exactMatch("README.txt");        // returns true


    foreach(QString arg, argsL){
        //qDebug() << arg;
        if(arg != argsL[0]){
            if(arg.compare("-h") == 0){
                argument a;
                a.key="-h";
                l.append(a);
            }else if(rx.exactMatch(arg)){
                if(!isFileExist(arg)){
                    qDebug() << "[ERROR] *.cm file doesn't exist!";
                    CArguments::printCameleonHelp();
                    exit(0);
                }else{
                    argument a;
                    a.key="-cm";
                    a.value=arg;
                    l.append(a);
                }
            }else{
                QStringList li = arg.split("=");
                if(li.size()==2){
                    argument a;
                    a.key=li[0];
                    a.value=li[1];
                    l.append(a);
                }else{
                    CArguments::printCameleonHelp();
                    exit(0);
                }
            }
        }
    }
    return l;
}

bool CArguments::isCompositionForced(QList<argument> commandArgs){
    bool ret = false;
    foreach(argument a, commandArgs){
        if(a.key.compare("-cm")==0) ret = true;
    }
    return ret;
}

QString CArguments::getCompositionFromAppArgs(QList<argument> commandArgs){
    QString cmp = "";
    foreach(argument a, commandArgs){
        if(a.key.compare("-cm")==0) cmp = a.value;
    }
    return cmp;
}

QString CArguments::mode(QList<argument> commandArgs){
    QString ret = "IDE";
    foreach(argument a, commandArgs){
        if(a.key.compare("-mode")==0) ret = a.value;
    }
    return ret;
}

bool CArguments::isFileExist(QString file){
    QFileInfo info(file);
    if(!info.exists()) qDebug()<<"[ERROR] File *.cm does not exist.";
    return info.exists();
}

bool CArguments::isDouble(QString key, QList<argument> commandArgs){
    int i = 0;
    foreach(argument arg, commandArgs){
        if(arg.key == key){
            i++;
        }
    }
    if(i>1){
        qDebug()<<"[ERROR] Argument "<<key<<" is setted twice. Please see help.";
        return true;
    }else{
        return false;
    }
}

bool CArguments::checkSurface(QList<argument> commandArgs){
    foreach(argument a, commandArgs){
        if(a.key.compare("-cm") == 0 && !isFileExist(a.value)) return false;
    }
    return true;
}

bool CArguments::isCameleonHelp(QList<argument> commandArgs){
    bool retCm = false;
    bool reth = false;
    foreach(argument a, commandArgs){
        if(a.key.compare("-h")==0) reth = true;
        if(a.key.compare("-cm")==0) retCm = true;
    }
    return reth&&!retCm;
}

bool CArguments::isCompositionHelp(QList<argument> commandArgs){
    bool retCm = false;
    bool reth = false;
    foreach(argument a, commandArgs){
        if(a.key.compare("-h")==0) reth = true;
        if(a.key.compare("-cm")==0) retCm = true;
    }
    return reth&&retCm;
}

bool CArguments::checkArguments(QList<argument>  commandArgs, QList<argument> compositionArgs ){ // QString : "-h", "-mode=$MODE", "-cm=$cmPath", "-key"
    foreach(argument cmparg, compositionArgs){
        if(!cmparg.optional){
            bool check = false;
            foreach(argument cmdarg, commandArgs){
                if(cmdarg.key.compare(cmparg.key) == 0){
                    check = true;
                }
            }
            /*   if(!check){
                qDebug()<<"[ERROR] Argument "<<cmparg.key<<" is mandatory. Please see help.";
                return false;
            }*/
        }
    }
    foreach(argument cmdarg, commandArgs){
        bool check = false;
        if(cmdarg.key.compare("-cm")!=0){
            foreach(argument cmparg, compositionArgs){
                if(cmdarg.key.compare(cmparg.key) == 0){
                    check = true;
                }
            }
            if(!check){
                qDebug()<<"[ERROR] Argument "<<cmdarg.key<<" does not exist. Please see help.";
                return false;
            }
            if(CArguments::isDouble(cmdarg.key, commandArgs)){
                qDebug()<<"[ERROR] Argument "<<cmdarg.key<<" is setted twice. Please see help.";
                return false;
            }
        }

    }
    return true;
}

void CArguments::printCameleonHelp(){
    QFile file(CGInstance::getInstance()->getRessourcePath()+"/lcm-man.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        std::cout << file.errorString().toStdString();
    }

    QTextStream in(&file);
    std::cout << "\n";
    while(!in.atEnd()) {
        QString line = in.readLine();
        std::cout << line.toStdString() <<"\n";
    }
    std::cout << "\n";

    file.close();

    /*
        IDE, integrated development environment (controller + composer),
        GUI, Graphical user interface (only controller),
        CLI command line interface.
    */
}

void CArguments::printCompositionHelp(QString compositionPath, QList<argument>  compositionArgs){
    //getReadme texte de la composition
    std::cout << "\n";
    std::cout << CGProject::getInstance()->getReadme().toStdString();
    std::cout << "\n";
    /*
    qDebug()<<("\nPATH");
    qDebug()<<" "<<compositionPath;
    qDebug()<<("\nSYNOPSIS");
    qDebug()<<"\n"<<CArguments::getReadmeFromComposition(compositionPath);
    qDebug() << ("DESCRIPTION");
    foreach(argument arg, compositionArgs){
        if(arg.optional)qDebug()<<"[OPTIONAL] "<<arg.key<<":    "<<arg.description;
        if(!arg.optional)qDebug()<<"[MANDATORY] "<<arg.key<<":    "<<arg.description;
    }
*/
}
