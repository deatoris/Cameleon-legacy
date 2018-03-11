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

void CGProjectSerialization::load(){
    emit progressSerial(0);
    //    ////qDebug << "try load project from " << CGProject::getInstance()->getPath();
    //    CGLayout* layout0 = CGLayoutManager::getInstance()->getLayout();
    //    layout0->setName("pattern");

    //    layout0 = CGLayoutManager::getInstance()->getLayout();
    //    layout0->setName("configuration");
    CGProject::getInstance()->setName("");
    CGProject::getInstance()->setVersion("");
    QFile file(CGProject::getInstance()->getPath());
    //    QFileInfo fileInfos(file);

    //    ////qDebug << "";
    //    ////qDebug << "==========================";
    //    ////qDebug << "read " << fileInfos.absoluteFilePath();
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QMap<int,int> mapOldNewControl;
    QMap<int,int> mapOldNewOperator;
    QMap<int,int> mapOldNewLayout;
    QMap<int,CGCompositionModel*> mapOldNewComposition;
    CGCompositionManager::getInstance()->reinit();
    CGProject::getInstance()->clearProcesses();
    mapOldNewComposition.insert(0,CGCompositionManager::getInstance()->getCurrentCompositionModel());
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    int currentLayout = -1;
    int currentComposition = 0;
    int nb_operators =0;
    int nb_controls = 0;
    int nb_compositions = 0;
    //CLogger::getInstance()->clear();
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }else{
                //                ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "readme"){
                        QString readme = xmlReader.attributes().value("text").toString();
                        CGProject::getInstance()->setReadMe(readme);
                    }
                    if (xmlReader.name() == "structure"){
                        QString sp = xmlReader.attributes().value("showproject").toString();
                        QString sg = xmlReader.attributes().value("showglossary").toString();
                        QString c = xmlReader.attributes().value("showcontroller").toString();
                        QString m = xmlReader.attributes().value("showmachine").toString();
                        if(sp.compare("true")==0){
                            CGCompositionManager::getInstance()->getComposer()->showProject();
                        }
                        if(sg.compare("true")!=0){
                            CGCompositionManager::getInstance()->getComposer()->showFactory();
                        }
                        if(c.compare("true")==0){
                            CGProject::getInstance()->setShowController(true);
                        }else{
                            CGProject::getInstance()->setShowController(false);
                        }
                        if(m.compare("true")==0){
                            CGCompositionManager::getInstance()->getComposer()->getMachine()->show();
                        }else{
                            CGCompositionManager::getInstance()->getComposer()->getMachine()->hide();
                        }
                    }
                    if (xmlReader.name() == "log"){
                        QString logPath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGProject::getInstance()->setLogPath(logPath);
                        CLogger::getInstance()->setPath(logPath);
                        QString b = xmlReader.attributes().value("active").toString();
                        if(b.compare("true")==0){
                            CLogger::getInstance()->setActive(true);
                        }else{
                            CLogger::getInstance()->setActive(false);
                        }
                    }
                    if (xmlReader.name() == "rule"){
                        QString componentName = xmlReader.attributes().value("component").toString();
                        QString level = xmlReader.attributes().value("level").toString();
                        CLogger::getInstance()->registerComponent(componentName);
                        CLogger::getInstance()->setComponentLevel(componentName, CLogger::getInstance()->fromString(level));
                    }
                    if (xmlReader.name() == "controllerscreen"){
                        int x = xmlReader.attributes().value("windowsx").toString().toInt();
                        int y = xmlReader.attributes().value("windowsy").toString().toInt();
                        int w = xmlReader.attributes().value("windowswidth").toString().toInt();
                        int h = xmlReader.attributes().value("windowsheight").toString().toInt();
                        int num = xmlReader.attributes().value("numscreen").toString().toInt();
                        QString full = xmlReader.attributes().value("fullscreen").toString();

                        CGLayoutManager::getInstance()->getController()->setGeometry(x,y,w,h);
                        if(num != 0){
                            QWidget* widget = QApplication::desktop()->screen(num);
                            CGLayoutManager::getInstance()->getController()->setParent(widget);
                        }
                        if(full.compare("true") == 0){
                            //                            CGLayoutManager::getInstance()->getController()->showMaximized();
                        }
                    }
                    if (xmlReader.name() == "composerscreen"){
                        int x = xmlReader.attributes().value("windowsx").toString().toInt();
                        int y = xmlReader.attributes().value("windowsy").toString().toInt();
                        int w = xmlReader.attributes().value("windowswidth").toString().toInt();
                        int h = xmlReader.attributes().value("windowsheight").toString().toInt();
                        int num = xmlReader.attributes().value("numscreen").toString().toInt();
                        QString full = xmlReader.attributes().value("fullscreen").toString();

                        CGCompositionManager::getInstance()->getComposer()->setGeometry(x,y,w,h);
                        if(num != 0){
                            //                            QWidget* widget = QApplication::desktop()->screen(num);
                            //                            CGCompositionManager::getInstance()->getComposer()->setParent(widget);
                        }
                        if(full.compare("true") == 0){
                            //                            CGCompositionManager::getInstance()->getComposer()->showMaximized();
                        }
                    }

                    if (xmlReader.name() == "threads"){
                        int max = xmlReader.attributes().value("max").toString().toInt();
                        CGProject::getInstance()->setNumberThread(max);

                    }
                    if (xmlReader.name() == "currentLayout"){
                        currentLayout = xmlReader.attributes().value("id").toString().toInt();
                    }
                    if (xmlReader.name() == "currentComposition"){
                        currentComposition = xmlReader.attributes().value("id").toString().toInt();
                    }
                    if (xmlReader.name() == "version"){
                        QString version = xmlReader.attributes().value("version").toString();
                        CGProject::getInstance()->setVersion(version);
                    }
                    if (xmlReader.name() == "name"){
                        QString name = xmlReader.attributes().value("name").toString();
                        CGProject::getInstance()->setName(name);
                    }
                    if (xmlReader.name() == "target"){
                        QString targetPath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGProject::getInstance()->setTargetPath(targetPath);
                    }
                    if(xmlReader.name() == "connectorOrder"){
                        QString m = xmlReader.attributes().value("mode").toString();
                        CGProject::getInstance()->setConnectorOrderMode("HARD");
                    }
                    if (xmlReader.name() == "tmp"){
                        QString tmpPath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGProject::getInstance()->setTmpPath(tmpPath);
                    }

                    if (xmlReader.name() == "ressource"){
                        CLogger::getInstance()->log("SERIALIZE",CLogger::INFO,"Ressources not supported by this version!");

                        //QString ressourcePath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        //CGProject::getInstance()->setRessourcePath(ressourcePath);
                    }
                    if (xmlReader.name() == "layout"){
                        QString name = xmlReader.attributes().value("name").toString();
                        QString type = xmlReader.attributes().value("type").toString();
                        QString resolution = xmlReader.attributes().value("resolution").toString();
                        QString popup = xmlReader.attributes().value("popup").toString();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        ////qDebug << "rebuild layout name: " <<name;
                        CGLayout* layout;
                        if(id == 0){
                            layout = CGLayoutManager::getInstance()->getLayout(id);
                            layout->setType("interface");
                        }else{
                            layout = CGLayoutManager::getInstance()->getLayout();
                            layout->setName(name);
                            layout->setType(type);
                            CGProject::getInstance()->addLayout(layout);
                        }
                        layout->setResolution(resolution);

                        if(popup.compare("true")==0)layout->setIsPopup(true);
                        if(popup.compare("false")==0)layout->setIsPopup(false);

                        //check id
                        if(layout->getId() == id){
                            CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layout id!");
                        }else{
                            CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layout id!"+QString::number(id)+" "+QString::number(layout->getId()));
                        }
                        mapOldNewLayout.insert(id, layout->getId());
                    }
                    if (xmlReader.name() == "composition"){
                        if(!xmlReader.attributes().value("parentId").isNull()){
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            ////qDebug << "rebuild composition name: " <<name;
                            CGCompositionModel* compositionModel = new CGCompositionModel();
                            QString informations = xmlReader.attributes().value("informations").toString();
                            compositionModel->setInformation(informations);
                            //                            int idNew;
                            //                            CClientSingleton::getInstance()->getCInterfaceClient2Server()->createUnitProcessSend(idNew);
                            //                            compositionModel->setId(idNew);
                            mapOldNewComposition.insert(id,compositionModel);
                        }else{
                            //nothing to be done
                        }
                    }
                    if (xmlReader.name() == "numbers"){
                        nb_operators = xmlReader.attributes().value("nb_operators").toString().toInt();
                        nb_controls = xmlReader.attributes().value("nb_controls").toString().toInt();
                        nb_compositions = xmlReader.attributes().value("nb_compositions").toString().toInt();
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(10);
    int countCompo = 0;
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
                        if(!xmlReader.attributes().value("parentId").isNull()){
                            int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            int x = xmlReader.attributes().value("x").toString().toDouble();
                            int y = xmlReader.attributes().value("y").toString().toDouble();
                            int z = xmlReader.attributes().value("z").toString().toDouble();
                            //                            int rayon = xmlReader.attributes().value("rayon").toString().toDouble();
                            ////qDebug << "rebuild composition name: " <<name;

                            CGCompositionModel* par = mapOldNewComposition.value(parentId);
                            CGCompositionModel* child = mapOldNewComposition.value(id);
                            //                            CClientSingleton::getInstance()->getCInterfaceClient2Server()->linkUnitProcess(child->getId(), par->getId());
                            QString informations = xmlReader.attributes().value("informations").toString();
                            child->setInformation(informations);

                            child->setParentCompositionModel(par);
                            child->initialise();
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(child->getOuterView());

                            CGMulti* multi = child->getOuterView();
                            child->setName(name);
                            multi->setPos(x,y);
                            multi->setZValue(z);
                            multi->setIsCompatibleMode(false);
                            multi->setSelected(false);
                            multi->setSelected(false);

                            if(child->getId() == id){
                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good composition id!");
                            }else{
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad composition id! "+QString::number(id)+" "+QString::number(child->getId()));
                            }
                            CGProject::getInstance()->addComposition(child);
                        }else{
                        }
                        //progress bar update
                        countCompo++;
                        int i = 0;
                        if(nb_compositions!=0) i = 10*countCompo/nb_compositions;
                        emit progressSerial(10+i);
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(20);
    int countOp=0;
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
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                        CGCompositionModel* par = mapOldNewComposition.value(parentId);
                        if(par!=NULL){
                            CGOperatorModel* model = par->addOperatorByName(name);
                            if(model!=NULL){
                                CGOperator* view = model->getView();
                                model->getView()->setPos(x,y);

                                model->setPos(model->getView()->scenePos());
                                par->getScene()->unselectAll();
                                par->getScene()->getModel()->activate(view);
                                model->getView()->setZValue(z);
                                ////qDebug << "[check]rebuild operator type: " << name << "" << model->getId();
                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load operator named "+name);
                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load operator with position "+QString::number(x)+","+QString::number(y));
                                //                                if(model->getId() == id){
                                //                                    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good operator id!");
                                //                                }else{
                                //                                    CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad operator id!");
                                //                                }
                                mapOldNewOperator.insert(id,model->getId());
                            }else{
                                mapOldNewOperator.insert(id,-1);
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create operator "+name);
                            }
                        }
                        //progress bar update
                        countOp++;
                        int i = 0;
                        if(nb_operators!=0) i = 30*countOp/nb_operators;
                        emit progressSerial(20+i);
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(50);
    int countCt = 0;
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


                    if (xmlReader.name() == "control"){
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        QString content = xmlReader.attributes().value("content").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        int width = xmlReader.attributes().value("width").toString().toDouble();
                        int height = xmlReader.attributes().value("height").toString().toDouble();
                        QString ispers = xmlReader.attributes().value("persistent").toString();

                        ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                        QRect rect(0,0,width,height);
                        parentId = mapOldNewLayout.value(parentId);
                        CGLayout* par = CGLayoutManager::getInstance()->getLayout(parentId);
                        CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name,par);
                        if(layoutItem!=NULL && layoutItem->getControl()!=NULL){
                            layoutItem->setSelected(false);
                            layoutItem->setPos(x,y);
                            if(ispers.compare("true") == 0){
                                layoutItem->setIsPersistant(true);
                            }else{
                                layoutItem->setIsPersistant(false);
                            }

                            //                        layoutItem->resize(rect.bottomRight());
                            //                        layoutItem->setRect(rect);
                            par->activate(layoutItem);
                            layoutItem->setZValue(z);
                            layoutItem->resize(rect);
                            ////qDebug << "[check]rebuild layoutItem type: " << name << "" << layoutItem->getControl()->getId();
                            //                            if(layoutItem->getControl()->getId() == id){
                            //                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layoutItem id!");
                            //                            }else{
                            //                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layoutItem id!");
                            //                            }

                            layoutItem->getControl()->fromString(content.toStdString());
                            mapOldNewControl.insert(id,layoutItem->getControl()->getId());
                        }else{
                            CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create control "+name);
                            mapOldNewControl.insert(id,-1);
                        }
                        countCt++;
                        int i = 0;
                        if(nb_controls!=0) i = 20*countCt/nb_controls;
                        emit progressSerial(50+i);
                    }
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(70);

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


                    if (xmlReader.name() == "compositioncontrol"){
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        if(name.compare("RessourceProject") == 0){
                            CLogger::getInstance()->log("SERIALIZE",CLogger::INFO,"Ressources not supported by this version!");
                        }else{
                            QString content = xmlReader.attributes().value("content").toString();
                            int x = xmlReader.attributes().value("x").toString().toDouble();
                            int y = xmlReader.attributes().value("y").toString().toDouble();
                            int z = xmlReader.attributes().value("z").toString().toDouble();
                            int width = xmlReader.attributes().value("width").toString().toDouble();
                            int height = xmlReader.attributes().value("height").toString().toDouble();
                            QString ispers = xmlReader.attributes().value("persistent").toString();

                            ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                            QRect rect(0,0,width,height);
                            CGCompositionModel* compM = mapOldNewComposition.value(parentId);
                            CGComposition* comp = compM->getScene();
                            CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name,comp);
                            if(layoutItem!=NULL && layoutItem->getControl()!=NULL){
                                layoutItem->setSelected(false);
                                layoutItem->setPos(x,y);
                                if(ispers.compare("true") == 0){
                                    layoutItem->setIsPersistant(true);
                                }else{
                                    layoutItem->setIsPersistant(false);
                                }

                                //                        layoutItem->resize(rect.bottomRight());
                                //                        layoutItem->setRect(rect);
                                layoutItem->setZValue(z);
                                compM->activate(layoutItem);
                                layoutItem->resize(rect);
                                ////qDebug << "[check]rebuild layoutItem type: " << name << "" << layoutItem->getControl()->getId();
                                //                            if(layoutItem->getControl()->getId() == id){
                                //                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layoutItem id!");
                                //                            }else{
                                //                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layoutItem id!");
                                //                            }

                                layoutItem->getControl()->fromString(content.toStdString());
                                mapOldNewControl.insert(id,layoutItem->getControl()->getId());
                                layoutItem->getControl()->apply();
                            }else{
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create control "+name);
                                mapOldNewControl.insert(id,-1);
                            }
                        }
                    }
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(70);

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

                    if (xmlReader.name() == "shape"){
                        ////qDebug << "rebuild shape" <<CGProject::getInstance()->getName();
                        int parentid = xmlReader.attributes().value("parentId").toString().toInt();
                        QString color = xmlReader.attributes().value("color").toString();
                        int xrect = xmlReader.attributes().value("xrect").toString().toDouble();
                        int yrect = xmlReader.attributes().value("yrect").toString().toDouble();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        int width = xmlReader.attributes().value("width").toString().toDouble();
                        int height = xmlReader.attributes().value("height").toString().toDouble();
                        QString ispers = xmlReader.attributes().value("persistent").toString();
                        QRect rect(xrect,yrect,width,height);
                        if(!xmlReader.attributes().value("fromLayout").isNull()){
                            parentid = mapOldNewLayout.value(parentid);
                            CGLayout* lay = CGLayoutManager::getInstance()->getLayout(parentid);

                            CGShape* shape = new CGShape(0,0,lay);
                            //                            lay->addItem(shape);
                            QColor c;
                            c.setNamedColor(color);
                            shape->setColor(color);
                            shape->setRect(rect);
                            shape->setPos(x,y);
                            shape->setZValue(z);
                            if(ispers.compare("true") == 0){
                                shape->setPersistent(true);
                            }else{
                                shape->setPersistent(false);
                            }
                        }else{
                            CGCompositionModel* par = mapOldNewComposition.value(parentid);
                            //                            CGCompositionModel* par = CGCompositionManager::getInstance()->getCompositionById(parentid);
                            CGShape* shape = new CGShape(0,0,par->getScene());
                            //                            par->getScene()->addItem(shape);
                            shape->setRect(rect);
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(shape);
                            QColor c;
                            c.setNamedColor(color);
                            shape->setPos(x,y);
                            shape->setColor(color);
                            shape->setZValue(z);
                        }
                    }

                    if (xmlReader.name() == "text"){
                        ////qDebug << "rebuild text";
                        int parentid = xmlReader.attributes().value("parentId").toString().toInt();
                        QString content = xmlReader.attributes().value("content").toString();
                        QString color = xmlReader.attributes().value("color").toString();
                        int police = xmlReader.attributes().value("police").toString().toInt();
                        QString font = xmlReader.attributes().value("font").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        QString italic = xmlReader.attributes().value("italic").toString();
                        QString bold = xmlReader.attributes().value("bold").toString();
                        QString underline = xmlReader.attributes().value("underline").toString();

                        if(!xmlReader.attributes().value("fromLayout").isNull()){
                            CGText* text = new CGText();
                            parentid = mapOldNewLayout.value(parentid);
                            CGLayout* lay = CGLayoutManager::getInstance()->getLayout(parentid);
                            //                            text->setFont(layout->font());
                            text->setTextInteractionFlags(Qt::TextEditorInteraction);
                            text->setZValue(1000.0);

                            QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                             lay, SLOT(editorLostFocus(CGText*)));
                            QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                             lay, SIGNAL(itemSelected(QGraphicsItem*)));
                            lay->addItem(text);
                            text->setPos(x,y);
                            text->setZValue(z);
                            text->setPlainText(content);

                            QColor c;
                            c.setNamedColor(color);
                            text->setDefaultTextColor(c);
                            QFont f;
                            f.setFamily(font);
                            f.setPointSize(police);

                            if(italic.compare("true") == 0){
                                f.setItalic(true);
                            }
                            if(bold.compare("true") == 0){
                                f.setBold(true);
                            }
                            if(underline.compare("true") == 0){
                                f.setUnderline(true);
                            }
                            text->setFont(f);

                            QString ispers = xmlReader.attributes().value("persistent").toString();
                            if(ispers.compare("true") == 0){
                                text->setPersistent(true);
                            }else{
                                text->setPersistent(false);
                            }
                        }else{
                            CGText* text = new CGText();
                            CGCompositionModel* par = mapOldNewComposition.value(parentid);
                            //                            CGCompositionModel* par = CGCompositionManager::getInstance()->getCompositionById(parentid);
                            text->setFont(par->getScene()->font());
                            text->setTextInteractionFlags(Qt::TextEditorInteraction);
                            text->setZValue(10000.0);

                            QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                             par->getScene(), SLOT(editorLostFocus(CGText*)));
                            QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                             par->getScene(), SIGNAL(itemSelected(QGraphicsItem*)));
                            par->getScene()->addItem(text);

                            text->setDefaultTextColor(Qt::white);
                            text->setPos(x,y);
                            text->setZValue(z);
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(text);
                            text->setPlainText(content);

                            QColor c;
                            c.setNamedColor(color);
                            text->setDefaultTextColor(c);

                            QFont f;
                            f.setFamily(font);
                            f.setPointSize(police);

                            if(italic.compare("true") == 0){
                                f.setItalic(true);
                            }
                            if(bold.compare("true") == 0){
                                f.setBold(true);
                            }
                            if(underline.compare("true") == 0){
                                f.setUnderline(true);
                            }
                            text->setFont(f);
                        }
                    }

                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);


    emit progressSerial(80);
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
                    if (xmlReader.name() == "operatoraction"){
                        int id = xmlReader.attributes().value("opId").toString().toInt();
                        QString actionKey = xmlReader.attributes().value("actionKey").toString();
                        int newId = mapOldNewOperator.value(id);
                        CGOperatorModel* ope = CGCompositionManager::getInstance()->searchOperatorById(newId);
                        if(ope!=0){
                            ope->getView()->saveAction(actionKey);
                            ope->setWaiting(true);
                            CClientSingleton::getInstance()->getCInterfaceClient2Server()->actionOperator(ope->getId(), actionKey.toStdString());
                        }
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);

    //patch #105
    //lire le connector out:
    //<controlconnector name="XXX" datatype="XXX" content="XXXXXX"\/>

    emit progressSerial(80);
    QList<CGMultiConnector*> multiConnectorList;
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
                        int compositionId = xmlReader.attributes().value("compositionId").toString().toInt();
                        QString side = xmlReader.attributes().value("side").toString();
                        if(xmlReader.attributes().value("x").isNull()){
                            CGCompositionModel* model = mapOldNewComposition.value(compositionId);
                            QString name = xmlReader.attributes().value("name").toString();
                            if(side.compare("IN") == 0){
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("OUTIN");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                            }
                            if(side.compare("OUT") == 0) {
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("INOUT");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                            }
                        }else{
                            double x = xmlReader.attributes().value("x").toString().toDouble();
                            double y = xmlReader.attributes().value("y").toString().toDouble();

                            CGCompositionModel* model = mapOldNewComposition.value(compositionId);
                            QString name = xmlReader.attributes().value("name").toString();
                            if(side.compare("IN") == 0){
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("OUTIN");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                                QPointF p(x,y);
                                multiConnectorList.append(m);
                                m->getOuterConnector()->setPos(p);

                            }
                            if(side.compare("OUT") == 0) {
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("INOUT");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();

                                QPointF p(x,y);
                                multiConnectorList.append(m);
                                m->getOuterConnector()->setPos(p);
                            }
                        }
                    }
                }
            }
        }
    }

    foreach(CGMultiConnector*m,multiConnectorList){
        double r = (m->getOuterConnector()->getRayon())/2;
        double teta = acos(m->getOuterConnector()->pos().x()/r)*(SCD::sgn(m->getOuterConnector()->pos().y()));
        m->getOuterConnector()->rePosition();

        double x1 = cos(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
        double y1 = sin(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
        m->getInnerConnector()->setPos(x1,y1);
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(90);

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
                    if (xmlReader.name() == "connection"){
                        if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull() ){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int operatorId1 = xmlReader.attributes().value("operatorId1").toString().toInt();
                            int operatorId2 = xmlReader.attributes().value("operatorId2").toString().toInt();
                            if(mapOldNewOperator.value(operatorId1)!=-1 && mapOldNewOperator.value(operatorId2)!=-1){
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId1));
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId2));
                                if(ope1->getView()->getConnectorById(connectorId1)!=0 && ope2->getView()->getConnectorById(connectorId2)!=0) ope1->getView()->getConnectorById(connectorId1)->connect(ope2->getView()->getConnectorById(connectorId2));
                            }
                        }
                        if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull()){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int compositionId1 = xmlReader.attributes().value("compositionId1").toString().toInt();
                            int operatorId2 = xmlReader.attributes().value("operatorId2").toString().toInt();
                            if(mapOldNewOperator.value(operatorId2)!=-1){
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId2));
                                CGCompositionModel* model = mapOldNewComposition.value(compositionId1);
                                CGConnector* c2 = ope2->getView()->getConnectorById(connectorId2);
                                CGConnector* cm;
                                if(ope2->getView()->scene() == model->getOuterView()->scene()){
                                    cm = model->getOuterView()->getConnectorById(connectorId1);
                                }else{
                                    cm = model->getInnerView()->getConnectorById(connectorId1);
                                }
                                if(c2 != 0 && cm != 0) c2->connect(cm);
                            }
                        }
                        if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int operatorId1 = xmlReader.attributes().value("operatorId1").toString().toInt();
                            int compositionId2 = xmlReader.attributes().value("compositionId2").toString().toInt();
                            if(mapOldNewOperator.value(operatorId1)!=-1){
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId1));
                                CGCompositionModel* model = mapOldNewComposition.value(compositionId2);

                                CGConnector* c2 = ope2->getView()->getConnectorById(connectorId1);
                                CGConnector* cm;
                                if(ope2->getView()->scene() == model->getOuterView()->scene()){
                                    cm = model->getOuterView()->getConnectorById(connectorId2);
                                }else{
                                    cm = model->getInnerView()->getConnectorById(connectorId2);
                                }
                                if(c2 != 0 && cm != 0) c2->connect(cm);
                            }
                        }
                        if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int compositionId1 = xmlReader.attributes().value("compositionId1").toString().toInt();
                            int compositionId2 = xmlReader.attributes().value("compositionId2").toString().toInt();
                            CGCompositionModel* model1 = mapOldNewComposition.value(compositionId1);
                            CGCompositionModel* model2 = mapOldNewComposition.value(compositionId2);
                            if(model1->getParentCompositionModel() == model2){
                                model1->getOuterView()->getConnectorById(connectorId1)->connect(model2->getInnerView()->getConnectorById(connectorId2));
                            }
                            if(model1 == model2->getParentCompositionModel()){
                                model1->getInnerView()->getConnectorById(connectorId1)->connect(model2->getOuterView()->getConnectorById(connectorId2));
                            }
                            if(model1->getParentCompositionModel() == model2->getParentCompositionModel()){
                                model1->getOuterView()->getConnectorById(connectorId1)->connect(model2->getOuterView()->getConnectorById(connectorId2));
                            }
                        }
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(90);

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
                    if (xmlReader.name() == "connection"){
                        if(!xmlReader.attributes().value("controlConnectorIdIn").isNull() && !xmlReader.attributes().value("operatorConnectorIdOut").isNull()){
                            int controlId = xmlReader.attributes().value("controlId").toString().toInt();
                            int operatorIdOut = xmlReader.attributes().value("operatorIdOut").toString().toInt();
                            int controlConnectorIdIn = xmlReader.attributes().value("controlConnectorIdIn").toString().toInt();
                            int operatorConnectorIdOut = xmlReader.attributes().value("operatorConnectorIdOut").toString().toInt();
                            if(mapOldNewOperator.value(operatorIdOut)!=-1 && mapOldNewControl.value(controlId) != -1){
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorIdOut));
                                CGLayoutItem* layoutItem = CGLayoutManager::getInstance()->getItemById(mapOldNewControl.value(controlId));
                                if(ope1 != NULL && layoutItem != NULL){
                                    if(ope1->getView()->getConnectorById(operatorConnectorIdOut)!=0
                                            &&layoutItem->getConnectorById(controlConnectorIdIn)!=0){
                                        //TODO: this control doesn't have to be there. Miss design here !
                                        if(ope1->getView()->getConnectorById(operatorConnectorIdOut)->getSide().compare(layoutItem->getConnectorById(controlConnectorIdIn)->getSide()) != 0){
                                            layoutItem->getConnectorById(controlConnectorIdIn)->setIsControlConnected(true);
                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->setIsControlConnected(true);
                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->connect(layoutItem->getConnectorById(controlConnectorIdIn));
                                            layoutItem->getConnectorById(controlConnectorIdIn)->connect(ope1->getView()->getConnectorById(operatorConnectorIdOut));

                                            layoutItem->getConnectorById(controlConnectorIdIn)->update();
                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->update();
                                        }
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }
    }

    if(currentLayout!=-1){
        CGLayoutManager::getInstance()->showLayout(currentLayout);
        CGProject::getInstance()->switchToLayout(CGLayoutManager::getInstance()->getLayout(mapOldNewLayout.value(currentLayout)));
        CGLayoutManager::getInstance()->getCurrentLayout()->update();
    }
    CGCompositionManager::getInstance()->showSceneFromModel(mapOldNewComposition.value(currentComposition));
    CGProject::getInstance()->switchToModel(mapOldNewComposition.value(currentComposition));
    CGCompositionManager::getInstance()->getCurrentComposerScene()->update();

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(100);
    CGCompositionManager::getInstance()->getComposer()->show();
    //40880049: ControllerView save/load position
    if(CGInstance::getInstance()->getMode() != CGInstance::CMD)CGLayoutManager::getInstance()->getController()->show();
    int currentC = CGCompositionManager::getInstance()->getCurrentCompositionModel()->getId();
    int currentL = -1;
    if(currentLayout!=-1) currentL = CGLayoutManager::getInstance()->getCurrentLayout()->getId();

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
//                    if (xmlReader.name() == "controllerscreen"){
//                        int viewx = xmlReader.attributes().value("viewx").toString().toInt();
//                        int viewy = xmlReader.attributes().value("viewy").toString().toInt();
//                        int scale = xmlReader.attributes().value("viewscale").toString().toInt();
//                        if(CGLayoutManager::getInstance()->getCurrentLayout() != 0) CGLayoutManager::getInstance()->getCurrentLayout()->getView()->renderScale(scale);
//                        if(CGLayoutManager::getInstance()->getCurrentLayout() != 0) CGLayoutManager::getInstance()->getCurrentLayout()->getView()->horizontalScrollBar()->setValue(viewx);
//                        if(CGLayoutManager::getInstance()->getCurrentLayout() != 0) CGLayoutManager::getInstance()->getCurrentLayout()->getView()->verticalScrollBar()->setValue(viewy);
//                    }
                    if (xmlReader.name() == "composerscreen"){
                        int viewx = xmlReader.attributes().value("viewx").toString().toInt();
                        int viewy = xmlReader.attributes().value("viewy").toString().toInt();
                        int scale = xmlReader.attributes().value("viewscale").toString().toInt();
                        CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->renderScale(scale);
                        CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->horizontalScrollBar()->setValue(viewx);
                        CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->verticalScrollBar()->setValue(viewy);
                        if(CGCompositionManager::getInstance()->getCurrentComposerScene()->getModel()->getInnerView()!=0) CGCompositionManager::getInstance()->getCurrentComposerScene()->getModel()->getInnerView()->processInnerRay(true);
                    }
                    if (xmlReader.name() == "composition"){
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        if(mapOldNewComposition.value(id)->getId() != currentC){
                            CGCompositionModel* child = mapOldNewComposition.value(id);

                            int viewx = xmlReader.attributes().value("viewx").toString().toInt();
                            int viewy = xmlReader.attributes().value("viewy").toString().toInt();
                            int scale = xmlReader.attributes().value("viewscale").toString().toInt();
                            CGCompositionManager::getInstance()->showSceneFromModel(child);
                            child->getScene()->getView()->renderScale(scale);
                            child->getScene()->getView()->horizontalScrollBar()->setValue(viewx);
                            child->getScene()->getView()->verticalScrollBar()->setValue(viewy);
                            if(child->getScene()->getModel()->getInnerView()!=0) child->getScene()->getModel()->getInnerView()->processInnerRay(true);
                        }
                    }
                    if (xmlReader.name() == "layout"){
//                        qDebug()<<"FIRST ======================";
                        int id = xmlReader.attributes().value("id").toString().toInt();
//                         qDebug() <<id;
//                         qDebug()<< mapOldNewLayout.value(id);
//                         qDebug()<<"FIRST ======================";
//                        if(mapOldNewLayout.value(id) != currentL){
                            CGLayout* layout = CGLayoutManager::getInstance()->getLayout(mapOldNewLayout.value(id));
                            int viewx = xmlReader.attributes().value("viewx").toString().toInt();
                            int viewy = xmlReader.attributes().value("viewy").toString().toInt();
                            int scale = xmlReader.attributes().value("viewscale").toString().toInt();
                            CGLayoutManager::getInstance()->showLayout(mapOldNewLayout.value(id));
                            layout->getView()->renderScale(scale);
//                             qDebug()<<"FIRST ======================";
//                             qDebug()<<viewx;
//                             qDebug()<<viewy;
//                             qDebug()<<"FIRST ======================";
                            layout->getView()->horizontalScrollBar()->setValue(viewx);
                            layout->getView()->verticalScrollBar()->setValue(viewy);
//                             qDebug()<<"FIRST ======================";
//                             qDebug()<<layout->getView()->verticalScrollBar()->value();
//                             qDebug()<<layout->getView()->horizontalScrollBar()->value();
//                             qDebug()<<"FIRST ======================";
//                        }
                    }
                }
            }
        }
    }
    //40880049: ControllerView save/load position
    CGLayoutManager::getInstance()->getController()->hide();

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
                        int id = xmlReader.attributes().value("connectorId").toString().toInt();
                        int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                        double x = xmlReader.attributes().value("x").toString().toDouble();
                        double y = xmlReader.attributes().value("y").toString().toDouble();
                        CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                        if(ope1!=0){
                            CGConnector* c = ope1->getView()->getConnectorById(id);
                            QPointF p(x,y);
                            if(c!=0)c->setPos(p);
                        }
                    }
                }
            }
        }
    }










    CGProject::getInstance()->loadRessources();

    CGCompositionModel* child = CGCompositionManager::getInstance()->getCompositionById(currentC);
