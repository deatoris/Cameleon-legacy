/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGProjectSerialization.h"
#include<CUtilitySTL.h>

CGProjectSerialization::CGProjectSerialization()
{   
    CLogger::getInstance()->registerComponent("SERIALIZE");
    CLogger::getInstance()->setComponentLevel("SERIALIZE", CLogger::getInstance()->fromString("INFO"));
}

QString CGProjectSerialization::makeAbsolute(QString it){
    QFileInfo info2(CGProject::getInstance()->getPath());
    QString instancePath = info2.absolutePath();
    return instancePath+"/"+it;
}

QString CGProjectSerialization::makeRelative(QString it){
    QFileInfo info(CGProject::getInstance()->getPath());
    QFileInfo itInfo(it);
    if(itInfo.isDir()){
        QDir d = info.absoluteDir();
        QString ds = d.absolutePath();
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

void CGProjectSerialization::loadWithoutGui(){
    CGProject::getInstance()->setName("");
    CGProject::getInstance()->setVersion("");
    QFile file(CGProject::getInstance()->getPath());
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

/*
 *The main
 *
 *Algo
 *operator: type/id
 *connection: opid:opid1:connectorid:connectorid1
 *inputs datas:controlid:controlConnectorId:datatype:content
 *inputs connection:
 *ignore controls inputs, take just outputs. controls must have only one output connector.
 *
 *Règles:
 *  * Pas de controlleur faisant office de "passeur" dans une boucle.
 *
*/


            /*
            #OPERATOR
            <operator id="1" type="sync" x="1035" y="-105.758" z="9" parentId="0"/>
            <operatorconnector opId="1" side="INPUT" connectorId="0" x="-49.2539" y="43.9324"/>
            <operatorconnector opId="1" side="INPUT" connectorId="1" x="-65.2423" y="9.97231"/>
            <operatorconnector opId="1" side="OUTPUT" connectorId="2" x="58.3959" y="30.7558"/>
            <operatorconnector opId="1" side="OUTPUT" connectorId="3" x="66" y="-0.0677037"/>

            #DATA (plugFromOutside)
            <compositioncontrol id="76" type="EditorNumber" x="352" y="26" z="1" width="186" height="83" parentId="0" content="1000000"/>
            <controlconnector controlid="76" connectorid="0" side="OUT" name="testexp" datatype="DATASTRING" content="ttt"/>

            #Connections
            #oper oper
            #control oper
            #composition oper
            #composition composition

            <connection connectorId1="0" operatorId1="0" connectorId2="2" operatorId2="1"/>
            <connection controlId="76" operatorIdOut="8" controlConnectorIdIn="0" operatorConnectorIdOut="1"/>
            <connection compositionId1="1" operatorId2="3" connectorId1="1" connectorId2="0"/>
            <connection operatorId1="0" compositionId2="1" connectorId1="0" connectorId2="1"/>
            <connection compositionId1="1" operatorId2="0" connectorId1="1" connectorId2="0"/>
            <connection compositionId1="2" compositionId2="3" connectorId1="1" connectorId2="1"/>
            <connection compositionId1="2" compositionId2="1" connectorId1="2" connectorId2="1"/>
            <connection compositionId1="3" compositionId2="2" connectorId1="1" connectorId2="1"/>
            <connection compositionId1="1" compositionId2="2" connectorId1="1" connectorId2="2"/>
            <connection operatorId1="1" compositionId2="2" connectorId1="0" connectorId2="1"/>
            <connection operatorId1="1" compositionId2="2" connectorId1="1" connectorId2="2"/>
            <connection compositionId1="2" operatorId2="1" connectorId1="1" connectorId2="0"/>
            <connection compositionId1="2" operatorId2="1" connectorId1="2" connectorId2="1"/>
            <connection operatorId1="2" compositionId2="3" connectorId1="2" connectorId2="1"/>
            <connection compositionId1="3" operatorId2="2" connectorId1="1" connectorId2="2"/>

            #compositions
            <composition id="1" name="test" x="-122" y="84" z="-700" informations="Select this, clicking left then, click right, juste here, select properties and edit this doc." viewx="-341" viewy="-184" viewscale="0" parentId="0"/>
            <connector compositionId="1" connectorId="1" side="OUT" name="" x="48.8639" y="34.8184"/>

*/

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }else{
                ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "operator"){
                        /*int id = xmlReader.attributes().value("connectorId").toString().toInt();
                        int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                        double x = xmlReader.attributes().value("x").toString().toDouble();
                        double y = xmlReader.attributes().value("y").toString().toDouble();
                        CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                        if(ope1!=0){
                            CGConnector* c = ope1->getView()->getConnectorById(id);
                            QPointF p(x,y);
                            if(c!=0)c->setPos(p);
                        }*/
                    }
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }else{
                ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "operatorconnector"){
                        /*int id = xmlReader.attributes().value("connectorId").toString().toInt();
                        int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                        double x = xmlReader.attributes().value("x").toString().toDouble();
                        double y = xmlReader.attributes().value("y").toString().toDouble();
                        CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                        if(ope1!=0){
                            CGConnector* c = ope1->getView()->getConnectorById(id);
                            QPointF p(x,y);
                            if(c!=0)c->setPos(p);
                        }*/
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }else{
                ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "controlconnector"){
                        /*int id = xmlReader.attributes().value("connectorId").toString().toInt();
                        int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                        double x = xmlReader.attributes().value("x").toString().toDouble();
                        double y = xmlReader.attributes().value("y").toString().toDouble();
                        CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                        if(ope1!=0){
                            CGConnector* c = ope1->getView()->getConnectorById(id);
                            QPointF p(x,y);
                            if(c!=0)c->setPos(p);
                        }*/
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }else{
                ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "connector"){
                        /*int id = xmlReader.attributes().value("connectorId").toString().toInt();
                        int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                        double x = xmlReader.attributes().value("x").toString().toDouble();
                        double y = xmlReader.attributes().value("y").toString().toDouble();
                        CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                        if(ope1!=0){
                            CGConnector* c = ope1->getView()->getConnectorById(id);
                            QPointF p(x,y);
                            if(c!=0)c->setPos(p);
                        }*/
                    }
                }
            }
        }
    }

            file.reset();
            xmlReader.clear ();
            xmlReader.setDevice(&file);
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                        xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
                    }else{
                        ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
                    }
                }else{
                    while (!xmlReader.atEnd()) {
                        xmlReader.readNext();
                        if (xmlReader.isStartElement()) {
                            if (xmlReader.name() == "connexion"){
                                /*int id = xmlReader.attributes().value("connectorId").toString().toInt();
                                int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                                double x = xmlReader.attributes().value("x").toString().toDouble();
                                double y = xmlReader.attributes().value("y").toString().toDouble();
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                                if(ope1!=0){
                                    CGConnector* c = ope1->getView()->getConnectorById(id);
                                    QPointF p(x,y);
                                    if(c!=0)c->setPos(p);
                                }*/
                            }
                        }
                    }
                }
            }
            file.reset();
            xmlReader.clear ();
            xmlReader.setDevice(&file);
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                        xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
                    }else{
                        ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
                    }
                }else{
                    while (!xmlReader.atEnd()) {
                        xmlReader.readNext();
                        if (xmlReader.isStartElement()) {
                            if (xmlReader.name() == "composition"){
                                /*int id = xmlReader.attributes().value("connectorId").toString().toInt();
                                int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                                double x = xmlReader.attributes().value("x").toString().toDouble();
                                double y = xmlReader.attributes().value("y").toString().toDouble();
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                                if(ope1!=0){
                                    CGConnector* c = ope1->getView()->getConnectorById(id);
                                    QPointF p(x,y);
                                    if(c!=0)c->setPos(p);
                                }*/
                            }
                        }
                    }
                }
            }


//    qDebug()<<"SECOND ======================";
//    qDebug()<<CGLayoutManager::getInstance()->getCurrentLayout()->getView()->verticalScrollBar()->value();
//    qDebug()<<CGLayoutManager::getInstance()->getCurrentLayout()->getView()->horizontalScrollBar()->value();
//    qDebug()<<"SECOND ======================";

    //start project logger


            //START MACHINE

}
