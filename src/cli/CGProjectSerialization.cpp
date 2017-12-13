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

#include <CGlossary.h>

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

QString CGProjectSerialization::searchFriend(QString operid,
                                             QString compid,
                                             QMap<QString,QString> connexionsOC,
                                             QMap<QString,QString> connexionsCC,
                                             QMap<QString,QString> connexionsCCreverse
                                             ){
    QString friendid = NULL;
    QMapIterator<QString, QString> i(connexionsOC);
    while (i.hasNext()) {
        i.next();
        if(i.value()==compid && i.key()!=operid) friendid=i.key();
    }

    if(friendid!=NULL) return friendid;
    QString cmid1 = connexionsCC.value(compid);

    if(cmid1!=NULL) return CGProjectSerialization::searchFriend(operid,
                                                                cmid1,
                                                                connexionsOC,
                                                                connexionsCC,
                                                                connexionsCCreverse);;
    QString cmid2 = connexionsCCreverse.value(compid);
    if(cmid2!=NULL) return CGProjectSerialization::searchFriend(operid,
                                                                cmid1,
                                                                connexionsOC,
                                                                connexionsCC,
                                                                connexionsCCreverse);;
    return NULL;

    /*
    *  COper* searchFriend(operid, compid, connexionsOC, connexionsCO, connexionsCC, connexionsCCreverse)
    *      if friendid = connexionsCO[compid]
    *          return friendid
    *      if cmid = connexionsCC[compid]
    *          return searchFriend(operid,cmid, connexionsCO, connexionsCC, connexionsCCreverse)
    *      if cmid = connexionsCCreverse[compid]
    *          return searchFriend(operid,cmid, connexionsCO, connexionsCC, connexionsCCreverse)
    */

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

    QMap<QString,QString> operatorMap;
    QMap<QString,QString> actionOperator;
    QMap<QString,QString> dataMap;
    QMap<QString,QString> dataContent;

    QMap<QString,QString> connexionsOO;
    QMap<QString,QString> connexionsOC;
    QMap<QString,QString> connexionsCC;
    QMap<QString,QString> connexionsCCreverse;
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
                QString id = xmlReader.attributes().value("id").toString();
                QString type = xmlReader.attributes().value("type").toString();
                qDebug() << ("Operator");
                operatorMap.insert(id,type);

            }
            if (xmlReader.name() == "operatoraction"){
                /*
                         *operatorsMap <id, TYPE>
                         *actionOperator <id, action> output
                        */
                QString id = xmlReader.attributes().value("opId").toString();
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
                QString side = xmlReader.attributes().value("side").toString();
                qDebug() << ("controlconnector");
                if(side == "OUT") dataMap.insert(ctrlid+":"+connectorid, type);
                if(side == "OUT") dataContent.insert(ctrlid+":"+connectorid, content);

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
                    if(connexionsOO.value(operatorId2+":"+connectorId2)==NULL) connexionsOO.insert(operatorId1+":"+connectorId1, operatorId2+":"+connectorId2);
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
                        connexionsCCreverse.insert(compositionId1+":"+connectorId1, compositionId2+":"+connectorId2);
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
    qDebug() << "dataContent: " << dataContent;
    qDebug() << "operatorMap: " << operatorMap;
    qDebug() << "connexionsDO: " << connexionsDO;
    qDebug() << "connexionsOO: " << connexionsOO;
    qDebug() << "connexionsOC: " << connexionsOC;
    qDebug() << "connexionsCC: " << connexionsCC;
    qDebug() << "connexionsCCreverse: " << connexionsCCreverse;

    file.close();
    /*
     A déduire:
         *connexionsOOTarget <id:id, id:id> output
         *
         *Base conceptuelle:
         *  https://www.shinoe.org/meta/
         *
         *Inputs:
         *  connexionsOC
         *  connexionsCC
         *  connexionsCCreverse
         *
         *Signautre:
         *  COper* searchFriend(Coper*, connexionsOC, connexionsCC)
         *
         *Algo (pseudo code c++pythonesque):
         *  foreach operid in connexionsOC:
         *      operFriendId = searchFriend(operid, connexionsOC[operid], connexionsOC, connexionsCC, connexionsCCreverse)
         *      connexionsOO.insert(oper, operFriendId)
         *
         *  COper* searchFriend(operid, compid, connexionsOC, connexionsCC, connexionsCCreverse)
         *      if friendid = connexionsOC[compid]
         *          return friendid
         *      if cmid = connexionsCC[compid]
         *          return searchFriend(operid,cmid, connexionsCC, connexionsCCreverse)
         *      if cmid = connexionsCCreverse[compid]
         *          return searchFriend(operid,cmid, connexionsCC, connexionsCCreverse)
         *
         */


    QMapIterator<QString, QString> i(connexionsOC);
    while (i.hasNext()) {
        i.next();
        QString friendid = CGProjectSerialization::searchFriend(i.key(),i.value(),connexionsOC,connexionsCC,connexionsCCreverse);
        if(friendid!=NULL && connexionsOO.value(friendid)==NULL) connexionsOO.insert(i.key(),friendid);
    }
    qDebug() << "connexionsOO++: " << connexionsOO;


    /*
     *A construire via l'api du kernel:
     *  mapOldNewOperator<id, COperator*> output
     *  mapOldNewData<id, CData*> output
     *
     *A partir de:
     *operatorMap
     *dataMap
     *dataContent
     *actionOperator
     */

    QMapIterator<QString, QString> j(operatorMap);
    while (j.hasNext()) {
        j.next();
        int opid;
        COperatorStructure opstruc;
        CClientSingleton::getInstance()->getCInterfaceClient2Server()->createOperatorSend(j.value().toStdString(),opid,opstruc);
        qDebug() << "Operator created: " << opstruc.getName().c_str();
    }

    QMapIterator<QString, QString> n(actionOperator);
    while (n.hasNext()) {
        n.next();
        //CClientSingleton::getInstance()->getCInterfaceClient2Server()->actionOperator(ope->getId(), actionKey.toStdString());
    }

    QMapIterator<QString, QString> k(dataMap);
    while (k.hasNext()) {
        k.next();
        CData* data = CGlossarySingletonClient::getInstance()->createData(k.value().toStdString());
        data->fromString(dataContent.value(k.key()).toStdString());
        qDebug() << "Data created: " << data->getKey().c_str();
    }

    /*
     *A connecter via l'api du kernel:
     *  connexionsOO++
     *  connexionsDO filtrés par dataMap
     *
     */
    QMapIterator<QString, QString> l(connexionsOO);
    while (l.hasNext()) {
        l.next();
        /*
            int idop1   = this->getCGOperatorModel()->getId();
            int idplug1 = this->getId();

            int idop2   = connectedConnector->getCGOperatorModel()->getId();
            int idplug2 = connectedConnector->getId();
            int nPlugIn = this->getCGOperatorModel()->getNumPlugIn();
            CClientSingleton::getInstance()->getCInterfaceClient2Server()->connectPlugOut2PlugInSend(idop1,idplug1-nPlugIn,idop2,idplug2);
        */
    }


    QMapIterator<QString, QString> m(connexionsDO);
    while (m.hasNext()) {
        m.next();
        //CClientSingleton::getInstance()->getCInterfaceClient2Server()->setPlugInSend(opid,plugkeyin,data,CPlug::NEW);
    }

}