//    qDebug()<<"SECOND ======================";
    if(currentL!=-1) CGLayoutManager::getInstance()->showLayout(currentL);
//    qDebug()<<CGLayoutManager::getInstance()->getCurrentLayout()->getView()->verticalScrollBar()->value();
//    qDebug()<<CGLayoutManager::getInstance()->getCurrentLayout()->getView()->horizontalScrollBar()->value();
    CGCompositionManager::getInstance()->showSceneFromModel(child);
//    qDebug()<<"SECOND ======================";

    //start project logger
    CLogger::getInstance()->start();




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
                    if (xmlReader.name() == "compositioncontrol"){
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        CGLayoutItem* layoutItem = CGLayoutManager::getInstance()->getItemById(mapOldNewControl.value(id));
                        layoutItem->getControl()->apply();
                    }

                    if (xmlReader.name() == "controlconnector"){
                        int id = xmlReader.attributes().value("controlid").toString().toInt();
                        int connectorid = xmlReader.attributes().value("connectorid").toString().toInt();
                        QString name = xmlReader.attributes().value("name").toString();
                        CGLayoutItem* layoutItem = CGLayoutManager::getInstance()->getItemById(mapOldNewControl.value(id));

                        layoutItem->getConnectorById(connectorid)->getModel()->setExposedName(name);

                    }
                }
            }
        }
    }
}


