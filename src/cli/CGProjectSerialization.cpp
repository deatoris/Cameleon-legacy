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
    qDebug() << ("loadWithoutGui");
    QFile file(CGProject::getInstance()->getPath());
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }



    /*
 *The main
 *
 *Besoin des structures suivantes:
 *Lecture:
 *
 *operatorsMap <id, TYPE>
 *actionOperator <id, action> output
 *
 *dataMap <id:id, TYPE>
 *dataMapContent <id:id, CONTENT>
 *
 *connexionsOO <id:id, id:id>
 *connexionsCO <id:id, id:id>
 *connexionsCC <id:id, id:id>
 *connexionsDO <id:id, id:id> output
 *
 *Déduction:
 *connexionsOOTarget <id:id, id:id> output
 *
 *Construction:
 *mapOldNewOperator<id, COperator*> output
 *mapOldNewData<id, CData*> output
 *
 *Processus:
 *1/ Lire le fichier *.cm pour produire les structures,
 *2/ Déduire l'opérateur connecté à un autre operateur, s'il y a des compositions entre les deux,
 *3/ Vérifier les règles de construction du *.cm
 *4/ Instancier les opérateurs et les datas dans le kernel,
 *5/ Lancer les actions sur les opérateurs,
 *6/ Connecter les opérateurs,
 *7/ Connecter opérateurs et datas,
 *8/ Lancer l'exécution du programme.
 *
 *Règles de construction du *.cm:
 *  * Pas de controlleur faisant office de "passeur" dans une boucle sinon ERROR.
 *
*/
    /*
            lcm -i /$DIR
            Créer une arbo Caméléon.

            lcm -i
            Décrit l'instance

            lcm -v
            kernel 2.2.0
            lcm 1.0.0

            lcm -l
            licence print

            lcm -h
            Donne la man page Caméléon (à écrire)

            lcm example.cm
            launch with default parameters setted on the editor.

            lcm example.cm -h
            Donne la man page du fichier (propriété, description).

            lcm example.cm -v
            Donne la version du fichier (propriété, version).

            lcm example.cm param1 ... paramN
            Permet de lancer le fichier avec des paramètres différents.

            #OPERATOR
            <operator id="1" type="sync" x="1035" y="-105.758" z="9" parentId="0"/>
            <operatorconnector opId="1" side="INPUT" connectorId="0" x="-49.2539" y="43.9324"/>
            <operatorconnector opId="1" side="INPUT" connectorId="1" x="-65.2423" y="9.97231"/>
            <operatorconnector opId="1" side="OUTPUT" connectorId="2" x="58.3959" y="30.7558"/>
            <operatorconnector opId="1" side="OUTPUT" connectorId="3" x="66" y="-0.0677037"/>

            => creator operator + mapOldNew <id, COperator>

            #DATA (plugFromOutside)
            <compositioncontrol id="76" type="EditorNumber" x="352" y="26" z="1" width="186" height="83" parentId="0" content="1000000"/>
            <controlconnector controlid="76" connectorid="0" side="OUT" name="testexp" datatype="DATASTRING" content="ttt"/>

            => create data + mapOldNew <id, CData*>

            #Connections
            #oper oper
            #control oper
            #composition oper
            #composition composition

            => create map connection oper/oper
            <id, oper*;id, oper*>

            => create map connection data/oper
            <id, CData*;id, oper*>

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

    QMap<int,QString> operatorMap;
    QMap<int,QString> actionOperator;
    QMap<QString,QString> dataMap;
    QMap<QString,QString> dataContent;

    QMap<QString,QString> connexionsOO;
    QMap<QString,QString> connexionsCO;
    QMap<QString,QString> connexionsOC;
    QMap<QString,QString> connexionsCC;
    QMap<QString,QString> connexionsDO;

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (xmlReader.name() == "operator"){
                /*
                 *operatorsMap <id, TYPE>
                 *actionOperator <id, action> output
                */
                int id = xmlReader.attributes().value("id").toString().toInt();
                QString type = xmlReader.attributes().value("type").toString();
                qDebug() << ("Operator");
                actionOperator.insert(id,type);

            }
            if (xmlReader.name() == "operatoraction"){
                        /*
                         *operatorsMap <id, TYPE>
                         *actionOperator <id, action> output
                        */
                        int id = xmlReader.attributes().value("opId").toString().toInt();
                        QString actionKey = xmlReader.attributes().value("actionKey").toString();
                        qDebug() << ("OperatorAction");
                        actionOperator.insert(id,actionKey);

            }
            if (xmlReader.name() == "controlconnector"){
                /*
                 *dataMap <id:id, TYPE>
                 *dataMapContent <id:id, CONTENT>
                            <controlconnector controlid="76" connectorid="0" side="OUT" name="testexp" datatype="DATASTRING" content="ttt"/>

                */
                QString ctrlid = xmlReader.attributes().value("controlid").toString();
                QString connectorid = xmlReader.attributes().value("connectorid").toString();
                QString type = xmlReader.attributes().value("datatype").toString();
                QString content = xmlReader.attributes().value("content").toString();
                qDebug() << ("controlconnector");
                dataMap.insert(ctrlid+":"+connectorid, type);
                dataMap.insert(ctrlid+":"+connectorid, content);

            }
            if (xmlReader.name() == "connection"){
                /*
                 *connexionsOO <id:id, id:id>
                 *connexionsCO <id:id, id:id>
                 *connexionsCC <id:id, id:id>
                 *connexionsDO <id:id, id:id> output
                */
                if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull() ){
                    QString connectorId1 = xmlReader.attributes().value("connectorId1").toString();
                    QString connectorId2 = xmlReader.attributes().value("connectorId2").toString();
                    QString operatorId1 = xmlReader.attributes().value("operatorId1").toString();
                    QString operatorId2 = xmlReader.attributes().value("operatorId2").toString();
                    qDebug() << ("connectionOO");
                    connexionsOO.insert(operatorId1+":"+connectorId1, operatorId2+":"+connectorId2);

                }
                if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull()){
                    QString connectorId1 = xmlReader.attributes().value("connectorId1").toString();
                    QString connectorId2 = xmlReader.attributes().value("connectorId2").toString();
                    QString compositionId1 = xmlReader.attributes().value("compositionId1").toString();
                    QString operatorId2 = xmlReader.attributes().value("operatorId2").toString();
                    qDebug() << ("connectionOC: "+operatorId2+":"+connectorId2+" "+compositionId1+":"+connectorId1);
                    connexionsOC.insert(operatorId2+":"+connectorId2, compositionId1+":"+connectorId1);

                }
                if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                    QString connectorId1 = xmlReader.attributes().value("connectorId1").toString();
                    QString connectorId2 = xmlReader.attributes().value("connectorId2").toString();
                    QString operatorId1 = xmlReader.attributes().value("operatorId1").toString();
                    QString compositionId2 = xmlReader.attributes().value("compositionId2").toString();
                    qDebug() << ("connectionOC: "+operatorId1+":"+connectorId1+" "+compositionId2+":"+connectorId2);
                    connexionsOC.insert(operatorId1+":"+connectorId1, compositionId2+":"+connectorId2);

                }
                if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                    QString connectorId1 = xmlReader.attributes().value("connectorId1").toString();
                    QString connectorId2 = xmlReader.attributes().value("connectorId2").toString();
                    QString compositionId1 = xmlReader.attributes().value("compositionId1").toString();
                    QString compositionId2 = xmlReader.attributes().value("compositionId2").toString();
                    if(compositionId2+":"+connectorId2!=connexionsCC.value(compositionId1+":"+connectorId1)){
                        connexionsCC.insert(compositionId2+":"+connectorId2, compositionId1+":"+connectorId1);
                        qDebug() << ("connectionCC: "+compositionId2+":"+connectorId2+" "+compositionId1+":"+connectorId1);
                    }
                }

                if(!xmlReader.attributes().value("controlConnectorIdIn").isNull() && !xmlReader.attributes().value("operatorConnectorIdOut").isNull()){
                    QString controlId = xmlReader.attributes().value("controlId").toString();
                    QString operatorIdOut = xmlReader.attributes().value("operatorIdOut").toString();
                    QString controlConnectorIdIn = xmlReader.attributes().value("controlConnectorIdIn").toString();
                    QString operatorConnectorIdOut = xmlReader.attributes().value("operatorConnectorIdOut").toString();
                    qDebug() << ("connectionDO");
                    connexionsDO.insert(controlId+":"+controlConnectorIdIn, operatorIdOut+":"+operatorConnectorIdOut);

                }

                qDebug() << ("connection");

            }
        }
    }

    qDebug() << "dataMap: " << dataMap;
    qDebug() << "connexionsDO: " << connexionsDO;
    qDebug() << "connexionsOO: " << connexionsOO;
    qDebug() << "connexionsOC: " << connexionsOC;
    qDebug() << "connexionsCC: " << connexionsCC;

    QMap<QString, QString>::const_iterator i = connexionsCO.constBegin();
    while (i != connexionsCO.constEnd()) {
        qDebug() << i.key() << ": " << i.value() << endl;
        ++i;
    }

    file.close();

    /*
     A déduire:
         *connexionsOOTarget <id:id, id:id> output
     */

    /*
     A construire:
         *mapOldNewOperator<id, COperator*> output
         *mapOldNewData<id, CData*> output
     */
}




