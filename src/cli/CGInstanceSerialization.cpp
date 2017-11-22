/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGInstanceSerialization.h"
#include "CGInstance.h"
#include "CGProject.h"

CGInstanceSerialization::CGInstanceSerialization()
{
}

void CGInstanceSerialization::load(){
    //read or create instance file
    CGInstance::getInstance()->setPath("instance.conf");
    QFile fileInstance(CGInstance::getInstance()->getPath());

    QFileInfo infos(CGInstance::getInstance()->getPath());
    CGInstance::getInstance()->setProjectPath(infos.absoluteDir().path()+"/project/");

    if(fileInstance.exists()){
        CGInstance::getInstance()->setFirstLaunch(false);
        CGInstanceSerialization::read();
    }else{
        CGInstance::getInstance()->setFirstLaunch(true);
        qDebug() << ("\nCreate a clean caméléon instance.");
        exit(0);
        //CGInstanceSerialization::createInstance(CGInstance::getInstance()->getPath());
    }
}

void CGInstanceSerialization::read(){
    QFile file(CGInstance::getInstance()->getPath());
    QFileInfo fileInfos(file);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    QXmlStreamReader xmlReader;

    xmlReader.setDevice(&file);

    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("instance") == 0
                    || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The instance configuration file is not compatible with this ide version."));
            }else{
//                //qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "ressource"){
                        CGInstance::getInstance()->setRessourcePath(makeAbsolute(xmlReader.attributes().value("path").toString()));
                    }
                    if (xmlReader.name() == "autosave"){
                        //CGCommandManager::getInstance()->setStep(xmlReader.attributes().value("step").toString().toInt());
                    }
                    if (xmlReader.name() == "lib"){
                        CGInstance::getInstance()->setLibPath(makeAbsolute(xmlReader.attributes().value("path").toString()));
                    }
                    if (xmlReader.name() == "plugin"){
                        CGInstance::getInstance()->setPluginPath(makeAbsolute(xmlReader.attributes().value("path").toString()));
                    }
                    if (xmlReader.name() == "activatedlib"){
                        QString name = xmlReader.attributes().value("path").toString();
                        QString version = xmlReader.attributes().value("version").toString();
                        QFileInfo info(makeAbsolute(name));
                        if(info.isFile()) CGInstance::getInstance()->activeLib(name,version,true);
                    }
                    if (xmlReader.name() == "last"){
                        QString path = makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGInstance::getInstance()->setCurrentProjectPath(path);
                    }
                    if (xmlReader.name() == "previous"){
                        QString path = makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGInstance::getInstance()->addRecentProject(path);
                    }

                    if (xmlReader.name() == "patterns"){
                        CGInstance::getInstance()->setPatternPath(makeAbsolute(xmlReader.attributes().value("path").toString()));
                    }
                }
            }
        }
    }
    CGInstance::getInstance()->loadPatterns();
    CGInstance::getInstance()->loadRessources();
    CGInstance::getInstance()->loadInstalledLibInfos();
    CGInstance::getInstance()->setFirstLaunch(false);
}

QString CGInstanceSerialization::makeAbsolute(QString it){
    QFileInfo info2(CGInstance::getInstance()->getPath());
    QString instancePath = info2.absolutePath();
    return instancePath+"/"+it;
}

QString CGInstanceSerialization::makeRelative(QString it){
    QFileInfo info(CGInstance::getInstance()->getPath());
    QFileInfo itInfo(it);
    if(itInfo.isDir()){
        QDir d = info.absoluteDir();
        QString s = it+"/s.pgm";
        QString toReturn  = d.relativeFilePath(s);
        QFileInfo i(toReturn);
        return i.path();
    }else{
        QString toDir = info.absolutePath();
        QDir d(toDir);
        return d.relativeFilePath(it);
    }
}