void CGProjectSerialization::loadWithoutGui(){
    emit progressSerial(0);
    //    ////qDebug << "try load project from " << CGProject::getInstance()->getPath();
    //    CGLayout* layout0 = CGLayoutManager::getInstance()->getLayout();
    //    layout0->setName("pattern");

    //    layout0 = CGLayoutManager::getInstance()->getLayout();
    //    layout0->setName("configuration");
    CGProject::getInstance()->setName("");
    CGProject::getInstance()->setVersion("");
    QFile file(CGProject::getInstance()->getPath());
    //    QFileInfo fileInfos(file);

    //    ////qDebug << "";
    //    ////qDebug << "==========================";
    //    ////qDebug << "read " << fileInfos.absoluteFilePath();
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }

    QMap<int,int> mapOldNewControl;
    QMap<int,int> mapOldNewOperator;
    QMap<int,int> mapOldNewLayout;
    QMap<int,CGCompositionModel*> mapOldNewComposition;
    //CGCompositionManager::getInstance()->reinit();
    CGProject::getInstance()->clearProcesses();
    mapOldNewComposition.insert(0,CGCompositionManager::getInstance()->getCurrentCompositionModel());
    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    int currentLayout = -1;
    int currentComposition = 0;
    int nb_operators =0;
    int nb_controls = 0;
    int nb_compositions = 0;
    //CLogger::getInstance()->clear();
    while (!xmlReader.atEnd()) {
        xmlReader.readNext();
        if (xmlReader.isStartElement()) {
            if (!xmlReader.name().compare("cameleon") == 0 || xmlReader.attributes().value("version") != CGInstance::getInstance()->getInstanceVersion()){
                xmlReader.raiseError(QObject::tr("The cameleon project file is not compatible with this ide version."));
            }else{
                //                ////qDebug << "ide instance version " << CGInstance::getInstance()->getInstanceVersion();
            }
        }else{
            while (!xmlReader.atEnd()) {
                xmlReader.readNext();
                if (xmlReader.isStartElement()) {
                    if (xmlReader.name() == "readme"){
                        QString readme = xmlReader.attributes().value("text").toString();
                        CGProject::getInstance()->setReadMe(readme);
                    }
                    if (xmlReader.name() == "structure"){
                        QString sp = xmlReader.attributes().value("showproject").toString();
                        QString sg = xmlReader.attributes().value("showglossary").toString();
                        QString c = xmlReader.attributes().value("showcontroller").toString();
                        QString m = xmlReader.attributes().value("showmachine").toString();

                        if(c.compare("true")==0){
                            CGProject::getInstance()->setShowController(true);
                        }else{
                            CGProject::getInstance()->setShowController(false);
                        }

                    }
                    if (xmlReader.name() == "log"){
                        QString logPath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGProject::getInstance()->setLogPath(logPath);
                        CLogger::getInstance()->setPath(logPath);
                        QString b = xmlReader.attributes().value("active").toString();
                        if(b.compare("true")==0){
                            CLogger::getInstance()->setActive(true);
                        }else{
                            CLogger::getInstance()->setActive(false);
                        }
                    }
                    if (xmlReader.name() == "rule"){
                        QString componentName = xmlReader.attributes().value("component").toString();
                        QString level = xmlReader.attributes().value("level").toString();
                        CLogger::getInstance()->registerComponent(componentName);
                        CLogger::getInstance()->setComponentLevel(componentName, CLogger::getInstance()->fromString(level));
                    }
                    if (xmlReader.name() == "controllerscreen"){
                        int x = xmlReader.attributes().value("windowsx").toString().toInt();
                        int y = xmlReader.attributes().value("windowsy").toString().toInt();
                        int w = xmlReader.attributes().value("windowswidth").toString().toInt();
                        int h = xmlReader.attributes().value("windowsheight").toString().toInt();
                        int num = xmlReader.attributes().value("numscreen").toString().toInt();
                        QString full = xmlReader.attributes().value("fullscreen").toString();

                        CGLayoutManager::getInstance()->getController()->setGeometry(x,y,w,h);
                        if(num != 0){
                            QWidget* widget = QApplication::desktop()->screen(num);
                            CGLayoutManager::getInstance()->getController()->setParent(widget);
                        }
                        if(full.compare("true") == 0){
                            //                            CGLayoutManager::getInstance()->getController()->showMaximized();
                        }
                    }

                    if (xmlReader.name() == "threads"){
                        int max = xmlReader.attributes().value("max").toString().toInt();
                        CGProject::getInstance()->setNumberThread(max);

                    }
                    if (xmlReader.name() == "currentLayout"){
                        currentLayout = xmlReader.attributes().value("id").toString().toInt();
                    }
                    if (xmlReader.name() == "currentComposition"){
                        currentComposition = xmlReader.attributes().value("id").toString().toInt();
                    }
                    if (xmlReader.name() == "version"){
                        QString version = xmlReader.attributes().value("version").toString();
                        CGProject::getInstance()->setVersion(version);
                    }
                    if (xmlReader.name() == "name"){
                        QString name = xmlReader.attributes().value("name").toString();
                        CGProject::getInstance()->setName(name);
                    }
                    if (xmlReader.name() == "target"){
                        QString targetPath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGProject::getInstance()->setTargetPath(targetPath);
                    }
                    if(xmlReader.name() == "connectorOrder"){
                        QString m = xmlReader.attributes().value("mode").toString();
                        CGProject::getInstance()->setConnectorOrderMode("HARD");
                    }
                    if (xmlReader.name() == "tmp"){
                        QString tmpPath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        CGProject::getInstance()->setTmpPath(tmpPath);
                    }

                    if (xmlReader.name() == "ressource"){
                        CLogger::getInstance()->log("SERIALIZE",CLogger::INFO,"Ressources not supported by this version!");

                        //QString ressourcePath = CGProjectSerialization::makeAbsolute(xmlReader.attributes().value("path").toString());
                        //CGProject::getInstance()->setRessourcePath(ressourcePath);
                    }
                    if (xmlReader.name() == "layout"){
                        QString name = xmlReader.attributes().value("name").toString();
                        QString type = xmlReader.attributes().value("type").toString();
                        QString resolution = xmlReader.attributes().value("resolution").toString();
                        QString popup = xmlReader.attributes().value("popup").toString();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        ////qDebug << "rebuild layout name: " <<name;
                        CGLayout* layout;
                        if(id == 0){
                            layout = CGLayoutManager::getInstance()->getLayout(id);
                            layout->setType("interface");
                        }else{
                            layout = CGLayoutManager::getInstance()->getLayout();
                            layout->setName(name);
                            layout->setType(type);
                            CGProject::getInstance()->addLayout(layout);
                        }
                        layout->setResolution(resolution);

                        if(popup.compare("true")==0)layout->setIsPopup(true);
                        if(popup.compare("false")==0)layout->setIsPopup(false);

                        //check id
                        if(layout->getId() == id){
                            CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layout id!");
                        }else{
                            CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layout id!"+QString::number(id)+" "+QString::number(layout->getId()));
                        }
                        mapOldNewLayout.insert(id, layout->getId());
                    }
                    if (xmlReader.name() == "composition"){
                        if(!xmlReader.attributes().value("parentId").isNull()){
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            ////qDebug << "rebuild composition name: " <<name;
                            CGCompositionModel* compositionModel = new CGCompositionModel();
                            QString informations = xmlReader.attributes().value("informations").toString();
                            compositionModel->setInformation(informations);
                            //                            int idNew;
                            //                            CClientSingleton::getInstance()->getCInterfaceClient2Server()->createUnitProcessSend(idNew);
                            //                            compositionModel->setId(idNew);
                            mapOldNewComposition.insert(id,compositionModel);
                        }else{
                            //nothing to be done
                        }
                    }
                    if (xmlReader.name() == "numbers"){
                        nb_operators = xmlReader.attributes().value("nb_operators").toString().toInt();
                        nb_controls = xmlReader.attributes().value("nb_controls").toString().toInt();
                        nb_compositions = xmlReader.attributes().value("nb_compositions").toString().toInt();
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(10);
    int countCompo = 0;
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
                        if(!xmlReader.attributes().value("parentId").isNull()){
                            int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            int x = xmlReader.attributes().value("x").toString().toDouble();
                            int y = xmlReader.attributes().value("y").toString().toDouble();
                            int z = xmlReader.attributes().value("z").toString().toDouble();
                            //                            int rayon = xmlReader.attributes().value("rayon").toString().toDouble();
                            ////qDebug << "rebuild composition name: " <<name;

                            CGCompositionModel* par = mapOldNewComposition.value(parentId);
                            CGCompositionModel* child = mapOldNewComposition.value(id);
                            //                            CClientSingleton::getInstance()->getCInterfaceClient2Server()->linkUnitProcess(child->getId(), par->getId());
                            QString informations = xmlReader.attributes().value("informations").toString();
                            child->setInformation(informations);

                            child->setParentCompositionModel(par);
                            child->initialise();
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(child->getOuterView());

                            CGMulti* multi = child->getOuterView();
                            child->setName(name);
                            multi->setPos(x,y);
                            multi->setZValue(z);
                            multi->setIsCompatibleMode(false);
                            multi->setSelected(false);
                            multi->setSelected(false);

                            if(child->getId() == id){
                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good composition id!");
                            }else{
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad composition id! "+QString::number(id)+" "+QString::number(child->getId()));
                            }
                            CGProject::getInstance()->addComposition(child);
                        }else{
                        }
                        //progress bar update
                        countCompo++;
                        int i = 0;
                        if(nb_compositions!=0) i = 10*countCompo/nb_compositions;
                        emit progressSerial(10+i);
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(20);
    int countOp=0;
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
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                        CGCompositionModel* par = mapOldNewComposition.value(parentId);
                        if(par!=NULL){
                            CGOperatorModel* model = par->addOperatorByName(name);
                            if(model!=NULL){
                                CGOperator* view = model->getView();
                                model->getView()->setPos(x,y);

                                model->setPos(model->getView()->scenePos());
                                par->getScene()->unselectAll();
                                par->getScene()->getModel()->activate(view);
                                model->getView()->setZValue(z);
                                ////qDebug << "[check]rebuild operator type: " << name << "" << model->getId();
                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load operator named "+name);
                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load operator with position "+QString::number(x)+","+QString::number(y));
                                //                                if(model->getId() == id){
                                //                                    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good operator id!");
                                //                                }else{
                                //                                    CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad operator id!");
                                //                                }
                                mapOldNewOperator.insert(id,model->getId());
                            }else{
                                mapOldNewOperator.insert(id,-1);
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create operator "+name);
                            }
                        }
                        //progress bar update
                        countOp++;
                        int i = 0;
                        if(nb_operators!=0) i = 30*countOp/nb_operators;
                        emit progressSerial(20+i);
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(50);
    int countCt = 0;
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


                    if (xmlReader.name() == "control"){
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        QString content = xmlReader.attributes().value("content").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        int width = xmlReader.attributes().value("width").toString().toDouble();
                        int height = xmlReader.attributes().value("height").toString().toDouble();
                        QString ispers = xmlReader.attributes().value("persistent").toString();

                        ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                        QRect rect(0,0,width,height);
                        parentId = mapOldNewLayout.value(parentId);
                        CGLayout* par = CGLayoutManager::getInstance()->getLayout(parentId);
                        CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name,par);
                        if(layoutItem!=NULL && layoutItem->getControl()!=NULL){
                            layoutItem->setSelected(false);
                            layoutItem->setPos(x,y);
                            if(ispers.compare("true") == 0){
                                layoutItem->setIsPersistant(true);
                            }else{
                                layoutItem->setIsPersistant(false);
                            }

                            //                        layoutItem->resize(rect.bottomRight());
                            //                        layoutItem->setRect(rect);
                            par->activate(layoutItem);
                            layoutItem->setZValue(z);
                            layoutItem->resize(rect);
                            ////qDebug << "[check]rebuild layoutItem type: " << name << "" << layoutItem->getControl()->getId();
                            //                            if(layoutItem->getControl()->getId() == id){
                            //                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layoutItem id!");
                            //                            }else{
                            //                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layoutItem id!");
                            //                            }

                            layoutItem->getControl()->fromString(content.toStdString());
                            mapOldNewControl.insert(id,layoutItem->getControl()->getId());
                        }else{
                            CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create control "+name);
                            mapOldNewControl.insert(id,-1);
                        }
                        countCt++;
                        int i = 0;
                        if(nb_controls!=0) i = 20*countCt/nb_controls;
                        emit progressSerial(50+i);
                    }
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(70);

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


                    if (xmlReader.name() == "compositioncontrol"){
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        if(name.compare("RessourceProject") == 0){
                            CLogger::getInstance()->log("SERIALIZE",CLogger::INFO,"Ressources not supported by this version!");
                        }else{
                            QString content = xmlReader.attributes().value("content").toString();
                            int x = xmlReader.attributes().value("x").toString().toDouble();
                            int y = xmlReader.attributes().value("y").toString().toDouble();
                            int z = xmlReader.attributes().value("z").toString().toDouble();
                            int width = xmlReader.attributes().value("width").toString().toDouble();
                            int height = xmlReader.attributes().value("height").toString().toDouble();
                            QString ispers = xmlReader.attributes().value("persistent").toString();

                            ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                            QRect rect(0,0,width,height);
                            CGCompositionModel* compM = mapOldNewComposition.value(parentId);
                            CGComposition* comp = compM->getScene();
                            CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name,comp);
                            if(layoutItem!=NULL && layoutItem->getControl()!=NULL){
                                layoutItem->setSelected(false);
                                layoutItem->setPos(x,y);
                                if(ispers.compare("true") == 0){
                                    layoutItem->setIsPersistant(true);
                                }else{
                                    layoutItem->setIsPersistant(false);
                                }

                                //                        layoutItem->resize(rect.bottomRight());
                                //                        layoutItem->setRect(rect);
                                layoutItem->setZValue(z);
                                compM->activate(layoutItem);
                                layoutItem->resize(rect);
                                ////qDebug << "[check]rebuild layoutItem type: " << name << "" << layoutItem->getControl()->getId();
                                //                            if(layoutItem->getControl()->getId() == id){
                                //                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layoutItem id!");
                                //                            }else{
                                //                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layoutItem id!");
                                //                            }

                                layoutItem->getControl()->fromString(content.toStdString());
                                mapOldNewControl.insert(id,layoutItem->getControl()->getId());
                                layoutItem->getControl()->apply();
                            }else{
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create control "+name);
                                mapOldNewControl.insert(id,-1);
                            }
                        }
                    }
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(70);

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

                    if (xmlReader.name() == "shape"){
                        ////qDebug << "rebuild shape" <<CGProject::getInstance()->getName();
                        int parentid = xmlReader.attributes().value("parentId").toString().toInt();
                        QString color = xmlReader.attributes().value("color").toString();
                        int xrect = xmlReader.attributes().value("xrect").toString().toDouble();
                        int yrect = xmlReader.attributes().value("yrect").toString().toDouble();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        int width = xmlReader.attributes().value("width").toString().toDouble();
                        int height = xmlReader.attributes().value("height").toString().toDouble();
                        QString ispers = xmlReader.attributes().value("persistent").toString();
                        QRect rect(xrect,yrect,width,height);
                        if(!xmlReader.attributes().value("fromLayout").isNull()){
                            parentid = mapOldNewLayout.value(parentid);
                            CGLayout* lay = CGLayoutManager::getInstance()->getLayout(parentid);

                            CGShape* shape = new CGShape(0,0,lay);
                            //                            lay->addItem(shape);
                            QColor c;
                            c.setNamedColor(color);
                            shape->setColor(color);
                            shape->setRect(rect);
                            shape->setPos(x,y);
                            shape->setZValue(z);
                            if(ispers.compare("true") == 0){
                                shape->setPersistent(true);
                            }else{
                                shape->setPersistent(false);
                            }
                        }else{
                            CGCompositionModel* par = mapOldNewComposition.value(parentid);
                            //                            CGCompositionModel* par = CGCompositionManager::getInstance()->getCompositionById(parentid);
                            CGShape* shape = new CGShape(0,0,par->getScene());
                            //                            par->getScene()->addItem(shape);
                            shape->setRect(rect);
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(shape);
                            QColor c;
                            c.setNamedColor(color);
                            shape->setPos(x,y);
                            shape->setColor(color);
                            shape->setZValue(z);
                        }
                    }

                    if (xmlReader.name() == "text"){
                        ////qDebug << "rebuild text";
                        int parentid = xmlReader.attributes().value("parentId").toString().toInt();
                        QString content = xmlReader.attributes().value("content").toString();
                        QString color = xmlReader.attributes().value("color").toString();
                        int police = xmlReader.attributes().value("police").toString().toInt();
                        QString font = xmlReader.attributes().value("font").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        QString italic = xmlReader.attributes().value("italic").toString();
                        QString bold = xmlReader.attributes().value("bold").toString();
                        QString underline = xmlReader.attributes().value("underline").toString();

                        if(!xmlReader.attributes().value("fromLayout").isNull()){
                            CGText* text = new CGText();
                            parentid = mapOldNewLayout.value(parentid);
                            CGLayout* lay = CGLayoutManager::getInstance()->getLayout(parentid);
                            //                            text->setFont(layout->font());
                            text->setTextInteractionFlags(Qt::TextEditorInteraction);
                            text->setZValue(1000.0);

                            QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                             lay, SLOT(editorLostFocus(CGText*)));
                            QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                             lay, SIGNAL(itemSelected(QGraphicsItem*)));
                            lay->addItem(text);
                            text->setPos(x,y);
                            text->setZValue(z);
                            text->setPlainText(content);

                            QColor c;
                            c.setNamedColor(color);
                            text->setDefaultTextColor(c);
                            QFont f;
                            f.setFamily(font);
                            f.setPointSize(police);

                            if(italic.compare("true") == 0){
                                f.setItalic(true);
                            }
                            if(bold.compare("true") == 0){
                                f.setBold(true);
                            }
                            if(underline.compare("true") == 0){
                                f.setUnderline(true);
                            }
                            text->setFont(f);

                            QString ispers = xmlReader.attributes().value("persistent").toString();
                            if(ispers.compare("true") == 0){
                                text->setPersistent(true);
                            }else{
                                text->setPersistent(false);
                            }
                        }else{
                            CGText* text = new CGText();
                            CGCompositionModel* par = mapOldNewComposition.value(parentid);
                            //                            CGCompositionModel* par = CGCompositionManager::getInstance()->getCompositionById(parentid);
                            text->setFont(par->getScene()->font());
                            text->setTextInteractionFlags(Qt::TextEditorInteraction);
                            text->setZValue(10000.0);

                            QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                             par->getScene(), SLOT(editorLostFocus(CGText*)));
                            QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                             par->getScene(), SIGNAL(itemSelected(QGraphicsItem*)));
                            par->getScene()->addItem(text);

                            text->setDefaultTextColor(Qt::white);
                            text->setPos(x,y);
                            text->setZValue(z);
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(text);
                            text->setPlainText(content);

                            QColor c;
                            c.setNamedColor(color);
                            text->setDefaultTextColor(c);

                            QFont f;
                            f.setFamily(font);
                            f.setPointSize(police);

                            if(italic.compare("true") == 0){
                                f.setItalic(true);
                            }
                            if(bold.compare("true") == 0){
                                f.setBold(true);
                            }
                            if(underline.compare("true") == 0){
                                f.setUnderline(true);
                            }
                            text->setFont(f);
                        }
                    }

                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);


    emit progressSerial(80);
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
                    if (xmlReader.name() == "operatoraction"){
                        int id = xmlReader.attributes().value("opId").toString().toInt();
                        QString actionKey = xmlReader.attributes().value("actionKey").toString();
                        int newId = mapOldNewOperator.value(id);
                        CClientSingleton::getInstance()->getCInterfaceClient2Server()->actionOperator(newId, actionKey.toStdString());

                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);

    emit progressSerial(80);
    QList<CGMultiConnector*> multiConnectorList;
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
                        int compositionId = xmlReader.attributes().value("compositionId").toString().toInt();
                        QString side = xmlReader.attributes().value("side").toString();
                        if(xmlReader.attributes().value("x").isNull()){
                            CGCompositionModel* model = mapOldNewComposition.value(compositionId);
                            QString name = xmlReader.attributes().value("name").toString();
                            if(side.compare("IN") == 0){
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("OUTIN");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                            }
                            if(side.compare("OUT") == 0) {
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("INOUT");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                            }
                        }else{
                            double x = xmlReader.attributes().value("x").toString().toDouble();
                            double y = xmlReader.attributes().value("y").toString().toDouble();

                            CGCompositionModel* model = mapOldNewComposition.value(compositionId);
                            QString name = xmlReader.attributes().value("name").toString();
                            if(side.compare("IN") == 0){
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("OUTIN");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                                QPointF p(x,y);
                                multiConnectorList.append(m);
                                m->getOuterConnector()->setPos(p);

                            }
                            if(side.compare("OUT") == 0) {
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("INOUT");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();

                                QPointF p(x,y);
                                multiConnectorList.append(m);
                                m->getOuterConnector()->setPos(p);
                            }
                        }
                    }
                }
            }
        }
    }

    foreach(CGMultiConnector*m,multiConnectorList){
        double r = (m->getOuterConnector()->getRayon())/2;
        double teta = acos(m->getOuterConnector()->pos().x()/r)*(SCD::sgn(m->getOuterConnector()->pos().y()));
        m->getOuterConnector()->rePosition();

        double x1 = cos(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
        double y1 = sin(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
        m->getInnerConnector()->setPos(x1,y1);
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(90);

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
                    if (xmlReader.name() == "connection"){
                        if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull() ){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int operatorId1 = xmlReader.attributes().value("operatorId1").toString().toInt();
                            int operatorId2 = xmlReader.attributes().value("operatorId2").toString().toInt();
                            if(mapOldNewOperator.value(operatorId1)!=-1 && mapOldNewOperator.value(operatorId2)!=-1){
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId1));
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId2));
                                if(ope1->getView()->getConnectorById(connectorId1)!=0 && ope2->getView()->getConnectorById(connectorId2)!=0) ope1->getView()->getConnectorById(connectorId1)->connect(ope2->getView()->getConnectorById(connectorId2));
                            }
                        }
                        if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull()){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int compositionId1 = xmlReader.attributes().value("compositionId1").toString().toInt();
                            int operatorId2 = xmlReader.attributes().value("operatorId2").toString().toInt();
                            if(mapOldNewOperator.value(operatorId2)!=-1){
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId2));
                                CGCompositionModel* model = mapOldNewComposition.value(compositionId1);
                                CGConnector* c2 = ope2->getView()->getConnectorById(connectorId2);
                                CGConnector* cm;
                                if(ope2->getView()->scene() == model->getOuterView()->scene()){
                                    cm = model->getOuterView()->getConnectorById(connectorId1);
                                }else{
                                    cm = model->getInnerView()->getConnectorById(connectorId1);
                                }
                                if(c2 != 0 && cm != 0) c2->connect(cm);
                            }
                        }
                        if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int operatorId1 = xmlReader.attributes().value("operatorId1").toString().toInt();
                            int compositionId2 = xmlReader.attributes().value("compositionId2").toString().toInt();
                            if(mapOldNewOperator.value(operatorId1)!=-1){
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId1));
                                CGCompositionModel* model = mapOldNewComposition.value(compositionId2);

                                CGConnector* c2 = ope2->getView()->getConnectorById(connectorId1);
                                CGConnector* cm;
                                if(ope2->getView()->scene() == model->getOuterView()->scene()){
                                    cm = model->getOuterView()->getConnectorById(connectorId2);
                                }else{
                                    cm = model->getInnerView()->getConnectorById(connectorId2);
                                }
                                if(c2 != 0 && cm != 0) c2->connect(cm);
                            }
                        }
                        if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                            int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                            int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                            int compositionId1 = xmlReader.attributes().value("compositionId1").toString().toInt();
                            int compositionId2 = xmlReader.attributes().value("compositionId2").toString().toInt();
                            CGCompositionModel* model1 = mapOldNewComposition.value(compositionId1);
                            CGCompositionModel* model2 = mapOldNewComposition.value(compositionId2);
                            if(model1->getParentCompositionModel() == model2){
                                model1->getOuterView()->getConnectorById(connectorId1)->connect(model2->getInnerView()->getConnectorById(connectorId2));
                            }
                            if(model1 == model2->getParentCompositionModel()){
                                model1->getInnerView()->getConnectorById(connectorId1)->connect(model2->getOuterView()->getConnectorById(connectorId2));
                            }
                            if(model1->getParentCompositionModel() == model2->getParentCompositionModel()){
                                model1->getOuterView()->getConnectorById(connectorId1)->connect(model2->getOuterView()->getConnectorById(connectorId2));
                            }
                        }
                    }
                }
            }
        }
    }

    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);
    emit progressSerial(90);

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
                    if (xmlReader.name() == "connection"){
                        if(!xmlReader.attributes().value("controlConnectorIdIn").isNull() && !xmlReader.attributes().value("operatorConnectorIdOut").isNull()){
                            int controlId = xmlReader.attributes().value("controlId").toString().toInt();
                            int operatorIdOut = xmlReader.attributes().value("operatorIdOut").toString().toInt();
                            int controlConnectorIdIn = xmlReader.attributes().value("controlConnectorIdIn").toString().toInt();
                            int operatorConnectorIdOut = xmlReader.attributes().value("operatorConnectorIdOut").toString().toInt();
                            if(mapOldNewOperator.value(operatorIdOut)!=-1 && mapOldNewControl.value(controlId) != -1){
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorIdOut));
                                CGLayoutItem* layoutItem = CGLayoutManager::getInstance()->getItemById(mapOldNewControl.value(controlId));
                                if(ope1 != NULL && layoutItem != NULL){
                                    if(ope1->getView()->getConnectorById(operatorConnectorIdOut)!=0
                                            &&layoutItem->getConnectorById(controlConnectorIdIn)!=0){
                                        //TODO: this control doesn't have to be there. Miss design here !
                                        if(ope1->getView()->getConnectorById(operatorConnectorIdOut)->getSide().compare(layoutItem->getConnectorById(controlConnectorIdIn)->getSide()) != 0){
                                            layoutItem->getConnectorById(controlConnectorIdIn)->setIsControlConnected(true);
                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->setIsControlConnected(true);
                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->connect(layoutItem->getConnectorById(controlConnectorIdIn));
                                            layoutItem->getConnectorById(controlConnectorIdIn)->connect(ope1->getView()->getConnectorById(operatorConnectorIdOut));

                                            layoutItem->getConnectorById(controlConnectorIdIn)->update();
                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->update();
                                        }
                                    }
                                }
                            }
                        }

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
                        int id = xmlReader.attributes().value("connectorId").toString().toInt();
                        int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                        double x = xmlReader.attributes().value("x").toString().toDouble();
                        double y = xmlReader.attributes().value("y").toString().toDouble();
                        CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                        if(ope1!=0){
                            CGConnector* c = ope1->getView()->getConnectorById(id);
                            QPointF p(x,y);
                            if(c!=0)c->setPos(p);
                        }
                    }
                }
            }
        }
    }

    CGProject::getInstance()->loadRessources();

    //CGCompositionModel* child = CGCompositionManager::getInstance()->getCompositionById(currentC);
//    qDebug()<<"SECOND ======================";
   // if(currentL!=-1) CGLayoutManager::getInstance()->showLayout(currentL);
//    qDebug()<<CGLayoutManager::getInstance()->getCurrentLayout()->getView()->verticalScrollBar()->value();
//    qDebug()<<CGLayoutManager::getInstance()->getCurrentLayout()->getView()->horizontalScrollBar()->value();
   // CGCompositionManager::getInstance()->showSceneFromModel(child);
//    qDebug()<<"SECOND ======================";

    //start project logger
    CLogger::getInstance()->start();
}



void CGProjectSerialization::saveAs(QString path, CGCompositionModel* topModel, QList<CGLayout*> layoutL, bool patterned, bool isInterface){
    if (!patterned) CGProjectSerialization::createProject(path);
    QString savedPath = path;
    path = path+".tmp";
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Save project "+path+" ...");
    QFile file(path);
    QFileInfo fileInfos(file);
    ////qDebug << "";
    ////qDebug << "==========================";
    ////qDebug << "CGProject::saveAs " << fileInfos.absoluteFilePath();
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }
    CGProject::getInstance()->setPath(fileInfos.absoluteFilePath());

    QXmlStreamWriter xmlWriter;
    xmlWriter.setAutoFormatting ( true );
    xmlWriter.setDevice(&file);
    xmlWriter.writeStartDocument();
    xmlWriter.writeDTD("<!DOCTYPE cameleon>");
    xmlWriter.writeStartElement("cameleon");
    xmlWriter.writeAttribute("version", CGInstance::getInstance()->getInstanceVersion());
    if(!patterned){

        xmlWriter.writeStartElement("readme");
        xmlWriter.writeAttribute("text", CGProject::getInstance()->getReadMe());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("log");
        xmlWriter.writeAttribute("path", CGProjectSerialization::makeRelative(CGProject::getInstance()->getLogPath()));
        if(CLogger::getInstance()->getActive())xmlWriter.writeAttribute("active", "true");
        if(!CLogger::getInstance()->getActive())xmlWriter.writeAttribute("active", "false");
        QMap<QString, CLogger::LVL> logRules = CLogger::getInstance()->getMap();
        QMap<QString, CLogger::LVL>::iterator i = logRules.begin();
        while (i != logRules.end()) {
            xmlWriter.writeStartElement("rule");
            xmlWriter.writeAttribute("component", i.key());
            xmlWriter.writeAttribute("level", CLogger::toString(i.value()));
            xmlWriter.writeEndElement();
            i++;
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("structure");
        if(CGCompositionManager::getInstance()->getComposer()->getMachine()->isHidden()){
            xmlWriter.writeAttribute("showmachine", "false");
        }else{
            xmlWriter.writeAttribute("showmachine", "true");
        }
        if(CGCompositionManager::getInstance()->getComposer()->getProject()->isHidden()){
            xmlWriter.writeAttribute("showproject", "false");
        }else{
            xmlWriter.writeAttribute("showproject", "true");
        }
        if(CGCompositionManager::getInstance()->getComposer()->getGlossary()->isHidden()){
            xmlWriter.writeAttribute("showglossary", "false");
        }else{
            xmlWriter.writeAttribute("showglossary", "true");
        }
        if(CGLayoutManager::getInstance()->getController()->isHidden()){
            xmlWriter.writeAttribute("showcontroller", "false");
        }else{
            xmlWriter.writeAttribute("showcontroller", "true");
        }
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("tmp");
        xmlWriter.writeAttribute("path", CGProjectSerialization::makeRelative(CGProject::getInstance()->getTmpPath()));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("ressource");
        xmlWriter.writeAttribute("path", CGProjectSerialization::makeRelative(CGProject::getInstance()->getRessourcePath()));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("name");
        xmlWriter.writeAttribute("name", CGProject::getInstance()->getName());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("version");
        xmlWriter.writeAttribute("version", CGProject::getInstance()->getVersion());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("connectorOrder");
        xmlWriter.writeAttribute("mode", "HARD");
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("target");
        xmlWriter.writeAttribute("path", CGProjectSerialization::makeRelative(CGProject::getInstance()->getTargetPath()));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("threads");
        xmlWriter.writeAttribute("max", QString::number(CGProject::getInstance()->getNumberThread()));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("numbers");

        xmlWriter.writeAttribute("nb_operators", QString::number(topModel->getOperatorModelList().size()));
        xmlWriter.writeAttribute("nb_controls", 0);
        xmlWriter.writeAttribute("nb_compositions", QString::number(CGCompositionManager::getInstance()->getCompositionList(topModel).size()));
        xmlWriter.writeEndElement();


        //=================================================
        //LAYOUTS
        xmlWriter.writeStartElement("controllerscreen");
        if(CGLayoutManager::getInstance()->getCurrentLayout() != 0){
            xmlWriter.writeAttribute("viewx", QString::number(CGLayoutManager::getInstance()->getCurrentLayout()->getView()->horizontalScrollBar()->value()));
            xmlWriter.writeAttribute("viewy", QString::number(CGLayoutManager::getInstance()->getCurrentLayout()->getView()->verticalScrollBar()->value()));
            xmlWriter.writeAttribute("viewscale", QString::number(CGLayoutManager::getInstance()->getCurrentLayout()->getView()->getScaleNum()));
        }
        xmlWriter.writeAttribute("windowsx", QString::number(CGLayoutManager::getInstance()->getController()->geometry().x()));
        xmlWriter.writeAttribute("windowsy", QString::number(CGLayoutManager::getInstance()->getController()->geometry().y()));
        xmlWriter.writeAttribute("windowswidth", QString::number(CGLayoutManager::getInstance()->getController()->geometry().width()));
        xmlWriter.writeAttribute("windowsheight", QString::number(CGLayoutManager::getInstance()->getController()->geometry().height()));
        xmlWriter.writeAttribute("numscreen", QString::number(QApplication::desktop()->screenNumber(CGLayoutManager::getInstance()->getController())));
        if(CGLayoutManager::getInstance()->getController()->isMaximized()){
            xmlWriter.writeAttribute("fullscreen", "true");
        }else{
            xmlWriter.writeAttribute("fullscreen", "false");
        }
        //        xmlWriter.writeAttribute("state", CGLayoutManager::getInstance()->getController()->saveState());
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("layouts");
        if(CGLayoutManager::getInstance()->getCurrentLayout() != 0){
            xmlWriter.writeStartElement("currentLayout");
            xmlWriter.writeAttribute("id", QString::number(CGLayoutManager::getInstance()->getCurrentLayout()->getId()));
            xmlWriter.writeEndElement();
        }
        foreach(CGLayout* layout, layoutL){
            xmlWriter.writeStartElement("layout");
            xmlWriter.writeAttribute("id", QString::number(layout->getId()));
            xmlWriter.writeAttribute("type", layout->getType());
            xmlWriter.writeAttribute("name", layout->getName());
            xmlWriter.writeAttribute("viewx", QString::number(layout->getView()->horizontalScrollBar()->value()));
            xmlWriter.writeAttribute("viewy", QString::number(layout->getView()->verticalScrollBar()->value()));
            xmlWriter.writeAttribute("viewscale", QString::number(layout->getView()->getScaleNum()));
            if(layout->getIsPopup()) {
                xmlWriter.writeAttribute("popup", "true");
            }else{
                xmlWriter.writeAttribute("popup", "false");
            }
            xmlWriter.writeAttribute("resolution", layout->getResolution());
            xmlWriter.writeEndElement();
        }

        foreach(CGLayout* layout, layoutL){
            QList<QGraphicsItem*> itemsL = layout->items();
            foreach(QGraphicsItem* item, itemsL){
                if(item->type() == CGLayoutItem::Type){
                    CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                    xmlWriter.writeStartElement("control");
                    xmlWriter.writeAttribute("id", QString::number(ctrl->getControl()->getId()));
                    xmlWriter.writeAttribute("type", ctrl->getControl()->getKey().c_str());
                    if(ctrl->getIsPersistant()){
                        xmlWriter.writeAttribute("persistent", "true");
                    }else{
                        xmlWriter.writeAttribute("persistent", "false");
                    }
                    xmlWriter.writeAttribute("x", QString::number(ctrl->scenePos().x()));
                    xmlWriter.writeAttribute("y", QString::number(ctrl->scenePos().y()));
                    xmlWriter.writeAttribute("z", QString::number(ctrl->zValue()));
                    xmlWriter.writeAttribute("width", QString::number(ctrl->getRect().width()));
                    xmlWriter.writeAttribute("height", QString::number(ctrl->getRect().height()));
                    xmlWriter.writeAttribute("parentId", QString::number(layout->getId()));
                    xmlWriter.writeAttribute("content",QString::fromStdString(ctrl->getControl()->toString()));
                    xmlWriter.writeEndElement();

                    //patch #105
                    //prendre le connector out et écrire:
                    //<controlconnector controlid="" connectorid="" name="XXX" datatype="XXX" content="XXXXXX"\/>
                    foreach(CGLayoutItemConnector* connector, ctrl->getConnectorsL()){
                        xmlWriter.writeStartElement("controlconnector");
                        xmlWriter.writeAttribute("controlid", QString::number(ctrl->getControl()->getId()));
                        xmlWriter.writeAttribute("connectorid", QString::number(connector->getId()));
                        xmlWriter.writeAttribute("side", connector->getModel()->getSide());
                        xmlWriter.writeAttribute("name", connector->getModel()->getExposedName());
                        xmlWriter.writeAttribute("datatype", connector->getModel()->getDataType());
                        xmlWriter.writeAttribute("content", ctrl->getControl()->getData());
                        xmlWriter.writeEndElement();
                    }
                }
                if(item->type() == CGText::Type){
                    CGText *text = qgraphicsitem_cast<CGText *>(item);
                    xmlWriter.writeStartElement("text");
                    xmlWriter.writeAttribute("fromLayout","true");
                    if(text->getPersistent()){
                        xmlWriter.writeAttribute("persistent", "true");
                    }else{
                        xmlWriter.writeAttribute("persistent", "false");
                    }
                    xmlWriter.writeAttribute("x", QString::number(text->scenePos().x()));
                    xmlWriter.writeAttribute("y", QString::number(text->scenePos().y()));
                    xmlWriter.writeAttribute("z", QString::number(text->zValue()));
                    xmlWriter.writeAttribute("parentId", QString::number(layout->getId()));
                    xmlWriter.writeAttribute("color",text->defaultTextColor().name());
                    xmlWriter.writeAttribute("police",QString::number(text->font().pointSize()));
                    xmlWriter.writeAttribute("font",text->font().family());
                    xmlWriter.writeAttribute("italic",QString(text->font().italic()?"true":"false"));
                    xmlWriter.writeAttribute("bold",QString(text->font().bold()?"true":"false"));
                    xmlWriter.writeAttribute("underline",QString(text->font().underline()?"true":"false"));
                    xmlWriter.writeAttribute("content",text->toPlainText());
                    xmlWriter.writeEndElement();
                }
                if(item->type() == CGShape::Type){
                    CGShape *shape =
                            qgraphicsitem_cast<CGShape *>(item);
                    xmlWriter.writeStartElement("shape");
                    if(shape->getPersistent()){
                        xmlWriter.writeAttribute("persistent", "true");
                    }else{
                        xmlWriter.writeAttribute("persistent", "false");
                    }
                    xmlWriter.writeAttribute("x", QString::number(shape->scenePos().x()));
                    xmlWriter.writeAttribute("y", QString::number(shape->scenePos().y()));
                    xmlWriter.writeAttribute("z", QString::number(shape->zValue()));
                    xmlWriter.writeAttribute("width", QString::number(shape->getRect().width()));
                    xmlWriter.writeAttribute("xrect", QString::number(shape->getRect().x()));
                    xmlWriter.writeAttribute("yrect", QString::number(shape->getRect().y()));
                    xmlWriter.writeAttribute("height", QString::number(shape->getRect().height()));
                    xmlWriter.writeAttribute("parentId", QString::number(layout->getId()));
                    xmlWriter.writeAttribute("fromLayout","true");
                    xmlWriter.writeAttribute("color",shape->getColor().name());
                    xmlWriter.writeEndElement();
                }
            }
        }

        foreach(CGLayout* layout, layoutL){
            QList<QGraphicsItem*> itemsL = layout->items();
            foreach(QGraphicsItem* item, itemsL){
                if(item->type() == CGLayoutItem::Type){
                    CGLayoutItem *ope = qgraphicsitem_cast<CGLayoutItem *>(item);
                    foreach(CGConnector* c, ope->getConnectorsL()){
                        if(c->isConnected()){
                            xmlWriter.writeStartElement("connection");
                            xmlWriter.writeAttribute("controlId", QString::number(ope->getControl()->getId()));
                            xmlWriter.writeAttribute("operatorIdOut", QString::number(c->getConnectedConnector()->getOperator()->getModel()->getId()));
                            xmlWriter.writeAttribute("controlConnectorIdIn", QString::number(c->getId()));
                            xmlWriter.writeAttribute("operatorConnectorIdOut", QString::number(c->getConnectedConnector()->getId()));
                            xmlWriter.writeEndElement();
                        }
                    }
                }
            }
        }

        xmlWriter.writeEndElement();
    }else{
        if(isInterface){
            xmlWriter.writeStartElement("mode");
            xmlWriter.writeAttribute("isInterface", "true");
            xmlWriter.writeEndElement();
        }else{
            xmlWriter.writeStartElement("mode");
            xmlWriter.writeAttribute("isInterface", "false");
            xmlWriter.writeEndElement();
        }
        xmlWriter.writeStartElement("layouts");
        QList<CGLayoutItem*> ctrlList;
        QList<CGLayout*> layoutList;

        foreach(CGOperatorModel* ope, topModel->getOperatorModelList()){
            foreach(CGConnector* connector, ope->getView()->getConnectorsList()){
                if(connector->getIsControlConnected()){
                    QGraphicsItem* item = connector->getConnectedControlConnector()->parentItem();
                    CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                    if(!ctrlList.contains(ctrl)) ctrlList.append(ctrl);
                    if(ctrl->getLayout()!=0 && !layoutList.contains(ctrl->getLayout())) layoutList.append(ctrl->getLayout());
                }
            }
        }
        foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCompositionList(topModel)){
            foreach(CGOperatorModel* ope, model->getOperatorModelList()){
                foreach(CGConnector* connector, ope->getView()->getConnectorsList()){
                    if(connector->getIsControlConnected()){
                        QGraphicsItem* item = connector->getConnectedControlConnector()->parentItem();
                        CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                        if(!ctrlList.contains(ctrl)) ctrlList.append(ctrl);
                        if(ctrl->getLayout()!=0 && !layoutList.contains(ctrl->getLayout())) layoutList.append(ctrl->getLayout());
                    }
                }
            }
        }
        foreach(CGLayout* layout, layoutList){
            xmlWriter.writeStartElement("layout");
            xmlWriter.writeAttribute("id", QString::number(layout->getId()));
            xmlWriter.writeAttribute("type", layout->getType());
            xmlWriter.writeAttribute("name", layout->getName());
            xmlWriter.writeEndElement();

            foreach(QGraphicsItem* item, layout->items()){
                if(item->type() == CGText::Type){
                    CGText *text = qgraphicsitem_cast<CGText *>(item);
                    xmlWriter.writeStartElement("text");
                    xmlWriter.writeAttribute("fromLayout","true");
                    if(text->getPersistent()){
                        xmlWriter.writeAttribute("persistent", "true");
                    }else{
                        xmlWriter.writeAttribute("persistent", "false");
                    }
                    xmlWriter.writeAttribute("x", QString::number(text->scenePos().x()));
                    xmlWriter.writeAttribute("y", QString::number(text->scenePos().y()));
                    xmlWriter.writeAttribute("z", QString::number(text->zValue()));
                    xmlWriter.writeAttribute("parentId", QString::number(layout->getId()));
                    xmlWriter.writeAttribute("color",text->defaultTextColor().name());
                    xmlWriter.writeAttribute("police",QString::number(text->font().pointSize()));
                    xmlWriter.writeAttribute("font",text->font().family());
                    xmlWriter.writeAttribute("italic",QString(text->font().italic()?"true":"false"));

                    xmlWriter.writeAttribute("bold",QString(text->font().bold()?"true":"false"));
                    xmlWriter.writeAttribute("underline",QString(text->font().underline()?"true":"false"));
                    xmlWriter.writeAttribute("content",text->toPlainText());
                    xmlWriter.writeEndElement();
                }
                if(item->type() == CGShape::Type){
                    CGShape *shape =
                            qgraphicsitem_cast<CGShape *>(item);
                    xmlWriter.writeStartElement("shape");
                    if(shape->getPersistent()){
                        xmlWriter.writeAttribute("persistent", "true");
                    }else{
                        xmlWriter.writeAttribute("persistent", "false");
                    }
                    xmlWriter.writeAttribute("x", QString::number(shape->scenePos().x()));
                    xmlWriter.writeAttribute("y", QString::number(shape->scenePos().y()));
                    xmlWriter.writeAttribute("z", QString::number(shape->zValue()));
                    xmlWriter.writeAttribute("width", QString::number(shape->getRect().width()));
                    xmlWriter.writeAttribute("xrect", QString::number(shape->getRect().x()));
                    xmlWriter.writeAttribute("yrect", QString::number(shape->getRect().y()));
                    xmlWriter.writeAttribute("height", QString::number(shape->getRect().height()));
                    xmlWriter.writeAttribute("parentId", QString::number(layout->getId()));
                    xmlWriter.writeAttribute("fromLayout","true");
                    xmlWriter.writeAttribute("color",shape->getColor().name());
                    xmlWriter.writeEndElement();
                }
            }


        }
        foreach(CGLayoutItem* ctrl, ctrlList){
            if(ctrl->getLayout()!=0){
                xmlWriter.writeStartElement("control");
                xmlWriter.writeAttribute("id", QString::number(ctrl->getControl()->getId()));
                xmlWriter.writeAttribute("type", ctrl->getControl()->getKey().c_str());
                xmlWriter.writeAttribute("x", QString::number(ctrl->scenePos().x()));
                xmlWriter.writeAttribute("y", QString::number(ctrl->scenePos().y()));
                xmlWriter.writeAttribute("z", QString::number(ctrl->zValue()));
                xmlWriter.writeAttribute("width", QString::number(ctrl->getRect().width()));
                xmlWriter.writeAttribute("height", QString::number(ctrl->getRect().height()));
                xmlWriter.writeAttribute("parentId", QString::number(ctrl->getLayout()->getId()));
                xmlWriter.writeAttribute("content",QString::fromStdString(ctrl->getControl()->toString()));
                xmlWriter.writeEndElement();
            }
        }

        foreach(CGOperatorModel* ope, topModel->getOperatorModelList()){
            foreach(CGConnector* connector, ope->getView()->getConnectorsList()){

                if(connector->getIsControlConnected()){
                    QGraphicsItem* item = connector->getConnectedControlConnector()->parentItem();
                    CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                    xmlWriter.writeStartElement("connection");
                    xmlWriter.writeAttribute("controlId", QString::number(ctrl->getControl()->getId()));
                    xmlWriter.writeAttribute("operatorIdOut", QString::number(connector->getOperator()->getModel()->getId()));
                    xmlWriter.writeAttribute("controlConnectorIdIn", QString::number(connector->getConnectedControlConnector()->getId()));
                    xmlWriter.writeAttribute("operatorConnectorIdOut", QString::number(connector->getId()));
                    xmlWriter.writeEndElement();
                }
            }
        }
        foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCompositionList(topModel)){
            foreach(CGOperatorModel* ope, model->getOperatorModelList()){
                foreach(CGConnector* connector, ope->getView()->getConnectorsList()){
                    if(connector->getIsControlConnected()){
                        QGraphicsItem* item = connector->getConnectedControlConnector()->parentItem();
                        CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                        xmlWriter.writeStartElement("connection");
                        xmlWriter.writeAttribute("controlId", QString::number(ctrl->getControl()->getId()));
                        xmlWriter.writeAttribute("operatorIdOut", QString::number(connector->getOperator()->getModel()->getId()));
                        xmlWriter.writeAttribute("controlConnectorIdIn", QString::number(connector->getConnectedControlConnector()->getId()));
                        xmlWriter.writeAttribute("operatorConnectorIdOut", QString::number(connector->getId()));
                        xmlWriter.writeEndElement();
                    }
                }
            }
        }
        xmlWriter.writeEndElement();
    }
    //=================================================
    //COMPOSITION
    xmlWriter.writeStartElement("composerscreen");
    xmlWriter.writeAttribute("viewx", QString::number(CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->horizontalScrollBar()->value()));
    xmlWriter.writeAttribute("viewy", QString::number(CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->verticalScrollBar()->value()));
    xmlWriter.writeAttribute("viewscale", QString::number(CGCompositionManager::getInstance()->getCurrentComposerScene()->getView()->getScaleNum()));
    xmlWriter.writeAttribute("windowsx", QString::number(CGCompositionManager::getInstance()->getComposer()->geometry().x()));
    xmlWriter.writeAttribute("windowsy", QString::number(CGCompositionManager::getInstance()->getComposer()->geometry().y()));
    xmlWriter.writeAttribute("windowswidth", QString::number(CGCompositionManager::getInstance()->getComposer()->geometry().width()));
    xmlWriter.writeAttribute("windowsheight", QString::number(CGCompositionManager::getInstance()->getComposer()->geometry().height()));
    xmlWriter.writeAttribute("numscreen", QString::number(QApplication::desktop()->screenNumber(CGCompositionManager::getInstance()->getComposer())));
    //    xmlWriter.writeAttribute("state", QString(CGCompositionManager::getInstance()->getComposer()->saveState()));
    if(CGCompositionManager::getInstance()->getComposer()->isMaximized()){
        xmlWriter.writeAttribute("fullscreen", "true");
    }else{
        xmlWriter.writeAttribute("fullscreen", "false");
    }
    xmlWriter.writeEndElement();

    xmlWriter.writeStartElement("compositions");
    xmlWriter.writeStartElement("currentComposition");
    if(!patterned){
        xmlWriter.writeAttribute("id", QString::number(CGCompositionManager::getInstance()->getCurrentCompositionModel()->getId()));
        xmlWriter.writeEndElement();

        xmlWriter.writeStartElement("composition");
        xmlWriter.writeAttribute("id", QString::number(0));
        xmlWriter.writeAttribute("name", "INSTANCE");
        xmlWriter.writeAttribute("viewx", QString::number(CGCompositionManager::getInstance()->getCompositionById(0)->getScene()->getView()->horizontalScrollBar()->value()));
        xmlWriter.writeAttribute("viewy", QString::number(CGCompositionManager::getInstance()->getCompositionById(0)->getScene()->getView()->verticalScrollBar()->value()));
        xmlWriter.writeAttribute("viewscale", QString::number(CGCompositionManager::getInstance()->getCompositionById(0)->getScene()->getView()->getScaleNum()));
        xmlWriter.writeEndElement();
    }else{
        xmlWriter.writeAttribute("id", QString::number(topModel->getId()));
        xmlWriter.writeEndElement();
        xmlWriter.writeStartElement("composition");
        xmlWriter.writeAttribute("id", QString::number(topModel->getId()));
        xmlWriter.writeAttribute("name", fileInfos.baseName());
        xmlWriter.writeAttribute("informations",topModel->getInformation());
        xmlWriter.writeEndElement();
        foreach(CGConnector* connector, topModel->getOuterView()->getConnectorsList()){
            xmlWriter.writeStartElement("connector");
            xmlWriter.writeAttribute("compositionId", QString::number(topModel->getId()));
            xmlWriter.writeAttribute("connectorId", QString::number(connector->getId()));
            xmlWriter.writeAttribute("side", connector->getSide()); //OUTTER SIDE
            CGMultiConnector *cm =
                    qgraphicsitem_cast<CGMultiConnector *>(connector);;
            xmlWriter.writeAttribute("name", cm->getName()); //OUTTER SIDE
            xmlWriter.writeAttribute("x", QString::number(connector->pos().x()));
            xmlWriter.writeAttribute("y", QString::number(connector->pos().y()));
            xmlWriter.writeEndElement();
        }

        foreach(CGOperatorModel* ope, topModel->getOperatorModelList()){
            foreach(CGConnector* connector, ope->getView()->getConnectorsList()){
                xmlWriter.writeStartElement("operatorconnector");
                xmlWriter.writeAttribute("opId", QString::number(ope->getId()));
                xmlWriter.writeAttribute("connectorId", QString::number(connector->getId()));
                xmlWriter.writeAttribute("side", connector->getModel()->getSide());
                xmlWriter.writeAttribute("x", QString::number(connector->pos().x()));
                xmlWriter.writeAttribute("y", QString::number(connector->pos().y()));
                xmlWriter.writeEndElement();
            }
        }
    }
    foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCompositionList(topModel)){
        xmlWriter.writeStartElement("composition");
        xmlWriter.writeAttribute("id", QString::number(model->getId()));
        xmlWriter.writeAttribute("name", model->getName());
        xmlWriter.writeAttribute("x", QString::number(model->getOuterView()->scenePos().x()));
        xmlWriter.writeAttribute("y", QString::number(model->getOuterView()->scenePos().y()));
        xmlWriter.writeAttribute("z", QString::number(model->getOuterView()->zValue()));
        xmlWriter.writeAttribute("informations",model->getInformation());

        xmlWriter.writeAttribute("viewx", QString::number(model->getScene()->getView()->horizontalScrollBar()->value()));
        xmlWriter.writeAttribute("viewy", QString::number(model->getScene()->getView()->verticalScrollBar()->value()));
        xmlWriter.writeAttribute("viewscale", QString::number(model->getScene()->getView()->getScaleNum()));
        if(model->getParentCompositionModel()) xmlWriter.writeAttribute("parentId", QString::number(model->getParentCompositionModel()->getId()));
        xmlWriter.writeEndElement();

        foreach(CGConnector* connector, model->getOuterView()->getConnectorsList()){
            xmlWriter.writeStartElement("connector");
            xmlWriter.writeAttribute("compositionId", QString::number(model->getId()));
            xmlWriter.writeAttribute("connectorId", QString::number(connector->getId()));
            xmlWriter.writeAttribute("side", connector->getSide()); //OUTTER SIDE
            CGMultiConnector *cm =
                    qgraphicsitem_cast<CGMultiConnector *>(connector);;
            xmlWriter.writeAttribute("name", cm->getName()); //OUTTER SIDE
            xmlWriter.writeAttribute("x", QString::number(connector->pos().x()));
            xmlWriter.writeAttribute("y", QString::number(connector->pos().y()));
            xmlWriter.writeEndElement();
        }
    }

    QList<QGraphicsItem*> itemsL2 = topModel->getScene()->items();
    foreach(QGraphicsItem* item, itemsL2){
        if(item->type() == CGLayoutItem::Type){
            CGLayoutItem *ctrl =
                    qgraphicsitem_cast<CGLayoutItem *>(item);
            xmlWriter.writeStartElement("compositioncontrol");
            xmlWriter.writeAttribute("id", QString::number(ctrl->getControl()->getId()));
            xmlWriter.writeAttribute("type", ctrl->getControl()->getKey().c_str());
            xmlWriter.writeAttribute("x", QString::number(ctrl->scenePos().x()));
            xmlWriter.writeAttribute("y", QString::number(ctrl->scenePos().y()));
            xmlWriter.writeAttribute("z", QString::number(ctrl->zValue()));
            xmlWriter.writeAttribute("width", QString::number(ctrl->getRect().width()));
            xmlWriter.writeAttribute("height", QString::number(ctrl->getRect().height()));
            xmlWriter.writeAttribute("parentId", QString::number(topModel->getId()));
            xmlWriter.writeAttribute("content",QString::fromStdString(ctrl->getControl()->toString()));
            xmlWriter.writeEndElement();

            //patch #105
            //prendre le connector out et écrire:
            //<controlconnector controlid="" connectorid="" name="XXX" datatype="XXX" content="XXXXXX"\/>
            foreach(CGLayoutItemConnector* connector, ctrl->getConnectorsL()){
                xmlWriter.writeStartElement("controlconnector");
                xmlWriter.writeAttribute("controlid", QString::number(ctrl->getControl()->getId()));
                xmlWriter.writeAttribute("connectorid", QString::number(connector->getId()));
                xmlWriter.writeAttribute("side", connector->getModel()->getSide());
                xmlWriter.writeAttribute("name", connector->getModel()->getExposedName());
                xmlWriter.writeAttribute("datatype", connector->getModel()->getDataType());
                xmlWriter.writeAttribute("content", ctrl->getControl()->getData());
                xmlWriter.writeEndElement();
            }

        }
        if(item->type() == CGOperator::Type){
            CGOperator *ope =
                    qgraphicsitem_cast<CGOperator *>(item);
            xmlWriter.writeStartElement("operator");
            xmlWriter.writeAttribute("id", QString::number(ope->getModel()->getId()));
            xmlWriter.writeAttribute("type", ope->getModel()->getKey());
            xmlWriter.writeAttribute("x", QString::number(ope->scenePos().x()));
            xmlWriter.writeAttribute("y", QString::number(ope->scenePos().y()));
            xmlWriter.writeAttribute("z", QString::number(ope->zValue()));
            xmlWriter.writeAttribute("parentId", QString::number(topModel->getId()));
            xmlWriter.writeEndElement();
            foreach(CGConnector* connector, ope->getConnectorsList()){
                xmlWriter.writeStartElement("operatorconnector");
                xmlWriter.writeAttribute("opId", QString::number(ope->getModel()->getId()));
                xmlWriter.writeAttribute("connectorId", QString::number(connector->getId()));
                xmlWriter.writeAttribute("side", connector->getModel()->getSide());
                xmlWriter.writeAttribute("x", QString::number(connector->pos().x()));
                xmlWriter.writeAttribute("y", QString::number(connector->pos().y()));
                xmlWriter.writeEndElement();
            }
            foreach(QString key, ope->getActionList()){
                xmlWriter.writeStartElement("operatoraction");
                xmlWriter.writeAttribute("opId", QString::number(ope->getModel()->getId()));
                xmlWriter.writeAttribute("actionKey", key);
                xmlWriter.writeEndElement();
            }
        }
        if(item->type() == CGText::Type){
            CGText *text = qgraphicsitem_cast<CGText *>(item);
            xmlWriter.writeStartElement("text");
            xmlWriter.writeAttribute("x", QString::number(text->scenePos().x()));
            xmlWriter.writeAttribute("y", QString::number(text->scenePos().y()));
            xmlWriter.writeAttribute("z", QString::number(text->zValue()));
            xmlWriter.writeAttribute("parentId", QString::number(topModel->getId()));
            xmlWriter.writeAttribute("color",text->defaultTextColor().name());
            xmlWriter.writeAttribute("police",QString::number(text->font().pointSize()));
            xmlWriter.writeAttribute("font",text->font().family());
            xmlWriter.writeAttribute("italic",QString(text->font().italic()?"true":"false"));
            xmlWriter.writeAttribute("bold",QString(text->font().bold()?"true":"false"));
            xmlWriter.writeAttribute("underline",QString(text->font().underline()?"true":"false"));
            xmlWriter.writeAttribute("content",text->toPlainText());
            xmlWriter.writeEndElement();

        }
        if(item->type() == CGShape::Type){
            CGShape *shape =
                    qgraphicsitem_cast<CGShape *>(item);
            xmlWriter.writeStartElement("shape");
            xmlWriter.writeAttribute("x", QString::number(shape->scenePos().x()));
            xmlWriter.writeAttribute("y", QString::number(shape->scenePos().y()));
            xmlWriter.writeAttribute("z", QString::number(shape->zValue()));
            xmlWriter.writeAttribute("xrect", QString::number(shape->getRect().x()));
            xmlWriter.writeAttribute("yrect", QString::number(shape->getRect().y()));
            xmlWriter.writeAttribute("width", QString::number(shape->getRect().width()));
            xmlWriter.writeAttribute("height", QString::number(shape->getRect().height()));
            xmlWriter.writeAttribute("parentId", QString::number(topModel->getId()));
            xmlWriter.writeAttribute("color",shape->getColor().name());
            xmlWriter.writeEndElement();
        }
    }
    QList<QGraphicsItem*> itemsL33 = topModel->getScene()->items();
    foreach(QGraphicsItem* item, itemsL33){
        if(item->type() == CGLayoutItem::Type){
            CGLayoutItem *ope = qgraphicsitem_cast<CGLayoutItem *>(item);
            foreach(CGConnector* c, ope->getConnectorsL()){
                if(c->isConnected()){
                    xmlWriter.writeStartElement("connection");
                    xmlWriter.writeAttribute("controlId", QString::number(ope->getControl()->getId()));
                    xmlWriter.writeAttribute("operatorIdOut", QString::number(c->getConnectedConnector()->getOperator()->getModel()->getId()));
                    xmlWriter.writeAttribute("controlConnectorIdIn", QString::number(c->getId()));
                    xmlWriter.writeAttribute("operatorConnectorIdOut", QString::number(c->getConnectedConnector()->getId()));
                    xmlWriter.writeEndElement();
                }
            }
        }
    }

    foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCompositionList(topModel)){
        QList<QGraphicsItem*> itemsL = model->getScene()->items();
        foreach(QGraphicsItem* item, itemsL){
            if(item->type() == CGLayoutItem::Type){
                CGLayoutItem *ctrl =
                        qgraphicsitem_cast<CGLayoutItem *>(item);
                xmlWriter.writeStartElement("compositioncontrol");
                xmlWriter.writeAttribute("id", QString::number(ctrl->getControl()->getId()));
                xmlWriter.writeAttribute("type", ctrl->getControl()->getKey().c_str());
                xmlWriter.writeAttribute("x", QString::number(ctrl->scenePos().x()));
                xmlWriter.writeAttribute("y", QString::number(ctrl->scenePos().y()));
                xmlWriter.writeAttribute("z", QString::number(ctrl->zValue()));
                xmlWriter.writeAttribute("width", QString::number(ctrl->getRect().width()));
                xmlWriter.writeAttribute("height", QString::number(ctrl->getRect().height()));
                xmlWriter.writeAttribute("parentId", QString::number(model->getId()));
                xmlWriter.writeAttribute("content",QString::fromStdString(ctrl->getControl()->toString()));
                xmlWriter.writeEndElement();
            }
            if(item->type() == CGOperator::Type){
                CGOperator *ope =
                        qgraphicsitem_cast<CGOperator *>(item);
                xmlWriter.writeStartElement("operator");
                xmlWriter.writeAttribute("id", QString::number(ope->getModel()->getId()));
                xmlWriter.writeAttribute("type", ope->getModel()->getKey());
                xmlWriter.writeAttribute("x", QString::number(ope->scenePos().x()));
                xmlWriter.writeAttribute("y", QString::number(ope->scenePos().y()));
                xmlWriter.writeAttribute("z", QString::number(ope->zValue()));
                xmlWriter.writeAttribute("parentId", QString::number(model->getId()));
                xmlWriter.writeEndElement();
                foreach(CGConnector* connector, ope->getConnectorsList()){
                    xmlWriter.writeStartElement("operatorconnector");
                    xmlWriter.writeAttribute("opId", QString::number(ope->getModel()->getId()));
                    xmlWriter.writeAttribute("connectorId", QString::number(connector->getId()));
                    xmlWriter.writeAttribute("side", connector->getModel()->getSide());
                    xmlWriter.writeAttribute("x", QString::number(connector->pos().x()));
                    xmlWriter.writeAttribute("y", QString::number(connector->pos().y()));
                    xmlWriter.writeEndElement();
                }
                foreach(QString key, ope->getActionList()){
                    xmlWriter.writeStartElement("operatoraction");
                    xmlWriter.writeAttribute("opId", QString::number(ope->getModel()->getId()));
                    xmlWriter.writeAttribute("actionKey", key);
                    xmlWriter.writeEndElement();
                }
            }
            if(item->type() == CGText::Type){
                CGText *text = qgraphicsitem_cast<CGText *>(item);
                xmlWriter.writeStartElement("text");
                xmlWriter.writeAttribute("x", QString::number(text->scenePos().x()));
                xmlWriter.writeAttribute("y", QString::number(text->scenePos().y()));
                xmlWriter.writeAttribute("z", QString::number(text->zValue()));
                xmlWriter.writeAttribute("parentId", QString::number(model->getId()));
                xmlWriter.writeAttribute("color",text->defaultTextColor().name());
                xmlWriter.writeAttribute("police",QString::number(text->font().pointSize()));
                xmlWriter.writeAttribute("font",text->font().family());
                xmlWriter.writeAttribute("italic",QString(text->font().italic()?"true":"false"));
                xmlWriter.writeAttribute("bold",QString(text->font().bold()?"true":"false"));
                xmlWriter.writeAttribute("underline",QString(text->font().underline()?"true":"false"));
                xmlWriter.writeAttribute("content",text->toPlainText());
                xmlWriter.writeEndElement();

            }
            if(item->type() == CGShape::Type){
                CGShape *shape =
                        qgraphicsitem_cast<CGShape *>(item);
                xmlWriter.writeStartElement("shape");
                xmlWriter.writeAttribute("x", QString::number(shape->scenePos().x()));
                xmlWriter.writeAttribute("y", QString::number(shape->scenePos().y()));
                xmlWriter.writeAttribute("z", QString::number(shape->zValue()));
                xmlWriter.writeAttribute("width", QString::number(shape->getRect().width()));
                xmlWriter.writeAttribute("height", QString::number(shape->getRect().height()));
                xmlWriter.writeAttribute("parentId", QString::number(model->getId()));
                xmlWriter.writeAttribute("color",shape->getColor().name());
                xmlWriter.writeEndElement();
            }
        }
    }

    foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCompositionList(topModel)){
        QList<QGraphicsItem*> itemsL = model->getScene()->items();
        foreach(QGraphicsItem* item, itemsL){
            if(item->type() == CGLayoutItem::Type){
                CGLayoutItem *ope = qgraphicsitem_cast<CGLayoutItem *>(item);
                foreach(CGConnector* c, ope->getConnectorsL()){
                    if(c->isConnected()){
                        xmlWriter.writeStartElement("connection");
                        xmlWriter.writeAttribute("controlId", QString::number(ope->getControl()->getId()));
                        xmlWriter.writeAttribute("operatorIdOut", QString::number(c->getConnectedConnector()->getOperator()->getModel()->getId()));
                        xmlWriter.writeAttribute("controlConnectorIdIn", QString::number(c->getId()));
                        xmlWriter.writeAttribute("operatorConnectorIdOut", QString::number(c->getConnectedConnector()->getId()));
                        xmlWriter.writeEndElement();
                    }
                }
            }
        }
    }
    QList<QGraphicsItem*> itemsL3 = topModel->getScene()->items();
    foreach(QGraphicsItem* item, itemsL3){
        if(item->type() == CGOperator::Type){
            CGOperator *ope = qgraphicsitem_cast<CGOperator *>(item);

            foreach(CGConnector* c, ope->getConnectorsList()){
                if(c->isConnected() && !c->getConnectedConnector()->getIsControlConnected()){
                    if(c->getConnectedConnector()->type() != CGMultiConnector::Type){
                        xmlWriter.writeStartElement("connection");
                        xmlWriter.writeAttribute("connectorId1", QString::number(c->getId()));
                        xmlWriter.writeAttribute("operatorId1", QString::number(ope->getModel()->getId()));
                        xmlWriter.writeAttribute("connectorId2", QString::number(c->getConnectedConnector()->getId()));
                        xmlWriter.writeAttribute("operatorId2", QString::number(c->getConnectedConnector()->getOperator()->getModel()->getId()));
                        xmlWriter.writeEndElement();
                    }else{
                        //connection operator/composition
                        CGMultiConnector *mc = qgraphicsitem_cast<CGMultiConnector *>(c->getConnectedConnector());
                        CGMulti *m = qgraphicsitem_cast<CGMulti *>(c->getConnectedConnector()->parentItem());

                        xmlWriter.writeStartElement("connection");
                        xmlWriter.writeAttribute("operatorId1", QString::number(ope->getModel()->getId()));
                        xmlWriter.writeAttribute("compositionId2", QString::number(m->getModel()->getId()));
                        xmlWriter.writeAttribute("connectorId1",  QString::number(c->getId()));
                        xmlWriter.writeAttribute("connectorId2", QString::number(mc->getId()));
                        xmlWriter.writeEndElement();
                    }
                }
            }

        }
        if(item->type() == CGMulti::Type){
            CGMulti *ope = qgraphicsitem_cast<CGMulti *>(item);

            foreach(CGConnector* c, ope->getConnectorsList()){
                if(c->isConnected()){
                    if(c->getConnectedConnector()->type() == CGMultiConnector::Type){
                        CGMultiConnector *mc = qgraphicsitem_cast<CGMultiConnector *>(c->getConnectedConnector());
                        CGMulti *m = qgraphicsitem_cast<CGMulti *>(c->getConnectedConnector()->parentItem());
                        xmlWriter.writeStartElement("connection");
                        xmlWriter.writeAttribute("compositionId1", QString::number(ope->getModel()->getId()));
                        xmlWriter.writeAttribute("compositionId2", QString::number(m->getModel()->getId()));
                        xmlWriter.writeAttribute("connectorId1", QString::number(c->getId()));
                        xmlWriter.writeAttribute("connectorId2", QString::number(mc->getId()));
                        xmlWriter.writeEndElement();
                    }else{
                        //connection operator/composition
                        xmlWriter.writeStartElement("connection");
                        xmlWriter.writeAttribute("compositionId1", QString::number(ope->getModel()->getId()));
                        xmlWriter.writeAttribute("operatorId2", QString::number(c->getConnectedConnector()->getOperator()->getModel()->getId()));
                        xmlWriter.writeAttribute("connectorId1", QString::number(c->getId()));
                        xmlWriter.writeAttribute("connectorId2", QString::number(c->getConnectedConnector()->getId()));
                        xmlWriter.writeEndElement();
                    }
                }
            }
        }
    }
    foreach(CGCompositionModel* model, CGCompositionManager::getInstance()->getCompositionList(topModel)){
        QList<QGraphicsItem*> itemsL = model->getScene()->items();
        foreach(QGraphicsItem* item, itemsL){
            if(item->type() == CGOperator::Type){
                CGOperator *ope = qgraphicsitem_cast<CGOperator *>(item);

                foreach(CGConnector* c, ope->getConnectorsList()){
                    if(c->isConnected()){
                        if(c->getConnectedConnector()->type() != CGLayoutItemConnector::Type ){
                            if(c->getConnectedConnector()->type() != CGMultiConnector::Type){
                                xmlWriter.writeStartElement("connection");
                                xmlWriter.writeAttribute("connectorId1", QString::number(c->getId()));
                                xmlWriter.writeAttribute("operatorId1", QString::number(ope->getModel()->getId()));
                                xmlWriter.writeAttribute("connectorId2", QString::number(c->getConnectedConnector()->getId()));
                                xmlWriter.writeAttribute("operatorId2", QString::number(c->getConnectedConnector()->getOperator()->getModel()->getId()));
                                xmlWriter.writeEndElement();
                            }else{
                                //connection operator/composition
                                CGMultiConnector *mc = qgraphicsitem_cast<CGMultiConnector *>(c->getConnectedConnector());
                                CGMulti *m = qgraphicsitem_cast<CGMulti *>(c->getConnectedConnector()->parentItem());

                                xmlWriter.writeStartElement("connection");
                                xmlWriter.writeAttribute("operatorId1", QString::number(ope->getModel()->getId()));
                                xmlWriter.writeAttribute("compositionId2", QString::number(m->getModel()->getId()));
                                xmlWriter.writeAttribute("connectorId1",  QString::number(c->getId()));
                                xmlWriter.writeAttribute("connectorId2", QString::number(mc->getId()));
                                xmlWriter.writeEndElement();
                            }
                        }
                    }
                }



            }
            if(item->type() == CGMulti::Type){
                CGMulti *ope = qgraphicsitem_cast<CGMulti *>(item);

                foreach(CGConnector* c, ope->getConnectorsList()){
                    if(c->isConnected()){
                        if(c->getConnectedConnector()->type() == CGMultiConnector::Type){
                            CGMultiConnector *mc = qgraphicsitem_cast<CGMultiConnector *>(c->getConnectedConnector());
                            CGMulti *m = qgraphicsitem_cast<CGMulti *>(c->getConnectedConnector()->parentItem());
                            xmlWriter.writeStartElement("connection");
                            xmlWriter.writeAttribute("compositionId1", QString::number(ope->getModel()->getId()));
                            xmlWriter.writeAttribute("compositionId2", QString::number(m->getModel()->getId()));
                            xmlWriter.writeAttribute("connectorId1", QString::number(c->getId()));
                            xmlWriter.writeAttribute("connectorId2", QString::number(mc->getId()));
                            xmlWriter.writeEndElement();
                        }else{
                            //connection operator/composition
                            xmlWriter.writeStartElement("connection");
                            xmlWriter.writeAttribute("compositionId1", QString::number(ope->getModel()->getId()));
                            xmlWriter.writeAttribute("operatorId2", QString::number(c->getConnectedConnector()->getOperator()->getModel()->getId()));
                            xmlWriter.writeAttribute("connectorId1", QString::number(c->getId()));
                            xmlWriter.writeAttribute("connectorId2", QString::number(c->getConnectedConnector()->getModel()->getId()));
                            xmlWriter.writeEndElement();
                        }
                    }
                }
            }
        }
    }

    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    xmlWriter.writeEndElement();
    ////qDebug << "==========================";
    file.close();

    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Save project "+savedPath+" ...");
    copyFile(path,savedPath);
    CGProject::getInstance()->setPath(savedPath);

    //create dirs
    //reroots logs
}

bool CGProjectSerialization::copyFile(QString oldFilePath, QString newFilePath)
{
    //same file, no need to copy
    if(oldFilePath.compare(newFilePath) == 0)
        return true;

    //load both files
    QFile oldFile(oldFilePath);
    QFile newFile(newFilePath);
    bool openOld = oldFile.open( QIODevice::ReadOnly );
    bool openNew = newFile.open( QIODevice::WriteOnly );

    //if either file fails to open bail
    if(!openOld || !openNew) { return false; }

    //copy contents
    uint BUFFER_SIZE = 16000;
    char* buffer = new char[BUFFER_SIZE];
    while(!oldFile.atEnd())
    {
        qint64 len = oldFile.read( buffer, BUFFER_SIZE );
        newFile.write( buffer, len );
    }

    //deallocate buffer
    delete[] buffer;
    buffer = NULL;
    return true;
}

CGCompositionModel* CGProjectSerialization::compositionFromFile(QString path, CGCompositionModel* parentModel){
    QFile file(path);
    QFileInfo fileInfos(file);
    if (!fileInfos.isFile() || !file.open(QFile::ReadOnly | QFile::Text)) {
        return 0;
    }

    QMap<int,int> mapOldNewControl;
    QMap<int,int> mapOldNewOperator;
    QMap<int,int> mapOldNewLayout;
    QMap<int,CGCompositionModel*> mapOldNewComposition;

    int currentC = -1;
    CGCompositionModel* patternModel = NULL;

    QXmlStreamReader xmlReader;
    xmlReader.setDevice(&file);
    bool isInterface = false;
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
                    if (xmlReader.name() == "mode"){
                        QString iss = xmlReader.attributes().value("isInterface").toString();
                        if(iss.compare("true") == 0){
                            isInterface = false;
                        }else{
                            isInterface = false;
                        }
                    }

                    if (xmlReader.name() == "currentComposition"){
                        currentC = xmlReader.attributes().value("id").toString().toInt();
                    }

                    if (xmlReader.name() == "layout"){
                        QString name = xmlReader.attributes().value("name").toString();
                        QString type = xmlReader.attributes().value("type").toString();
                        QString resolution = xmlReader.attributes().value("resolution").toString();
                        QString popup = xmlReader.attributes().value("resolution").toString();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        ////qDebug << "rebuild layout name: " <<name;

                        //check id
                        if(id == 0 && !isInterface){
                            mapOldNewLayout.insert(id,CGLayoutManager::getInstance()->getCurrentLayout()->getId());
//                            CGLayoutManager::getInstance()->getCurrentLayout()->setResolution(resolution);

                            if(popup.compare("true")==0)CGLayoutManager::getInstance()->getCurrentLayout()->setIsPopup(true);
                            if(popup.compare("false")==0)CGLayoutManager::getInstance()->getCurrentLayout()->setIsPopup(false);
                        }else{
                            CGLayout* layout = CGLayoutManager::getInstance()->getLayout();
                            layout->setName(type+" "+fileInfos.baseName());
                            layout->setType(type);
                            layout->setResolution(resolution);

                            if(popup.compare("true")==0)layout->setIsPopup(true);
                            if(popup.compare("false")==0)layout->setIsPopup(false);
                            CGProject::getInstance()->addLayout(layout);
                            mapOldNewLayout.insert(id,layout->getId());
                        }
                    }
                    if (xmlReader.name() == "composition"){
                        if(!xmlReader.attributes().value("parentId").isNull()){
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            ////qDebug << "rebuild composition name: " <<name;
                            CGCompositionModel* compositionModel = new CGCompositionModel();
                            QString informations = xmlReader.attributes().value("informations").toString();
                            compositionModel->setInformation(informations);
                            mapOldNewComposition.insert(id,compositionModel);
                        }else{
                            //nothing to be done
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            CGCompositionModel* compositionModel = new CGCompositionModel();
                            mapOldNewComposition.insert(id,compositionModel);

                        }
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
                        if(!xmlReader.attributes().value("parentId").isNull()){
                            int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            int x = xmlReader.attributes().value("x").toString().toDouble();
                            int y = xmlReader.attributes().value("y").toString().toDouble();
                            int z = xmlReader.attributes().value("z").toString().toDouble();
                            ////qDebug << "rebuild composition name: " <<name;
                            CGCompositionModel* child = mapOldNewComposition.value(id);

                            QString informations = xmlReader.attributes().value("informations").toString();
                            child->setInformation(informations);

                            CGCompositionModel* par = mapOldNewComposition.value(parentId);
                            child->setParentCompositionModel(par);
                            child->initialise();
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(child->getOuterView());

                            CGMulti* multi = child->getOuterView();
                            child->setName(name);
                            multi->setPos(x,y);
                            multi->setIsCompatibleMode(false);
                            multi->setSelected(false);
                            multi->setSelected(false);
                            multi->setZValue(z);
                            if(child->getId() == id){
                                CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good composition id!");
                            }else{
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad composition id! "+QString::number(id)+" "+QString::number(child->getId()));
                            }
                            CGProject::getInstance()->addComposition(child);

                        }else{
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("name").toString();
                            int x = xmlReader.attributes().value("x").toString().toDouble();
                            int y = xmlReader.attributes().value("y").toString().toDouble();
                            ////qDebug << "rebuild composition name: " <<name;
                            CGCompositionModel* child = mapOldNewComposition.value(id);

                            QString informations = xmlReader.attributes().value("informations").toString();
                            child->setInformation(informations);

                            patternModel = child;
                            child->setParentCompositionModel(parentModel);
                            child->initialise();
                            parentModel->getScene()->unselectAll();
                            parentModel->getScene()->getModel()->activate(child->getOuterView());

                            CGMulti* multi = child->getOuterView();
                            child->setName(name);
                            multi->setPos(x,y);
                            multi->setIsCompatibleMode(false);
                            multi->setSelected(false);
                            multi->setSelected(false);
                            CGProject::getInstance()->addComposition(child);
                        }

                    }
                }
            }
        }
    }
    file.reset();
    xmlReader.clear ();
    xmlReader.setDevice(&file);

    QList<CGMultiConnector*> multiConnectorList;

    QGraphicsItemGroup* group = new QGraphicsItemGroup(0,CGLayoutManager::getInstance()->getCurrentLayout());

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
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                        CGCompositionModel* par = mapOldNewComposition.value(parentId);
                        CGOperatorModel* model = par->addOperatorByName(name);
                        CGOperator* view = model->getView();
                        model->getView()->setPos(x,y);
                        model->setPos(model->getView()->scenePos());
                        par->getScene()->unselectAll();
                        par->getScene()->getModel()->activate(view);
                        model->getView()->setZValue(z);
                        ////qDebug << "[check]rebuild operator type: " << name << "" << model->getId();
                        CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load operator named "+name);
                        CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load operator with position "+QString::number(x)+","+QString::number(y));
                        //                        if(model->getId() == id){
                        //                            CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good operator id!");
                        //                        }else{
                        //                            CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad operator id!");
                        //                        }
                        mapOldNewOperator.insert(id,model->getId());

                    }

                    if (xmlReader.name() == "control"){
                        int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                        int id = xmlReader.attributes().value("id").toString().toInt();
                        QString name = xmlReader.attributes().value("type").toString();
                        QString content = xmlReader.attributes().value("content").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        int width = xmlReader.attributes().value("width").toString().toDouble();
                        int height = xmlReader.attributes().value("height").toString().toDouble();
                        ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                        QRect rect(0,0,width,height);
                        CGLayout* par = CGLayoutManager::getInstance()->getLayout(mapOldNewLayout.value(parentId));
                        CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name,par);
                        layoutItem->setSelected(false);
                        layoutItem->setPos(x,y);
                        //                        layoutItem->resize(rect.bottomRight());
                        //                        layoutItem->setRect(rect);
                        par->activate(layoutItem);
                        layoutItem->setZValue(z);
                        layoutItem->resize(rect);
                        ////qDebug << "[check]rebuild layoutItem type: " << name << "" << layoutItem->getControl()->getId();
                        //                        if(layoutItem->getControl()->getId() == id){
                        //                            CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layoutItem id!");
                        //                        }else{
                        //                            CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layoutItem id!");
                        //                        }

                        CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Load control data before connection");
                        layoutItem->getControl()->fromString(content.toStdString());
                        if(parentId == 0 && !isInterface) group->addToGroup(layoutItem);
                        mapOldNewControl.insert(id,layoutItem->getControl()->getId());
                    }

                    if (xmlReader.name() == "shape"){
                        ////qDebug << "rebuild shape" <<CGProject::getInstance()->getName();
                        int parentid = xmlReader.attributes().value("parentId").toString().toInt();
                        QString color = xmlReader.attributes().value("color").toString();
                        int xrect = xmlReader.attributes().value("xrect").toString().toDouble();
                        int yrect = xmlReader.attributes().value("yrect").toString().toDouble();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        int width = xmlReader.attributes().value("width").toString().toDouble();
                        int height = xmlReader.attributes().value("height").toString().toDouble();
                        QString ispers = xmlReader.attributes().value("persistent").toString();
                        QRect rect(xrect,yrect,width,height);
                        if(!xmlReader.attributes().value("fromLayout").isNull()){
                            parentid = mapOldNewLayout.value(parentid);
                            CGLayout* lay = CGLayoutManager::getInstance()->getLayout(parentid);

                            CGShape* shape = new CGShape(0,0,lay);
                            lay->addItem(shape);
                            QColor c;
                            c.setNamedColor(color);
                            shape->setColor(color);
                            shape->setRect(rect);
                            shape->setPos(x,y);
                            shape->setZValue(z);
                            if(ispers.compare("true") == 0){
                                shape->setPersistent(true);
                            }else{
                                shape->setPersistent(false);
                            }
                            if(parentid == 0 && !isInterface) group->addToGroup(shape);
                        }else{
                            CGCompositionModel* par = mapOldNewComposition.value(parentid);
                            //                            CGCompositionModel* par = CGCompositionManager::getInstance()->getCompositionById(parentid);
                            CGShape* shape = new CGShape(0,0,par->getScene());
                            par->getScene()->addItem(shape);
                            shape->setRect(rect);
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(shape);
                            QColor c;
                            c.setNamedColor(color);
                            shape->setPos(x,y);
                            shape->setColor(color);
                            shape->setZValue(z);
                        }
                    }

                    if (xmlReader.name() == "text"){
                        ////qDebug << "rebuild text";
                        int parentid = xmlReader.attributes().value("parentId").toString().toInt();
                        QString content = xmlReader.attributes().value("content").toString();
                        QString color = xmlReader.attributes().value("color").toString();
                        int police = xmlReader.attributes().value("police").toString().toInt();
                        QString font = xmlReader.attributes().value("font").toString();
                        int x = xmlReader.attributes().value("x").toString().toDouble();
                        int y = xmlReader.attributes().value("y").toString().toDouble();
                        int z = xmlReader.attributes().value("z").toString().toDouble();
                        QString italic = xmlReader.attributes().value("italic").toString();
                        QString bold = xmlReader.attributes().value("bold").toString();
                        QString underline = xmlReader.attributes().value("underline").toString();

                        if(!xmlReader.attributes().value("fromLayout").isNull()){
                            CGText* text = new CGText();
                            parentid = mapOldNewLayout.value(parentid);
                            CGLayout* lay = CGLayoutManager::getInstance()->getLayout(parentid);
                            //                            text->setFont(layout->font());
                            text->setTextInteractionFlags(Qt::TextEditorInteraction);
                            text->setZValue(1000.0);

                            QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                             lay, SLOT(editorLostFocus(CGText*)));
                            QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                             lay, SIGNAL(itemSelected(QGraphicsItem*)));
                            lay->addItem(text);
                            text->setPos(x,y);
                            text->setZValue(z);
                            text->setPlainText(content);

                            QColor c;
                            c.setNamedColor(color);
                            text->setDefaultTextColor(c);
                            QFont f;
                            f.setFamily(font);
                            f.setPointSize(police);

                            if(italic.compare("true") == 0){
                                f.setItalic(true);
                            }
                            if(bold.compare("true") == 0){
                                f.setBold(true);
                            }
                            if(underline.compare("true") == 0){
                                f.setUnderline(true);
                            }
                            text->setFont(f);

                            QString ispers = xmlReader.attributes().value("persistent").toString();
                            if(ispers.compare("true") == 0){
                                text->setPersistent(true);
                            }else{
                                text->setPersistent(false);
                            }
                            if(parentid == 0 && !isInterface) group->addToGroup(text);
                        }else{
                            CGText* text = new CGText();
                            CGCompositionModel* par = mapOldNewComposition.value(parentid);
                            //                            CGCompositionModel* par = CGCompositionManager::getInstance()->getCompositionById(parentid);
                            text->setFont(par->getScene()->font());
                            text->setTextInteractionFlags(Qt::TextEditorInteraction);
                            text->setZValue(10000.0);

                            QObject::connect(text, SIGNAL(lostFocus(CGText*)),
                                             par->getScene(), SLOT(editorLostFocus(CGText*)));
                            QObject::connect(text, SIGNAL(selectedChange(QGraphicsItem*)),
                                             par->getScene(), SIGNAL(itemSelected(QGraphicsItem*)));
                            par->getScene()->addItem(text);

                            text->setDefaultTextColor(Qt::white);
                            text->setPos(x,y);
                            text->setZValue(z);
                            par->getScene()->unselectAll();
                            par->getScene()->getModel()->activate(text);
                            text->setPlainText(content);

                            QColor c;
                            c.setNamedColor(color);
                            text->setDefaultTextColor(c);

                            QFont f;
                            f.setFamily(font);
                            f.setPointSize(police);

                            if(italic.compare("true") == 0){
                                f.setItalic(true);
                            }
                            if(bold.compare("true") == 0){
                                f.setBold(true);
                            }
                            if(underline.compare("true") == 0){
                                f.setUnderline(true);
                            }
                            text->setFont(f);
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
                        if (xmlReader.name() == "operatoraction"){
                            int id = xmlReader.attributes().value("opId").toString().toInt();
                            QString actionKey = xmlReader.attributes().value("actionKey").toString();
                            int newId = mapOldNewOperator.value(id);
                            CGOperatorModel* ope = CGCompositionManager::getInstance()->searchOperatorById(newId);
                            if(ope!=0){
                                ope->getView()->saveAction(actionKey);
                                ope->setWaiting(true);
                                CClientSingleton::getInstance()->getCInterfaceClient2Server()->actionOperator(ope->getId(), actionKey.toStdString());
                            }
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
                    if (xmlReader.name() == "connector"){
                        int compositionId = xmlReader.attributes().value("compositionId").toString().toInt();
                        QString side = xmlReader.attributes().value("side").toString();
                        if(xmlReader.attributes().value("x").isNull()){
                            CGCompositionModel* model = mapOldNewComposition.value(compositionId);
                            QString name = xmlReader.attributes().value("name").toString();
                            if(side.compare("IN") == 0){
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("OUTIN");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                                model->getInnerView()->processAllConnectorsPos();
                                model->getOuterView()->processAllConnectorsPos();
                            }
                            if(side.compare("OUT") == 0) {
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("INOUT");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                                model->getInnerView()->processAllConnectorsPos();
                                model->getOuterView()->processAllConnectorsPos();
                            }
                        }else{
                            double x = xmlReader.attributes().value("x").toString().toDouble();
                            double y = xmlReader.attributes().value("y").toString().toDouble();

                            CGCompositionModel* model = mapOldNewComposition.value(compositionId);
                            QString name = xmlReader.attributes().value("name").toString();
                            if(side.compare("IN") == 0){
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("OUTIN");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();
                                QPointF p(x,y);
                                m->getOuterConnector()->setPos(p);
                                multiConnectorList.append(m);
                                //                                double r = (m->getOuterConnector()->getRayon())/2;
                                //                                double teta = acos(m->getOuterConnector()->pos().x()/r)*(SCD::sgn(m->getOuterConnector()->pos().y()));

                                //                                double x1 = cos(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
                                //                                double y1 = sin(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
                                //                                m->getInnerConnector()->setPos(x1,y1);
                            }
                            if(side.compare("OUT") == 0) {
                                CGMultiConnector* m = model->getInnerView()->getParentView()->addConnector("INOUT");
                                m->getOuterConnector()->setName(name);
                                m->getOuterConnector()->toolT();

                                QPointF p(x,y);
                                m->getOuterConnector()->setPos(p);
                                multiConnectorList.append(m);
                                //                                double r = (m->getOuterConnector()->getRayon())/2;
                                //                                double teta = acos(m->getOuterConnector()->pos().x()/r)*(SCD::sgn(m->getOuterConnector()->pos().y()));

                                //                                double x1 = cos(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
                                //                                double y1 = sin(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
                                //                                m->getInnerConnector()->setPos(x1,y1);
                            }
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
                        if (xmlReader.name() == "compositioncontrol"){
                            int parentId = xmlReader.attributes().value("parentId").toString().toInt();
                            int id = xmlReader.attributes().value("id").toString().toInt();
                            QString name = xmlReader.attributes().value("type").toString();
                            QString content = xmlReader.attributes().value("content").toString();
                            int x = xmlReader.attributes().value("x").toString().toDouble();
                            int y = xmlReader.attributes().value("y").toString().toDouble();
                            int z = xmlReader.attributes().value("z").toString().toDouble();
                            int width = xmlReader.attributes().value("width").toString().toDouble();
                            int height = xmlReader.attributes().value("height").toString().toDouble();
                            QString ispers = xmlReader.attributes().value("persistent").toString();

                            ////qDebug << "[Ref.]rebuild operator type: " << name << "" << id;
                            QRect rect(0,0,width,height);
                            CGCompositionModel* compM = mapOldNewComposition.value(parentId);
                            CGComposition* comp = compM->getScene();
                            CGLayoutItem* layoutItem = CGLayoutFactory::getInstance()->getLayoutItemByName(name,comp);
                            if(layoutItem!=NULL && layoutItem->getControl()!=NULL){
                                layoutItem->setSelected(false);
                                layoutItem->setPos(x,y);
                                if(ispers.compare("true") == 0){
                                    layoutItem->setIsPersistant(true);
                                }else{
                                    layoutItem->setIsPersistant(false);
                                }

                                //                        layoutItem->resize(rect.bottomRight());
                                //                        layoutItem->setRect(rect);
                                layoutItem->setZValue(z);
                                compM->activate(layoutItem);
                                layoutItem->resize(rect);
                                ////qDebug << "[check]rebuild layoutItem type: " << name << "" << layoutItem->getControl()->getId();
                                //                                if(layoutItem->getControl()->getId() == id){
                                //                                    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"Good layoutItem id!");
                                //                                }else{
                                //                                    CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Bad layoutItem id!");
                                //                                }

                                layoutItem->getControl()->fromString(content.toStdString());
                                mapOldNewControl.insert(id,layoutItem->getControl()->getId());
                            }else{
                                CLogger::getInstance()->log("INSTANCE",CLogger::ERROR,"Can't create control "+name);
                                mapOldNewControl.insert(id,-1);
                            }
                        }
                    }
                }
            }
        }

        file.reset();
        xmlReader.clear ();
        xmlReader.setDevice(&file);

        foreach(CGMultiConnector*m,multiConnectorList){
            double r = (m->getOuterConnector()->getRayon())/2;
            double teta = acos(m->getOuterConnector()->pos().x()/r)*(SCD::sgn(m->getOuterConnector()->pos().y()));
            m->getOuterConnector()->rePosition();

            double x1 = cos(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
            double y1 = sin(teta)*((m->getInnerConnector()->getRayon()-20)+20)/2;
            m->getInnerConnector()->setPos(x1,y1);
        }

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
                        if (xmlReader.name() == "connection"){
                            if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull() ){
                                int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                                int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                                int operatorId1 = xmlReader.attributes().value("operatorId1").toString().toInt();
                                int operatorId2 = xmlReader.attributes().value("operatorId2").toString().toInt();
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId1));
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId2));
                                ope1->getView()->getConnectorById(connectorId1)->connect(ope2->getView()->getConnectorById(connectorId2));
                            }
                            if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("operatorId2").isNull()){
                                int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                                int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                                int compositionId1 = xmlReader.attributes().value("compositionId1").toString().toInt();
                                int operatorId2 = xmlReader.attributes().value("operatorId2").toString().toInt();
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId2));
                                CGCompositionModel* model = mapOldNewComposition.value(compositionId1);
                                CGConnector* c2 = ope2->getView()->getConnectorById(connectorId2);
                                CGConnector* cm;
                                if(ope2->getView()->scene() == model->getOuterView()->scene()){
                                    cm = model->getOuterView()->getConnectorById(connectorId1);
                                }else{
                                    cm = model->getInnerView()->getConnectorById(connectorId1);
                                }
                                c2->connect(cm);
                            }
                            if(!xmlReader.attributes().value("operatorId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                                int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                                int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                                int operatorId1 = xmlReader.attributes().value("operatorId1").toString().toInt();
                                int compositionId2 = xmlReader.attributes().value("compositionId2").toString().toInt();
                                CGOperatorModel* ope2 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId1));
                                CGCompositionModel* model = mapOldNewComposition.value(compositionId2);

                                CGConnector* c2 = ope2->getView()->getConnectorById(connectorId1);
                                CGConnector* cm;
                                if(ope2->getView()->scene() == model->getOuterView()->scene()){
                                    cm = model->getOuterView()->getConnectorById(connectorId2);
                                }else{
                                    cm = model->getInnerView()->getConnectorById(connectorId2);
                                }
                                c2->connect(cm);
                            }
                            if(!xmlReader.attributes().value("compositionId1").isNull() && !xmlReader.attributes().value("compositionId2").isNull()){
                                int connectorId1 = xmlReader.attributes().value("connectorId1").toString().toInt();
                                int connectorId2 = xmlReader.attributes().value("connectorId2").toString().toInt();
                                int compositionId1 = xmlReader.attributes().value("compositionId1").toString().toInt();
                                int compositionId2 = xmlReader.attributes().value("compositionId2").toString().toInt();
                                CGCompositionModel* model1 = mapOldNewComposition.value(compositionId1);
                                CGCompositionModel* model2 = mapOldNewComposition.value(compositionId2);
                                if(model1->getParentCompositionModel() == model2){
                                    model1->getOuterView()->getConnectorById(connectorId1)->connect(model2->getInnerView()->getConnectorById(connectorId2));
                                }
                                if(model1 == model2->getParentCompositionModel()){
                                    model1->getInnerView()->getConnectorById(connectorId1)->connect(model2->getOuterView()->getConnectorById(connectorId2));
                                }
                                if(model1->getParentCompositionModel() == model2->getParentCompositionModel()){
                                    model1->getOuterView()->getConnectorById(connectorId1)->connect(model2->getOuterView()->getConnectorById(connectorId2));
                                }
                            }
                            if(!xmlReader.attributes().value("controlConnectorIdIn").isNull() && !xmlReader.attributes().value("operatorConnectorIdOut").isNull()){
                                int controlId = xmlReader.attributes().value("controlId").toString().toInt();
                                int operatorIdOut = xmlReader.attributes().value("operatorIdOut").toString().toInt();
                                int controlConnectorIdIn = xmlReader.attributes().value("controlConnectorIdIn").toString().toInt();
                                int operatorConnectorIdOut = xmlReader.attributes().value("operatorConnectorIdOut").toString().toInt();
                                CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorIdOut));
                                CGLayoutItem* layoutItem = CGLayoutManager::getInstance()->getItemById(mapOldNewControl.value(controlId));
                                //TODO: this control doesn't have to be there. Miss design here !

                                if(ope1 != NULL && layoutItem!=NULL){
                                    if(ope1->getView()->getConnectorById(operatorConnectorIdOut)->getSide().compare(layoutItem->getConnectorById(controlConnectorIdIn)->getSide()) != 0){
                                        if(ope1->getView()->getConnectorById(operatorConnectorIdOut)!=0
                                                &&layoutItem->getConnectorById(controlConnectorIdIn)!=0){
                                            layoutItem->getConnectorById(controlConnectorIdIn)->setIsControlConnected(true);
                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->setIsControlConnected(true);

                                            ope1->getView()->getConnectorById(operatorConnectorIdOut)->connect(layoutItem->getConnectorById(controlConnectorIdIn));
                                            layoutItem->getConnectorById(controlConnectorIdIn)->connect(ope1->getView()->getConnectorById(operatorConnectorIdOut));
                                        }
                                    }
                                    layoutItem->getConnectorById(controlConnectorIdIn)->update();
                                    ope1->getView()->getConnectorById(operatorConnectorIdOut)->update();
                                }
                            }

                        }
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
                        int id = xmlReader.attributes().value("connectorId").toString().toInt();
                        int operatorId = xmlReader.attributes().value("opId").toString().toInt();
                        double x = xmlReader.attributes().value("x").toString().toDouble();
                        double y = xmlReader.attributes().value("y").toString().toDouble();
                        CGOperatorModel* ope1 = CGCompositionManager::getInstance()->searchOperatorById(mapOldNewOperator.value(operatorId));
                        CGConnector* c = ope1->getView()->getConnectorById(id);
                        QPointF p(x,y);
                        c->setPos(p);
                    }
                }
            }
        }
    }

    if(!isInterface){
        QRectF r = CGLayoutManager::getInstance()->getCurrentLayout()->getView()->sceneRect();
        int x = r.x()+r.width()/2;
        int y = r.y()+r.height()/2;
        group->setPos(x,y);
        QList<QGraphicsItem*> il = group->childItems();
        CGLayoutManager::getInstance()->getCurrentLayout()->destroyItemGroup(group);
        foreach(QGraphicsItem*item, il){
            item->setSelected(true);
            CGLayoutManager::getInstance()->getController()->toFront();
        }
    }

    return patternModel;
}

void CGProjectSerialization::createProject(QString path){
    QFileInfo info(path);
    CGProject::getInstance()->setLogPath(info.absoluteDir().path()+"/log/");
    if(CGProject::getInstance()->getName().compare("")==0) CGProject::getInstance()->setName(info.baseName());
    if(CGProject::getInstance()->getVersion().compare("")==0) CGProject::getInstance()->setVersion("0.0.0-SNAPSHOT");
    CGProject::getInstance()->setPath(info.absoluteDir().path());
    CGProject::getInstance()->setTmpPath(info.absoluteDir().path()+"/tmp/");
    CGProject::getInstance()->setRessourcePath(info.absoluteDir().path()+"/ressource/");
    CGProject::getInstance()->setTargetPath(info.absoluteDir().path()+"/target/");

    //    if(!info.absoluteDir().exists()){
    //        info.absoluteDir().mkpath(info.absoluteDir().path());
    //    }

    QFileInfo info2(CGProject::getInstance()->getTmpPath());
    if(!info2.absoluteDir().exists()){
        info2.absoluteDir().mkpath(CGProject::getInstance()->getTmpPath());
    }
    QFileInfo info3(CGProject::getInstance()->getRessourcePath());
    if(!info3.absoluteDir().exists()){
        info3.absoluteDir().mkpath(CGProject::getInstance()->getRessourcePath());
    }
    QFileInfo info4(CGProject::getInstance()->getTargetPath());
    if(!info4.absoluteDir().exists()){
        info4.absoluteDir().mkpath(CGProject::getInstance()->getTargetPath());
    }
    QFileInfo info5(CGProject::getInstance()->getLogPath());
    if(!info5.absoluteDir().exists()){
        info5.absoluteDir().mkpath(CGProject::getInstance()->getLogPath());
    }
}

