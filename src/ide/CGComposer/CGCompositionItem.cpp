/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCompositionItem.h"

#include <QPainter>
#include "CGCompositionManager.h"
#include "CGCompositionFactory.h"
#include "CGLayoutManager.h"
#include "../CGProject/CGProject.h"
#include "../CGCommand/CGChangeView.h"
#include "../CGCommand/CGDisconnectConnector.h"
#include "../CGCommand/CGConnectConnector.h"
#include "CLogger.h"

#include<CUtilitySTL.h>
CGOperator::CGOperator(QMenu *operatorMenu, QMenu *connectorMenu,
                       QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    this->operatorMenu = operatorMenu;
    this->connectorMenu = connectorMenu;

    innerColor = Qt::lightGray;
    outerColor = Qt::gray;
    textColor = Qt::white;
    doc = false;

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    model = NULL;

    nbConnectors = 0;
    rayon = 120;
    errormsg = "";

    ellipse1 = 0;
    ellipse2 = 0;

    actionsMenu = 0;
}

void CGOperator::setIsHighlight(bool isH){
    this->isHighLight = isH;
    if(!isHighLight) this->update();
}

void CGOperator::updateState(COperator::State state){
    if(errormsg.compare("") != 0){
        outerColor = Qt::red;
        this->update();
        QString n = "ERROR: "+errormsg+"\n";
        n = n+this->model->getInformation();
        this->setToolTip(QObject::tr(n.toStdString().c_str()));
    }else{
        if(state == COperator::RUNNING){
            outerColor = Qt::yellow;
        }else if(state == COperator::NOTRUNNABLE){
            outerColor = Qt::gray;
        }else if(state == COperator::RUNNABLE){
            outerColor = Qt::green;
        }
        this->update();
        QString n = this->model->getInformation();

        this->setToolTip(QObject::tr(n.toStdString().c_str()));
    }
}

QString CGOperator::getError(){
    return errormsg;
}

void CGOperator::setError(QString errorMsg){
    this->errormsg = errorMsg;
}

CGOperator* CGOperator::clone(){
    CGOperator* ope = new CGOperator(operatorMenu,connectorMenu,0,0);
    CGOperatorModel* modelClone = CGCompositionFactory::getInstance()->getCGOperatorByName(this->model->getKey());
    ope->setModel(modelClone);
    ope->renderModel();
    modelClone->setView(ope);
    return ope;
}

void CGOperator::renderModel(){


    //process ray
    if(model->getConnectorsList().size()>4){
        for(int i=0;i<model->getConnectorsList().size()-4;i++){
            rayon = rayon + 10;
        }
    }

    foreach(CGConnector*c,this->connectorsL){
        c->hide();
        c->scene()->removeItem(c);
    }
    this->connectorsL.clear();

    QList<CGConnectorModel*> connectorsL = model->getConnectorsList();
    int nbrin=0;
    int nbrout=0;
    foreach(CGConnectorModel*c,connectorsL){
        if(c->getSide().compare("IN")==0){
            nbrin++;
        }else{
            nbrout++;
        }
    }
    int i=0,j=0;
    foreach(CGConnectorModel*c,connectorsL){

        if(c->getSide().compare("IN")==0){
            CGConnector* connector = new CGConnector(this->connectorMenu, c->getSide(), c, this, scene());
            connector->setOperator(this);
            this->connectorsL.push_back(connector);
            connector->setRayon(rayon+10*rayon/100);
            qreal x,y;
            double angle = 3.14*(135+90*(j*1.0/nbrin))/180;
            x = cos(angle)*rayon/2;
            y = sin(angle)*rayon/2;
            j++;
            connector->setPos(x,y); //operator relative pos (origin on circle center)
        }
        else{
            CGConnector* connector2 = new CGConnector(this->connectorMenu, c->getSide(), c, this, scene());
            connector2->setOperator(this);
            connector2->setRayon(rayon+10*rayon/100);
            this->connectorsL.push_back(connector2);
            qreal x,y;
            double angle = 3.14*(45-90*(i*1.0/nbrout))/180;
            x = cos(angle)*rayon/2;
            y = sin(angle)*rayon/2;
            i++;
            connector2->setPos(x,y); //operator relative pos (origin on circle center)
        }
    }
    foreach(CGConnector*c,this->connectorsL){
        c->updatePosition();
    }

    nbConnectors = nbrin+nbrout;
}

int CGOperator::centerName(int size){
    int unity = 4;

    int width = size*unity;
    return -width/2;
}


void CGOperator::drawInfo(QPainter* painter){
    QString name2(this->getModel()->getName());

    QFont f("times new roman,utopia");
    f.setStyleStrategy(QFont::ForceOutline);
    f.setPointSize(6);
    f.setStyleHint(QFont::Times);
    int size = name2.size();
    if(size > 22){
        name2.resize(19);
        name2.append("...");
        size = 22;
    }
    painter->setBrush(textColor);

    painter->setFont(f);

    QString idperf = QString::number(this->getModel()->getId());

    int x = centerName(idperf.size());
    painter->drawText(x, -10, idperf);
    int c = centerName(size);
    painter->drawText(c, 0, name2);
}

void CGOperator::setModel(CGOperatorModel* model){
    this->model = model;
    //create tool tip
    QString n = this->model->getInformation();
    this->setToolTip(QObject::tr(n.toStdString().c_str()));
}

CGOperatorModel* CGOperator::getModel(){
    return model;
}

void CGOperator::disconnect(){
    foreach (CGConnector *c, this->connectorsL) {
        if(c->getIsControlConnected() && c->getConnectedControlConnector()!=NULL){
            CGConnector* connectortoconnect = c->getConnectedControlConnector();
            CGLayoutItemConnector* lconnectortoconnect = qgraphicsitem_cast<CGLayoutItemConnector *>(connectortoconnect);
            CGLayoutItem* layitem = qgraphicsitem_cast<CGLayoutItem *>(lconnectortoconnect->parentItem());
            CControl* control = layitem->getControl();

            CGDisconnectConnector* disc = new CGDisconnectConnector();
            disc->setConnector(c);
            disc->setControlConnector(connectortoconnect);
            disc->setControl(control);
            disc->doCommand();
        }
        if(c->getConnectedConnector() != NULL){
            CGDisconnectConnector* disc = new CGDisconnectConnector();
            disc->setConnector(c);
            disc->doCommand();
        }
    }
}

void CGOperator::removeConnector(CGConnector *){

}

void CGOperator::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *  ){
    //TODO show augmented informations (circle infos)
    if(!this->doc){
        this->doc = true;
        this->drawDocs();
    }else{
        this->doc = false;
        this->drawDocs();
    }
    this->update();
    if (this->scene()!=0)this->scene()->update();
}

void CGOperator::updateDocs(){
    QMap<int, QGraphicsTextItem*>::iterator j = mapDoc.begin();
    while (j != mapDoc.end()) {
        CGConnector* c = this->getConnectorById(j.key());
        QGraphicsTextItem* textItem = j.value();
        QPointF p = c->pos();
        if(p.x()>=0 && p.y()>=0){
            QPointF p2;
            p2.setX(p.x()+20);
            p2.setY(p.y()-20);
            textItem->setPos(p2);
        }
        if(p.x()>0 && p.y()<0){
            QPointF p2;
            p2.setX(p.x()+20);
            p2.setY(p.y()-20);
            textItem->setPos(p2);
        }
        if(p.x()<0 && p.y()>=0){
            QPointF p2;
            p2.setX(p.x()-20-textItem->boundingRect().width());
            p2.setY(p.y()-20);
            textItem->setPos(p2);
        }
        if(p.x()<0 && p.y()<=0){
            QPointF p2;
            p2.setX(p.x()-20-textItem->boundingRect().width());
            p2.setY(p.y()-20);
            textItem->setPos(p2);
        }
        //        textItem->update();
        j++;
    }
}

void CGOperator::setDoc(bool doc){
    this->doc = doc;
}

void CGOperator::drawDocs(){
    if(doc && ellipse1 == 0 && ellipse2 == 0){
        ellipse1 = new QGraphicsEllipseItem(this);
        ellipse2 = new QGraphicsEllipseItem(this);
        QColor c = Qt::white;
        c.setAlpha(50);
        ellipse1->setBrush(c);
        ellipse2->setBrush(c);

        ellipse1->setZValue(-1200);
        ellipse2->setZValue(-1100);
        int maxw = 0;
        foreach(CGConnector* connector, this->connectorsL){
            QPointF p = connector->pos();
            if(p.x()>=0 && p.y()>=0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()+40);
                p2.setY(p.y()-40);
                textItem->setPos(p2);
                textItem->setPlainText(connector->getModel()->getDataType()+" "+connector->getModel()->getName());
                textItem->setDefaultTextColor(Qt::white);
            }
            if(p.x()>0 && p.y()<0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()+40);
                p2.setY(p.y()-40);
                textItem->setPos(p2);
                textItem->setPlainText(connector->getModel()->getDataType()+" "+connector->getModel()->getName());
                textItem->setDefaultTextColor(Qt::white);
            }
            if(p.x()<0 && p.y()>=0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                textItem->setPlainText(connector->getModel()->getDataType()+" "+connector->getModel()->getName());
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()-40-textItem->boundingRect().width());
                p2.setY(p.y()-40);
                textItem->setPos(p2);
                textItem->setDefaultTextColor(Qt::white);
            }
            if(p.x()<0 && p.y()<=0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                textItem->setPlainText(connector->getModel()->getDataType()+" "+connector->getModel()->getName());
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()-40-textItem->boundingRect().width());
                p2.setY(p.y()-40);
                textItem->setPos(p2);
                textItem->setDefaultTextColor(Qt::white);
            }
        }

        maxw = rayon+20;
        ellipse1->setRect(-maxw/2-5,-maxw/2-5, maxw+10,maxw+10);
        ellipse2->setRect(-maxw/2,-maxw/2, maxw,maxw);


        //        if(this->getModel()->getInformation().compare("")!=0){
        //            informations  = new QGraphicsTextItem(this);
        //            informations->setTextInteractionFlags(Qt::TextBrowserInteraction);
        //            informations->setHtml(this->getModel()->getInformation());
        //            informations->setDefaultTextColor(Qt::white);
        //            informations->setPos(-informations->boundingRect().width()/2,this->rayon/2+20);
        //        }else{
        //            informations  = new QGraphicsTextItem(this);
        //            informations->setTextInteractionFlags(Qt::TextBrowserInteraction);
        //            informations->setHtml("This information is empty. Contact the dictionnary owner for more informations.<br>Go to <a href=\"www.shinoe.com\">www.shinoe.com</a>.");
        //            informations->setDefaultTextColor(Qt::white);
        //            informations->setPos(-informations->boundingRect().width()/2,this->rayon/2+20);
        //        }
        //        c.setAlpha(0);
        //        docrect1 = new QGraphicsRectItem(informations);
        //        docrect1->setBrush(c);
        //        docrect1->setRect(informations->boundingRect());
        //        docrect2 = new QGraphicsRectItem(informations);
        //        docrect2->setBrush(c);
        //        docrect2->setRect(informations->boundingRect());

    }else{
        foreach(QGraphicsTextItem* textItem, itemDocList){
            this->scene()->removeItem(textItem);
        }
        //        this->scene()->removeItem(docrect2);
        //        this->scene()->removeItem(docrect1);
        //        this->scene()->removeItem(informations);
        if(ellipse1!=0) this->scene()->removeItem(ellipse1);
        if(ellipse2!=0) this->scene()->removeItem(ellipse2);
        ellipse1 = 0;
        ellipse2 = 0;
        mapDoc.clear();
        itemDocList.clear();
    }
}

void CGOperator::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget *  ){
    CLogger::getInstance()->log("PAINT",CLogger::DEBUG,"CGOperator::PAINT id:"+QString::number(this->getModel()->getId()));

    //    foreach (CGConnector *connector, connectorsL) {
    //        connector->updatePosition();
    //        if(connector->isConnected()){
    //            connector->getConnectedConnector()->updatePosition();
    //        }
    //    }
    if(errormsg.compare("") != 0){
        outerColor = Qt::red;
    }

    if(this->isHighLight){
        QColor c = Qt::cyan;
        c.setAlpha(150);
        painter->setBrush(c);
        painter->drawEllipse(-(10+rayon+100)/2, -(10+rayon+100+2)/2, rayon+100+10, rayon+100+10); //origin on circle center
    }else if(this->isSelected()){
        QPen pen = painter->pen();
        pen.setWidth(3);
        painter->setPen(pen);
    }else{
        painter->setPen(Qt::SolidLine);
    }
    //    QString message = "operator position x: "+QString::number(this->scenePos().x())+" y: "+QString::number(this->scenePos().y());
    //    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,message);

    painter->setBrush(outerColor);
    painter->drawEllipse(-rayon/2, -rayon/2, rayon, rayon); //origin on circle center
    painter->setBrush(innerColor);
    painter->drawEllipse(-(rayon-20)/2, -(rayon-20)/2, rayon-20, rayon-20); //origin on circle center
    this->drawInfo(painter);
    this->updateDocs();
}

QRectF CGOperator::boundingRect() const{
    //    if(doc) return QRectF(-60, -60, 400, 200);
    if(!isHighLight) return QRectF(-(rayon+10)/2, -(rayon+10)/2, rayon+10, rayon+10); //origin on circle center
    return QRectF(-(10+rayon+100)/2, -(10+rayon+100+2)/2, rayon+100+10, rayon+100+10);
}

void CGOperator::addConnector(CGConnector *){

}

void CGOperator::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    setSelected(true);

    //create interation menu
    CGCompositionManager::getInstance()->getComposer()->removePreviousActionList();
    if(actionsMenu!=0) delete actionsMenu;
    actionsMenu = new QMenu("actions");

    vector<string> acts = this->model->getActionList();
    vector<string>::iterator it = acts.begin();
    int i = 0;
    for(;it!=acts.end();it++){
        string action = (*it);
        QAction* act = new QAction(QIcon(":/icons/cog.png"),
                                   action.c_str(),CGCompositionManager::getInstance()->getCurrentCompositionModel()->getScene());
        act->setStatusTip(action.c_str());
        //        QObject::connect(actionsMenu, SIGNAL(triggered(QAction*)), CGCompositionManager::getInstance()->getCurrentCompositionModel()->getScene(), SLOT(action(QAction*)));
        QObject::connect(act, SIGNAL(triggered()), CGCompositionManager::getInstance()->getCurrentCompositionModel()->getScene(), SLOT(action()));
        actionsMenu->addAction(act);
        i++;
    }
    if(i!=0) CGCompositionManager::getInstance()->getComposer()->addActionList(actionsMenu);
    operatorMenu->exec(event->screenPos());
}

void CGOperator::saveAction(QString action){
    actionsL.append(action);
}

QList<QString> CGOperator::getActionList(){
    return actionsL;
}

QMenu* CGOperator::getActionMenu(){
    return actionsMenu;
}

QVariant CGOperator::itemChange(GraphicsItemChange change, const QVariant &value){
    if(!isSelected()){
        isHighLight = false;
        this->update();
    }
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (CGConnector *connector, connectorsL) {
            connector->updatePosition();
            if(connector->isConnected()){
                connector->getConnectedConnector()->updatePosition();
            }
        }
    }
    return QGraphicsItem::itemChange(change, value);;
}

QPixmap CGOperator::image()
{
    QPixmap pixmap(250, 250);

    return pixmap;
}

CGConnector* CGOperator::getConnectorById(int id){
    foreach(CGConnector* connector, connectorsL){
        if(connector->getModel()->getId() == id){
            return connector;
        }
    }
    return NULL;
}

QList<CGConnector*> CGOperator::getConnectorsList(){
    return connectorsL;
}

CGConnector::CGConnector(QMenu *contextMenu, QString side, CGConnectorModel* model,
                         QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    this->connectorMenu = contextMenu;
    this->model = model;
    this->connectedConnector = NULL;
    this->arrow = NULL;
    crayon = 30;
    this->side = side;
    this->isPressed = true;
    this->isUp = false;
    isMoving = false;

    this->innerColor = Qt::black;
    this->outerColor = Qt::gray;
    outerColor.setAlpha(150);

    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->compatibleMode = false;
    this->controlMode = false;
    this->isControlConnected = false;
    this->highlight = false;

    controlConnectionRect.setRect(-20,-20,10,10);

    if(this->getModel()!=0){
        QString n = this->getModel()->getDataType()+" "+this->getModel()->getName();
        this->setToolTip(n);
    }
}

void CGConnector::setColor(QColor c){
    outerColor = c;
}

QColor CGConnector::getColor(){
    return outerColor;
}

void CGConnector::updateState(CPlug::State state){
    if(state == CPlug::EMPTY){
        outerColor = Qt::gray;
    }else if(state == CPlug::NEW){
        outerColor = Qt::green;
    }else if(state == CPlug::OLD){
        outerColor = Qt::blue;
    }

    outerColor.setAlpha(150);
    this->update();

    QString n = this->getModel()->getDataType()+" "+this->getModel()->getName();
    this->setToolTip(n);
}

CGConnector* CGConnector::getConnectedConnector(){
    return this->connectedConnector;
}

bool CGConnector::getIsHightLight(){
    return this->highlight;
}

void CGConnector::setIsHighLight(bool highlight){
    this->highlight = highlight;
    if(highlight){
        this->parentItem()->setSelected(true);
        CGLayoutManager::getInstance()->getController()->toFront();
    }/*else{
        this->parentItem()->setSelected(false);
    }*/
    this->update();
}

CGConnector* CGConnector::getNearest(){
    CGConnector* oneWay = NULL;
    if(this->getModel()!=NULL) oneWay = this;
    if(this->getModel()==NULL){
        if(this->type() == CGMultiConnector::Type){
            CGMultiConnector* multiconnector = qgraphicsitem_cast<CGMultiConnector *>(this);
            CGConnector* otherSide = multiconnector->getOtherSide();
            while(otherSide->getConnectedConnector()!=NULL
                  && otherSide->getConnectedConnector()->getModel()==NULL
                  && otherSide->getConnectedConnector()->type() == CGMultiConnector::Type){
                multiconnector = qgraphicsitem_cast<CGMultiConnector *>(otherSide->getConnectedConnector());
                otherSide = multiconnector->getOtherSide();
            }
            if(otherSide->getConnectedConnector()!=NULL
                    && otherSide->getConnectedConnector()->getModel()!=NULL){
                oneWay = otherSide->getConnectedConnector();
            }
        }
    }
    return oneWay;
}

void CGConnector::connectToNearest(CGConnector* connector){
    CGConnectorModel* oneWay = NULL;
    CGConnectorModel* otherWay = NULL;
    if(this->getModel()!=NULL) oneWay = this->getModel();
    if(connector->getModel()!=NULL) otherWay = connector->getModel();
    if(this->getModel()==NULL){
        if(this->type() == CGMultiConnector::Type){
            CGMultiConnector* multiconnector = qgraphicsitem_cast<CGMultiConnector *>(this);
            CGConnector* otherSide = multiconnector->getOtherSide();
            while(otherSide->getConnectedConnector()!=NULL
                  && otherSide->getConnectedConnector()->getModel()==NULL
                  && otherSide->getConnectedConnector()->type() == CGMultiConnector::Type){
                multiconnector = qgraphicsitem_cast<CGMultiConnector *>(otherSide->getConnectedConnector());
                otherSide = multiconnector->getOtherSide();
            }
            if(otherSide->getConnectedConnector()!=NULL
                    && otherSide->getConnectedConnector()->getModel()!=NULL){
                oneWay = otherSide->getConnectedConnector()->getModel();
            }
        }
    }
    if(connector->getModel()==NULL){
        if(connector->type() == CGMultiConnector::Type){
            CGMultiConnector* multiconnector = qgraphicsitem_cast<CGMultiConnector *>(connector);
            CGConnector* otherSide = multiconnector->getOtherSide();
            while(otherSide->getConnectedConnector()!=NULL
                  && otherSide->getConnectedConnector()->getModel()==NULL
                  && otherSide->getConnectedConnector()->type() == CGMultiConnector::Type){
                multiconnector = qgraphicsitem_cast<CGMultiConnector *>(otherSide->getConnectedConnector());
                otherSide = multiconnector->getOtherSide();
            }
            if(otherSide->getConnectedConnector()!=NULL
                    && otherSide->getConnectedConnector()->getModel()!=NULL){
                otherWay = otherSide->getConnectedConnector()->getModel();
            }
        }
    }
    if(oneWay != NULL && otherWay != NULL){
        oneWay->connect(otherWay);
        otherWay->connect(oneWay);
    }
}

void CGConnector::setIsControlConnected(bool isControlConnected){
    this->isControlConnected = isControlConnected;
}

void CGConnector::connect(CGConnector* connectedConnector){
    if(this->getSide().compare(connectedConnector->getSide()) != 0){
        if(this->type() == CGLayoutItemConnector::Type && !this->isConnected()){
            CGLayoutItem* layoutConnector = qgraphicsitem_cast<CGLayoutItem*>(this->parentItem());
            CControl*control = layoutConnector->getControl();
            if(this->getSide().compare("IN") == 0){
                //view
                //TODO [ISO] refactoring to be iso with the machine
                int index = connectedConnector->getModel()->getId() - connectedConnector->getModel()->getCGOperatorModel()->getNumPlugIn();
                control->connectPlugIn(this->getId(),connectedConnector->getModel()->getCGOperatorModel()->getId(),index);
                this->connectedConnector = connectedConnector;
            }else{
                //editor
                //TODO [ISO] refactoring to be iso with the machine
                int indexplugoutcontrol = this->getId()- control->structurePlug().plugIn().size();
                control->connectPlugOut(indexplugoutcontrol,connectedConnector->getModel()->getCGOperatorModel()->getId(),connectedConnector->getModel()->getId() );
                this->connectedConnector = connectedConnector;
            }
        }else{
            if(connectedConnector!=0 && connectedConnector->type() == CGLayoutItemConnector::Type){
                if(this->getSide().compare("IN") == 0){
                    this->connectedConnector = connectedConnector;
                    this->connectToNearest(connectedConnector);
                    connectedConnector->connect(this);
                    connectedLayoutConnector = qgraphicsitem_cast<CGLayoutItemConnector*>(connectedConnector);
                }else{
                    //store connectedControl to disconnect view
                    connectedLayoutConnector = qgraphicsitem_cast<CGLayoutItemConnector*>(connectedConnector);
                }
            }else {
                if(this->getConnectedConnector() == NULL && connectedConnector!=0){
                    if(this->getSide().compare("IN") == 0){
                        arrow = new CGArrow(this, connectedConnector);
                        arrow->setColor(Qt::gray);
                        this->connectedConnector = connectedConnector;
                        arrow->setZValue(-1000.0);
                        if(this->parentItem()!=0 && this->parentItem()->scene()!=0){
                            this->parentItem()->scene()->addItem(arrow);
                        }else{
                            CGCompositionManager::getInstance()->getCurrentComposerScene()->addItem(arrow);
                        }
                        arrow->updatePosition();
                        if(this->type() == CGMultiConnector::Type){
                            CGMulti* m = qgraphicsitem_cast<CGMulti*>(this->parentItem());
                            if(m->getMode() == CGMulti::INNER){
                                arrow->setIsMulti(true,false);
                            }
                        }
                        if(this->getConnectedConnector()->type() == CGMultiConnector::Type){
                            CGMulti* m = qgraphicsitem_cast<CGMulti*>(this->getConnectedConnector()->parentItem());
                            if(m->getMode() == CGMulti::INNER){
                                arrow->setIsMulti(true,true);
                            }
                        }
                        this->connectToNearest(connectedConnector);

                        this->connectedConnector->setArrow(arrow);
                        this->connectedConnector->updatePosition();
                        connectedConnector->connect(this);
                        this->updatePosition();
                        arrow->updatePosition();
                    }else{
                        if(this->getModel()!=NULL && connectedConnector->getModel()!=NULL){
                            if(this->getModel()->getDataType().compare(connectedConnector->getModel()->getDataType()) ==0
                                    || this->getModel()->getDataType().compare("CDATAGENERIC") ==0
                                    || connectedConnector->getModel()->getDataType().compare("CDATAGENERIC") ==0){
                                if(this->getModel()->getSide().compare(connectedConnector->getSide()) != 0){
                                    this->getModel()->connect(connectedConnector->getModel());
                                }
                            }
                        }
                        this->connectedConnector = connectedConnector;
                        this->updatePosition();
                        connectedConnector->connect(this);
                    }
                }
            }
        }
    }
}

bool CGConnector::getControlMode(){
    return controlMode;
}

bool CGConnector::getIsControlConnected(){
    return isControlConnected;
}

bool CGConnector::getCompatibleMod(){
    return compatibleMode;
}

void CGConnector::disconnect(){
    CGConnector* con = this->connectedConnector;
    CGArrow* arr = arrow;
    if(isConnected() && this->type() == CGLayoutItemConnector::Type){
        CGLayoutItem* layoutConnector = qgraphicsitem_cast<CGLayoutItem*>(this->parentItem());
        CControl*control = layoutConnector->getControl();
        if(this->getSide().compare("IN") == 0) control->deconnectPlugIn(this->getId());
        if(this->getSide().compare("OUT") == 0) {
            int indexplugoutcontrol = this->getId()- control->structurePlug().plugIn().size();
            control->deconnectPlugOut(indexplugoutcontrol);
        }
        this->connectedConnector = NULL;
    }else if(isConnected() && this->connectedConnector->type() == CGLayoutItemConnector::Type){
        this->connectedConnector = NULL;
    }else{
        if(isConnected()){
            if(side.compare("IN") == 0 && arr != NULL){
                if(scene()) scene()->removeItem(arr);
                delete arr;
            }
            this->connectedConnector = NULL;
            this->arrow = NULL;
            if(con->getConnectedConnector()!=NULL) this->disconnectNearest(con);
            con->disconnect();
        }
    }
    if(this->getSide().compare("IN") == 0) isControlConnected = false;
    this->update();
}

CGConnector* CGConnector::getConnectedControlConnector(){
    return connectedLayoutConnector;
}

void CGConnector::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *  ){
    if(isControlConnected){
        CGLayoutManager::getInstance()->showConnected(this->getConnectedControlConnector());
    }
}

void CGConnector::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(controlMode){
            isControlConnected = true;
            CControl* control = CGCompositionManager::getInstance()->getCurrentComposerScene()->getControlToConnect();
            CGConnector* connectortoconnect = CGCompositionManager::getInstance()->getCurrentComposerScene()->getConnectorToConnect();
            CGCommandManager::getInstance()->startAction("CONNECT");
            CGConnectConnector* connect = new CGConnectConnector();
            connect->setControl(control);
            connect->setControlConnector(connectortoconnect);
            connect->setConnector(this);
            connect->doCommand();
            connectortoconnect->parentItem()->setSelected(false);
            CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
            CGCommandManager::getInstance()->endAction("CONNECT");
        }
    }
}

void CGConnector::disconnectNearest(CGConnector* connector){
    CGConnectorModel* oneWay = NULL;
    CGConnectorModel* otherWay = NULL;
    if(this->getModel()!=NULL) oneWay = this->getModel();
    if(connector->getModel()!=NULL) otherWay = connector->getModel();
    if(this->getModel()==NULL){
        if(this->type() == CGMultiConnector::Type){
            CGMultiConnector* multiconnector = qgraphicsitem_cast<CGMultiConnector *>(this);
            CGConnector* otherSide = multiconnector->getOtherSide();
            while(otherSide->getConnectedConnector()!=NULL
                  && otherSide->getConnectedConnector()->getModel()==NULL
                  && otherSide->getConnectedConnector()->type() == CGMultiConnector::Type){
                multiconnector = qgraphicsitem_cast<CGMultiConnector *>(otherSide->getConnectedConnector());
                otherSide = multiconnector->getOtherSide();
            }
            if(otherSide->getConnectedConnector()!=NULL
                    && otherSide->getConnectedConnector()->getModel()!=NULL){
                oneWay = otherSide->getConnectedConnector()->getModel();
            }
        }
    }
    if(connector->getModel()==NULL){
        if(connector->type() == CGMultiConnector::Type){
            CGMultiConnector* multiconnector = qgraphicsitem_cast<CGMultiConnector *>(connector);
            CGConnector* otherSide = multiconnector->getOtherSide();
            while(otherSide->getConnectedConnector()!=NULL
                  && otherSide->getConnectedConnector()->getModel()==NULL
                  && otherSide->getConnectedConnector()->type() == CGMultiConnector::Type){
                multiconnector = qgraphicsitem_cast<CGMultiConnector *>(otherSide->getConnectedConnector());
                otherSide = multiconnector->getOtherSide();
            }
            if(otherSide->getConnectedConnector()!=NULL
                    && otherSide->getConnectedConnector()->getModel()!=NULL){
                otherWay = otherSide->getConnectedConnector()->getModel();
            }
        }
    }
    if(oneWay != NULL && otherWay != NULL){
        oneWay->disconnect();
        otherWay->disconnect();
    }
}

CGArrow* CGConnector::getArrow(){
    return arrow;
}

void CGConnector::setArrow(CGArrow* arrow){
    this->arrow = arrow;
}

void CGConnector::removeArrow(){
    this->arrow = NULL;
}

void CGConnector::setOperator(CGOperator* ope){
    this->ope = ope;
}

CGOperator* CGConnector::getOperator(){
    return this->ope;
}

void CGConnector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    foreach(QGraphicsItem* item, CGCompositionManager::getInstance()->getCurrentComposerScene()->items()){
        item->setSelected(false);
    }
    setSelected(true);
    CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
    connectorMenu->exec(event->screenPos());
}

QVariant CGConnector::itemChange(GraphicsItemChange change, const QVariant &value){
    if (arrow != NULL && change == QGraphicsItem::ItemPositionChange) {
        arrow->updatePosition();
    }
    return value;
}

void CGConnector::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget *  ){
    CLogger::getInstance()->log("PAINT",CLogger::DEBUG,"CGConnector::PAINT");
    this->updatePosition();
    if(isMoving){
        QColor c = Qt::cyan;
        c.setAlpha(150);

        painter->setBrush(c);
        painter->drawEllipse(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
    }
    if(highlight){
        QColor c = Qt::yellow;
        c.setAlpha(50);

        painter->setBrush(c);
        painter->drawEllipse(-(crayon+80)/2, -(crayon+80)/2, crayon+80, crayon+80);
    }
    if(compatibleMode){
        QColor c = Qt::yellow;
        c.setAlpha(150);

        painter->setBrush(c);
        painter->drawEllipse(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
    }
    if(controlMode){
        QColor c = Qt::darkYellow;
        c.setAlpha(150);

        painter->setBrush(c);
        painter->drawEllipse(-(crayon+10)/2, -(crayon+10)/2, crayon+10, crayon+10);
        painter->drawEllipse(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
    }
    if(isControlConnected){
        QColor c = Qt::white;
        c.setAlpha(150);
        painter->setBrush(c);
        painter->drawEllipse(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
    }
    if(isIn() && !isUp){
        painter->setPen(outerColor);
        painter->setBrush(outerColor);
        painter->drawEllipse(-(crayon)/2, -(crayon)/2, crayon, crayon);
        painter->setBrush(innerColor);
        painter->drawEllipse(-(crayon-10)/2, -(crayon-10)/2, crayon-10, crayon-10);
    }else if(!isUp){
        painter->setPen(innerColor);
        painter->setBrush(innerColor);
        painter->drawEllipse(-(crayon)/2, -(crayon)/2, crayon, crayon);
        painter->setBrush(outerColor);
        painter->drawEllipse(-(crayon-10)/2, -(crayon-10)/2, crayon-10, crayon-10);
    }else if(isIn() && isUp){
        painter->setPen(innerColor);
        painter->setBrush(innerColor);
        painter->drawEllipse(-(crayon-10)/2, -(crayon-10)/2, crayon-10, crayon-10);
    }else{
        painter->setBrush(Qt::white);
        painter->drawEllipse(-(crayon-10)/2, -(crayon-10)/2, crayon-10, crayon-10);
    }
}

QRectF CGConnector::boundingRect() const{
    if(highlight){
        return QRectF(-(crayon+80)/2, -(crayon+80)/2, crayon+80, crayon+80);
    }else if(compatibleMode || controlMode || isControlConnected || isMoving){
        return QRectF(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
    }else{
        return QRectF(-(crayon)/2, -(crayon)/2, crayon, crayon);
    }
}

void CGConnector::setControlMode(bool controlMode){
    this->controlMode = controlMode;
    this->update();
}

void CGConnector::setCompatibleMode(bool compatibleMode){
    this->compatibleMode = compatibleMode;
    this->update();
}
#define pi 3.14159265
namespace Private{
void intersectionUp( double myangle,  vector<double> v_angle,double excudedangle, double & d ){
    vector<double>::iterator it;
    for(it=v_angle.begin();it!=v_angle.end();it++){
        double diff=  (*it-myangle)*SCD::sgn((*it-myangle));
        //diff= min(3.14159265-diff,diff);
        if( diff<excudedangle){
            double angle = *it + excudedangle +0.05;
            double d1;
            intersectionUp(angle, v_angle,excudedangle,d1);
            if(d1==angle)
                d = angle;
            else
                d = d1;
            return;
        }
    }
    d = myangle;
}
void intersectionDown( double myangle,  vector<double> v_angle,double excudedangle, double & d ){
    vector<double>::iterator it;
    for(it=v_angle.begin();it!=v_angle.end();it++){
        double diff=  (*it-myangle)*SCD::sgn((*it-myangle));
        //diff= min(3.14159265-diff,diff);
        if( diff<excudedangle){
            double angle = *it - excudedangle -0.05;
            double d1;
            intersectionDown(angle, v_angle,excudedangle,d1);
            if(d1==angle)
                d = angle;
            else
                d = d1;
            return;
        }
    }
    d = myangle;
}
double minAngle( double angle1, double angle2 ){
    return min( min( (angle1-angle2)*SCD::sgn(angle1-angle2),(angle1+2*pi-angle2)*SCD::sgn(angle1-angle2)), (angle1-2*pi-angle2)*SCD::sgn(angle1-angle2) ) ;
}
}
QPointF CGConnector::checkPosition2(double px, double py){
    QList<CGConnectorModel*> cl = this->model->getCGOperatorModel()->getConnectorsList();

    double connectorRayon = 20;
    double rayonPos = sqrt(px*px+py*py);
    double ratio = connectorRayon/rayonPos;
    double angleexcluded = asin(ratio);
    double myangle = acos(px/rayonPos)*SCD::sgn(-py);
    double myangletemp=myangle;
    bool sidein;
    if(this->getSide()=="IN"){
        sidein =true;
    }else{
        sidein=false;
    }
    int id = this->getId();
    int maxin=0;
    int max=0;
    foreach(CGConnectorModel*cm, cl){
        CGConnector*c = this->model->getCGOperatorModel()->getView()->getConnectorById(cm->getId());
        if(c->getSide()=="IN"){
            maxin++;
        }
        max++;
    }

    int maxout=max-maxin;


    int idup;
    if(sidein ==true&&id>0)
        idup=id-1;
    else if(sidein ==true&&id==0)
        idup=maxin;
    else if(sidein==false&&(id-maxin)!=maxout-1)
        idup=id+1;
    else
        idup=maxin-1;

    int iddown;
    if(sidein ==true&&id!=maxin-1)
        iddown=id+1;
    else if(sidein ==true&&id==maxin-1)
        iddown=max-1;
    else if(sidein==false&&(id-maxin)!=0)
        iddown=id-1;
    else
        iddown=0;
    double angleup=0,angleuptemp=0;
    double angledown=0,angledowntemp=0;
    foreach(CGConnectorModel*cm, cl){
        CGConnector*c = this->model->getCGOperatorModel()->getView()->getConnectorById(cm->getId());
        if(c->getId()==idup)
        {
            angleup = acos(c->pos().x()/rayonPos)*SCD::sgn(-c->pos().y());
            angleuptemp = angleup;
        }
        if(c->getId()==iddown)
        {
            angledown = acos(c->pos().x()/rayonPos)*SCD::sgn(-c->pos().y());
            angledowntemp = angledown;
        }

    }
    angleup-=(angleexcluded+0.01);
    angledown+=(angleexcluded+0.01);
    bool temp =false;
    if(angleup>=0&&angledown>=0&&temp ==false){
        temp =true;

        if(angleup<angledown)
        {
            angleup+=2*pi;
        }
        if(myangle<angledown)
        {
            myangle+=2*pi;
        }
    }
    else if(angleup>=0&&angledown<0&&temp ==false){
        temp =true;
    }
    else if(angleup<0&&angledown>=0&&temp ==false){
        temp =true;
        angleup  +=2*pi;
        if(myangle<0)
            myangle+=2*pi;

    }else if(angleup<0&&angledown<0&&temp ==false){

        if(angledown<angleup)
        {
            angledown+=2*pi;
            if(myangle<0)
            {
                myangle+=2*pi;
            }
        }

        angleup+=2*pi;
    }
    if(myangle<angleup&&myangle> angledown)
        return QPointF(px,py);
    else{
        double diff1 = Private::minAngle(angleuptemp,myangletemp);
        double diff2 =  Private::minAngle(angledowntemp,myangletemp);
        if(diff1<diff2)
            return QPointF(cos(angleup)*rayonPos,-sin(angleup)*rayonPos);
        else
            return QPointF(cos(angledown)*rayonPos,-sin(angledown)*rayonPos);
    }
}
QPointF CGConnector::checkPosition(double px, double py){
    QList<CGConnectorModel*> cl = this->model->getCGOperatorModel()->getConnectorsList();

    double connectorRayon = 20;
    double rayonPos = sqrt(px*px+py*py);
    double ratio = connectorRayon/rayonPos;
    double angleexcluded = asin(ratio);
    double myangle = acos(px/rayonPos)*SCD::sgn(py);
    vector<double> v_angle;
    foreach(CGConnectorModel*cm, cl){
        CGConnector*c = this->model->getCGOperatorModel()->getView()->getConnectorById(cm->getId());
        if(c != this){
            //            c->getSide();
            //            c->getId();
            //QPointF p = c->pos(); //other connector pos
            double rayon = sqrt(c->pos().x()*c->pos().x()+c->pos().y()*c->pos().y());
            double angle = acos(c->pos().x()/rayon)*SCD::sgn(c->pos().y());
            v_angle.push_back(angle);
        }
    }
    double dup;
    Private::intersectionUp(myangle,v_angle,angleexcluded,dup);
    if(dup==myangle)
        return QPointF(px,py);
    double ddown;
    Private::intersectionDown(myangle,v_angle,angleexcluded,ddown);
    if(ddown==myangle)
        return QPointF(px,py);
    double diffup =(dup-myangle)*SCD::sgn((dup-myangle));
    double diffdown = (ddown-myangle)*SCD::sgn((ddown-myangle));

    if(diffup<diffdown )
        return QPointF(rayonPos*cos(dup),rayonPos*sin(dup));
    else
        return QPointF(rayonPos*cos(ddown),rayonPos*sin(ddown));


}

void CGConnector::setIsMoving(bool b){
    this->isMoving = b;
}
void CGConnector::up(){
    if(this->type() == CGConnector::Type){
        foreach(CGConnector* c,this->ope->getConnectorsList()){
            if(this != c){
                if(this->zValue()<=c->zValue())this->setZValue(c->zValue()+2);
            }
        }
    }
}

void CGConnector::moveTo(QPointF scenePos){
    this->up();
    QPointF posM = this->parentItem()->mapFromScene(scenePos);
    /*
    homothécie: OM * length(OP)/lenght(OM)= OP
    avec P (px,py) le point sur le cercle et O (0,0) le centre du cercle et M(x,y) le point distant
    length(OM)/length(OP):
    length(OP) = rayon de ton cercle
    length(OM) = sqrt(x^2+y^2)

    or OM * length(OM)/lenght= OP
    d'où,
    px = x * (R/sqrt(x^2+y^2))
    py = y * (R/sqrt(x^2+y^2))
    */
    double r = rayon/2;
    double carre = posM.x()*posM.x()+posM.y()*posM.y();
    double px = posM.x() * (r/sqrt(carre));
    double py = posM.y() * (r/sqrt(carre));
    //    QPointF p;
    //    p = checkPosition(px,py);

    this->setPos(px,py);
}

void CGConnector::updatePosition(){
    if(CGProject::getInstance()->getConnectorOrderMode().compare("HARD") != 0){
        CLogger::getInstance()->log("INSTANCE",CLogger::DEBUG,"CGConnector::updatePosition()");
        if((isControlConnected && this->getSide().compare("OUT") == 0) || !isControlConnected){
            if(isConnected()){
                QPointF posM = this->parentItem()->mapFromScene(this->connectedConnector->scenePos());
                /*
                    homothécie: OM * length(OP)/lenght(OM)= OP
                    avec P (px,py) le point sur le cercle et O (0,0) le centre du cercle et M(x,y) le point distant
                    length(OM)/length(OP):
                    length(OP) = rayon de ton cercle
                    length(OM) = sqrt(x^2+y^2)

                    or OM * length(OM)/lenght= OP
                    d'où,
                    px = x * (R/sqrt(x^2+y^2))
                    py = y * (R/sqrt(x^2+y^2))
                */
                double r = (rayon-10)/2;
                double carre = posM.x()*posM.x()+posM.y()*posM.y();
                double px = posM.x() * (r/sqrt(carre));
                double py = posM.y() * (r/sqrt(carre));

                QPointF p;
                if(CGProject::getInstance()->getConnectorOrderMode().compare("FREE") == 0){
                    p.setX(px);//FREE CONNECTORS !
                    p.setY(py);//FREE CONNECTORS !
                }else if(CGProject::getInstance()->getConnectorOrderMode().compare("MEDIUM") == 0){
                    p = checkPosition(px,py);
                }
                this->setPos(p.x(),p.y());
            }
        }
    }
}

void CGConnector::rePosition(){
    QPointF posM = this->parentItem()->mapFromScene(this->scenePos());
    /*
        homothécie: OM * length(OP)/lenght(OM)= OP
        avec P (px,py) le point sur le cercle et O (0,0) le centre du cercle et M(x,y) le point distant
        length(OM)/length(OP):
        length(OP) = rayon de ton cercle
        length(OM) = sqrt(x^2+y^2)

        or OM * length(OM)/lenght= OP
        d'où,
        px = x * (R/sqrt(x^2+y^2))
        py = y * (R/sqrt(x^2+y^2))
    */
    double r = (rayon-10)/2;
    double carre = posM.x()*posM.x()+posM.y()*posM.y();
    double px = posM.x() * (r/sqrt(carre));
    double py = posM.y() * (r/sqrt(carre));

    QPointF p;
    p.setX(px);//FREE CONNECTORS !
    p.setY(py);//FREE CONNECTORS !
    this->setPos(p.x(),p.y());
}


void CGConnector::freePosition(){
    if((isControlConnected && this->getSide().compare("OUT") == 0) || !isControlConnected){
        if(isConnected()){
            QPointF posM = this->parentItem()->mapFromScene(this->connectedConnector->scenePos());
            /*
                homothécie: OM * length(OP)/lenght(OM)= OP
                avec P (px,py) le point sur le cercle et O (0,0) le centre du cercle et M(x,y) le point distant
                length(OM)/length(OP):
                length(OP) = rayon de ton cercle
                length(OM) = sqrt(x^2+y^2)

                or OM * length(OM)/lenght= OP
                d'où,
                px = x * (R/sqrt(x^2+y^2))
                py = y * (R/sqrt(x^2+y^2))
            */
            double r = (rayon-10)/2;
            double carre = posM.x()*posM.x()+posM.y()*posM.y();
            double px = posM.x() * (r/sqrt(carre));
            double py = posM.y() * (r/sqrt(carre));

            QPointF p;
            p.setX(px);//FREE CONNECTORS !
            p.setY(py);//FREE CONNECTORS !
            this->setPos(p.x(),p.y());
        }
    }
}


bool CGConnector::isConnected(){
    if(this->connectedConnector != NULL){
        return true;
    }else{
        return false;
    }
}

bool CGConnector::isIn(){
    if(side.compare("IN")) return true;
    return false;
}

void CGConnector::setModel(CGConnectorModel* model){
    this->model = model;
}

CGConnectorModel* CGConnector::getModel(){
    return model;
}

void CGConnector::setIsUp(bool isup){
    this->isUp = isup;
}

int CGConnector::getRayon(){
    return this->rayon;
}

void CGConnector::setRayon(int rayon){
    this->rayon = rayon;
}

QString CGConnector::getSide(){
    return this->side;
}

int CGConnector::getId(){
    return model->getId();
}

CGMulti::CGMulti(QMenu *multiMenu,  QMenu *connectorMenu,
                 QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    this->multiMenu = multiMenu;
    this->connectorMenu = connectorMenu;
    this->isUp = false;
    innerColor = Qt::white;
    outerColor = Qt::gray;

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->model = NULL;

    countin = 0;
    countout = 0;

    rayon = 120;
    this->parentView = NULL;
    nbConnectors=1;

    ellipse1 = 0;
    ellipse2 = 0;

}

CGConnector* CGMulti::getConnectorById(int id){
    foreach(CGConnector* connector, connectorL){
        if(connector->getId() == id){
            return connector;
        }
    }
    return NULL;
}

QList<CGMultiConnector*> CGMulti::getMultiConnectorsList(){
    return multiconnectorsL;
}

CGMulti* CGMulti::clone(){
    QList<CGMulti*> modelsListCopy;
    QList<CGOperator*> operatorsListCopy;

    QMap<CGConnector*, CGConnector*> mapNewPrevious;
    QMap<CGConnector*, CGConnector*> mapPreviousNew;

    /////////////////////////////
    //clone structures & store connections informations
    foreach(CGCompositionModel* comp, this->model->getCompositionModelList()){
        CGMulti* multi = comp->getOuterView()->getParentView()->clone();
        multi->getOuterView()->setPos(comp->getOuterView()->scenePos());
        multi->getModel()->setName(comp->getName()+" "+QString::number(comp->getId()));
        modelsListCopy.append(multi);
        foreach(CGConnector* connector, comp->getOuterView()->getConnectorsList()){
            CGConnector* connectorCopy = multi->getOuterView()->getConnectorById(connector->getId());
            mapNewPrevious.insert(connectorCopy, connector);
            mapPreviousNew.insert(connector, connectorCopy);
        }
    }
    foreach(CGOperatorModel* ope, this->model->getOperatorModelList()){
        CGOperator* operatorNew = ope->getView()->clone();
        operatorNew->setPos(ope->getView()->scenePos());
        operatorsListCopy.append(operatorNew);
        foreach(CGConnector* connector, ope->getView()->getConnectorsList()){
            CGConnector* connectorCopy = operatorNew->getConnectorById(connector->getModel()->getId());
            mapNewPrevious.insert(connectorCopy, connector);
            mapPreviousNew.insert(connector, connectorCopy);
        }
    }
    /////////////////////////////

    /////////////////////////////
    //create new objects

    //CGCompositionModel
    CGCompositionModel* modeleClone = new CGCompositionModel(this->getModel()->getParentCompositionModel());
    CGMulti* multiClone = modeleClone->getInnerView()->getParentView();

    //rebuild multi structure
    foreach(CGMultiConnector* con, this->getMultiConnectorsList()){
        CGMultiConnector* m;
        if(con->getInnerConnector()->getSide().compare("IN")){
            m = multiClone->addConnector("OUTIN");
        }else{
            m = multiClone->addConnector("INOUT");
        }
        mapNewPrevious.insert(m->getInnerConnector(), con->getInnerConnector());
        mapPreviousNew.insert(con->getInnerConnector(), m->getInnerConnector());
    }
    //end
    /////////////////////////////

    /////////////////////////////
    //start populate object
    foreach(CGMulti* comp, modelsListCopy){
        modeleClone->activate(comp->getOuterView());
    }
    foreach(CGOperator* ope, operatorsListCopy){
        modeleClone->activate(ope);
    }
    //end
    /////////////////////////////

    /////////////////////////////
    //connect objects
    foreach(CGMulti* comp, modelsListCopy){
        foreach(CGConnector* con, comp->getOuterView()->getConnectorsList()){
            if(!con->isConnected() && mapNewPrevious.value(con)->isConnected()){
                CGConnector* toconnect = mapPreviousNew.value(mapNewPrevious.value(con)->getConnectedConnector());
                con->connect(toconnect);
            }
        }
    }
    foreach(CGOperator* ope, operatorsListCopy){
        foreach(CGConnector* con, ope->getConnectorsList()){
            if(!con->isConnected() && mapNewPrevious.value(con)->isConnected()){
                CGConnector* toconnect = mapPreviousNew.value(mapNewPrevious.value(con)->getConnectedConnector());
                con->connect(toconnect);
            }
        }
    }
    //end
    /////////////////////////////
    return multiClone;
}

void CGMulti::setDoc(bool doc){
    this->doc = doc;
}

void CGMulti::updateDocs(){
    QMap<int, QGraphicsTextItem*>::iterator j = mapDoc.begin();
    while (j != mapDoc.end()) {
        CGConnector* c = this->getConnectorById(j.key());
        QGraphicsTextItem* textItem = j.value();
        QPointF p = c->pos();
        if(p.x()>=0 && p.y()>=0){
            QPointF p2;
            p2.setX(p.x()+10);
            p2.setY(p.y()-10);
            textItem->setPos(p2);
        }
        if(p.x()>0 && p.y()<0){
            QPointF p2;
            p2.setX(p.x()+10);
            p2.setY(p.y()-10);
            textItem->setPos(p2);
        }
        if(p.x()<0 && p.y()>=0){
            QPointF p2;
            p2.setX(p.x()-10-textItem->boundingRect().width());
            p2.setY(p.y()-10);
            textItem->setPos(p2);
        }
        if(p.x()<0 && p.y()<=0){
            QPointF p2;
            p2.setX(p.x()-10-textItem->boundingRect().width());
            p2.setY(p.y()-10);
            textItem->setPos(p2);
        }
        //        textItem->update();
        j++;
    }
}

void CGMulti::drawDocs(){
    if(doc){
        ellipse1 = new QGraphicsEllipseItem(this);
        ellipse2 = new QGraphicsEllipseItem(this);
        QColor c = Qt::white;
        c.setAlpha(50);
        ellipse1->setBrush(c);
        ellipse2->setBrush(c);

        ellipse1->setZValue(-1200);
        ellipse2->setZValue(-1100);
        int maxw = 0;
        foreach(CGMultiConnector* cm, model->getOuterView()->getParentView()->getMultiConnectorsList()){
            CGConnector* connector = cm->getOuterConnector();
            cm = cm->getOuterConnector();
            QPointF p = connector->pos();
            if(p.x()>=0 && p.y()>=0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()+20);
                p2.setY(p.y()-20);
                textItem->setPos(p2);

                QString name = "";
                if(cm->getName().compare("")!=0
                        && cm->getName().compare("Other side not connected")!=0
                        && cm->getName().compare("Can't find name")!=0){
                    name = cm->getName();
                }else{
                    if(cm->getOtherSide()->isConnected()){
                        if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                            name = cm->getOtherSide()->getConnectedConnector()->getModel()->getName();
                            cm->setName(name);
                        }else{
                            name = "Can't find name";
                            cm->setName(name);
                        }
                    }else{
                        name = "Other side not connected";
                        cm->setName(name);
                    }
                }

                QString type = "";
                if(cm->getOtherSide()->isConnected()){
                    if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                        type =cm->getOtherSide()->getConnectedConnector()->getModel()->getDataType();
                    }else{
                        type ="UNKNOWN";
                    }
                }else{
                    type = "UNKNOWN";
                }

                textItem->setPlainText(QString::number(connector->getId())+" "+type+" "+name);
                textItem->setDefaultTextColor(Qt::white);
            }
            if(p.x()>0 && p.y()<0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()+20);
                p2.setY(p.y()-20);
                textItem->setPos(p2);
                QString name = "";
                if(cm->getName().compare("")!=0
                        && cm->getName().compare("Other side not connected")!=0
                        && cm->getName().compare("Can't find name")!=0){
                    name = cm->getName();
                }else{
                    if(cm->getOtherSide()->isConnected()){
                        if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                            name = cm->getOtherSide()->getConnectedConnector()->getModel()->getName();
                            cm->setName(name);
                        }else{
                            name = "Can't find name";
                            cm->setName(name);
                        }
                    }else{
                        name = "Other side not connected";
                        cm->setName(name);
                    }
                }

                QString type = "";
                if(cm->getOtherSide()->isConnected()){
                    if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                        type =cm->getOtherSide()->getConnectedConnector()->getModel()->getDataType();
                    }else{
                        type ="UNKNOWN";
                    }
                }else{
                    type = "UNKNOWN";
                }

                textItem->setPlainText(QString::number(connector->getId())+" "+type+" "+name);
                textItem->setDefaultTextColor(Qt::white);
            }
            if(p.x()<0 && p.y()>=0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                QString name = "";
                if(cm->getName().compare("")!=0
                        && cm->getName().compare("Other side not connected")!=0
                        && cm->getName().compare("Can't find name")!=0){
                    name = cm->getName();
                }else{
                    if(cm->getOtherSide()->isConnected()){
                        if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                            name = cm->getOtherSide()->getConnectedConnector()->getModel()->getName();
                            cm->setName(name);
                        }else{
                            name = "Can't find name";
                            cm->setName(name);
                        }
                    }else{
                        name = "Other side not connected";
                        cm->setName(name);
                    }
                }

                QString type = "";
                if(cm->getOtherSide()->isConnected()){
                    if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                        type =cm->getOtherSide()->getConnectedConnector()->getModel()->getDataType();
                    }else{
                        type ="UNKNOWN";
                    }
                }else{
                    type = "UNKNOWN";
                }

                textItem->setPlainText(QString::number(connector->getId())+" "+type+" "+name);
                textItem->setDefaultTextColor(Qt::white);
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()-20-textItem->boundingRect().width());
                p2.setY(p.y()-20);
                textItem->setPos(p2);
                textItem->setDefaultTextColor(Qt::white);
            }
            if(p.x()<0 && p.y()<=0){
                QGraphicsTextItem* textItem = new QGraphicsTextItem(this);
                itemDocList.append(textItem);
                mapDoc.insert(connector->getId(),textItem);
                QString name = "";
                if(cm->getName().compare("")!=0
                        && cm->getName().compare("Other side not connected")!=0
                        && cm->getName().compare("Can't find name")!=0){
                    name = cm->getName();
                }else{
                    if(cm->getOtherSide()->isConnected()){
                        if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                            name = cm->getOtherSide()->getConnectedConnector()->getModel()->getName();
                            cm->setName(name);
                        }else{
                            name = "Can't find name";
                            cm->setName(name);
                        }
                    }else{
                        name = "Other side not connected";
                        cm->setName(name);
                    }
                }

                QString type = "";
                if(cm->getOtherSide()->isConnected()){
                    if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                        type =cm->getOtherSide()->getConnectedConnector()->getModel()->getDataType();
                    }else{
                        type ="UNKNOWN";
                    }
                }else{
                    type = "UNKNOWN";
                }

                textItem->setPlainText(QString::number(connector->getId())+" "+type+" "+name);
                textItem->setDefaultTextColor(Qt::white);
                QPointF p2;
                if(textItem->boundingRect().width()>maxw) maxw = textItem->boundingRect().width();
                p2.setX(p.x()-20-textItem->boundingRect().width());
                p2.setY(p.y()-20);
                textItem->setPos(p2);
                textItem->setDefaultTextColor(Qt::white);
            }
        }

        maxw = rayon+20;
        ellipse1->setRect(-maxw/2-5,-maxw/2-5, maxw+10,maxw+10);
        ellipse2->setRect(-maxw/2,-maxw/2, maxw,maxw);


        //        if(this->getModel()->getInformation().compare("")!=0){
        //            informations  = new QGraphicsTextItem(this);
        //            informations->setTextInteractionFlags(Qt::TextBrowserInteraction);
        //            informations->setHtml(this->getModel()->getInformation());
        //            informations->setDefaultTextColor(Qt::white);
        //            informations->setPos(-informations->boundingRect().width()/2,this->rayon/2+20);
        //        }else{
        //            informations  = new QGraphicsTextItem(this);
        //            informations->setTextInteractionFlags(Qt::TextBrowserInteraction);
        //            informations->setHtml("This information is empty. Contact the dictionnary owner for more informations.<br>Go to <a href=\"www.shinoe.com\">www.shinoe.com</a>.");
        //            informations->setDefaultTextColor(Qt::white);
        //            informations->setPos(-informations->boundingRect().width()/2,this->rayon/2+20);
        //        }
        //        c.setAlpha(0);
        //        docrect1 = new QGraphicsRectItem(informations);
        //        docrect1->setBrush(c);
        //        docrect1->setRect(informations->boundingRect());
        //        docrect2 = new QGraphicsRectItem(informations);
        //        docrect2->setBrush(c);
        //        docrect2->setRect(informations->boundingRect());

    }else{
        foreach(QGraphicsTextItem* textItem, itemDocList){
            this->scene()->removeItem(textItem);
        }
        //        this->scene()->removeItem(docrect2);
        //        this->scene()->removeItem(docrect1);
        //        this->scene()->removeItem(informations);
        if(ellipse1!=0) this->scene()->removeItem(ellipse1);
        if(ellipse2!=0) this->scene()->removeItem(ellipse2);
        ellipse1 = 0;
        ellipse2 = 0;
        mapDoc.clear();
        itemDocList.clear();
    }
}

void CGMulti::setParentView(CGMulti* parentv){
    this->parentView = parentv;
}

void CGMulti::renderModel(){
    if(mode == TOP){
        this->setActive(false);
        this->setEnabled(false);
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        setFlag(QGraphicsItem::ItemIsFocusable, false);
        this->buildViews();
    }else if(mode == INNER){
        this->rayon = 1000;
        setFlag(QGraphicsItem::ItemIsMovable, false);
        setFlag(QGraphicsItem::ItemIsSelectable, false);
        setFlag(QGraphicsItem::ItemIsFocusable, false);
        this->setEnabled(false);
        this->setActive(false);
        this->setSelected(false);
        this->setZValue(-1500);
    }else{
        this->rayon = 120;
        setFlag(QGraphicsItem::ItemIsMovable, true);
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setFlag(QGraphicsItem::ItemIsFocusable, false);
        this->setZValue(-700);
    }
}

void CGMulti::buildViews(){
    outerView = new CGMulti(multiMenu,connectorMenu,0,0); //null if parent is null
    outerView->setParentView(this);
    outerView->setMode(OUTER);
    outerView->setModel(model);
    outerView->renderModel();
    outerView->setPos(this->pos());
    outerView->setToolTip(model->getInformation());
    innerView = new CGMulti(multiMenu,connectorMenu,0,model->getScene());
    innerView->setParentView(this);
    innerView->setMode(INNER);
    innerView->setModel(model);
    innerView->renderModel();
}

void CGMulti::setRayon(int rayon){
    this->rayon = rayon;
}

void CGMulti::setError(QString error){
    this->errormsg = error;
}

void CGMulti::updateState(COperator::State state){
    if(errormsg.compare("") != 0){
        outerColor = Qt::red;
        this->update();
        QString n = "ERROR: "+errormsg+"\n";
        n = n+this->model->getInformation();
        this->setToolTip(n);
    }else{
        if(state == COperator::RUNNING){
            outerColor = Qt::yellow;
        }else if(state == COperator::NOTRUNNABLE){
            outerColor = Qt::gray;
        }else if(state == COperator::RUNNABLE){
            outerColor = Qt::green;
        }
        this->update();
        QString n = this->model->getInformation();

        this->setToolTip(n);
    }
    foreach(CGConnector*c,this->getConnectorsList()){
        if(c->isConnected()){
            QColor col = c->getConnectedConnector()->getColor();
            c->setColor(col);
            c->update();
        }
    }
}

QList<CGConnector*> CGMulti::getConnectorsList(){
    return connectorL;
}

bool CGMulti::belongToThis(CGConnector* conn){
    foreach(CGOperatorModel*ope, this->model->getOperatorModelList()){
        foreach(CGConnectorModel* connector,ope->getConnectorsList()){
            CGConnector* vc = ope->getView()->getConnectorById(connector->getId());
            if(vc == conn){
                return true;
            }
        }
    }
    foreach(CGCompositionModel*compo, this->model->getCompositionModelList()){
        foreach(CGConnector* vc,compo->getOuterView()->getConnectorsList()){
            if(vc == conn){
                return true;
            }
        }
    }
    return false;
}

CGMulti* CGMulti::getParentView(){
    return this->parentView;
}

void CGMulti::addConnection(CGConnector* inner, CGConnector* outer){
    QString mode;
    if(inner->getSide().compare("IN") == 0){
        mode = "OUTIN";
    }else{
        mode = "INOUT";
    }
    if(this->getParentView() == NULL){
        CGMultiConnector* multic = addConnector(mode);
        CGConnectConnector* connect1 = new CGConnectConnector();
        connect1->setStartItem(multic->getOuterConnector());
        connect1->setEndItem(outer);
        connect1->doCommand();
        CGConnectConnector* connect2 = new CGConnectConnector();
        connect2->setStartItem(multic->getInnerConnector());
        connect2->setEndItem(inner);
        connect2->doCommand();
    }else{
        this->getParentView()->addConnection(inner, outer);
    }
}

void CGMulti::processAllConnectorsPos(){
    countin = 0;
    countout = 0;
    int maxin = 0;
    int maxout = 0;
    foreach(CGConnector* c, connectorL){
        if(c->getSide().compare("IN") == 0){
            maxin++;
        }else{
            maxout++;
        }
    }
    foreach(CGConnector* c, connectorL){
        CGMultiConnector* multiconnector = qgraphicsitem_cast<CGMultiConnector *>(c);
        if(c->getSide().compare("IN") == 0){
            countin++;
        }else{
            countout++;
        }
        if(multiconnector->pos()==QPointF(0,0)){
            multiconnector->setInitialPos(maxin,maxout,countin,countout);
        }else{
            multiconnector->update();
        }
        multiconnector->updatePosition();
        multiconnector->rePosition();
    }
}

void CGMulti::addConnector(CGConnector* connector){
    connectorL.append(connector);
    if(connectorL.size()>4){
        rayon = rayon + 10;
    }
    foreach(CGConnector* c, connectorL){
        c->setRayon(rayon);
    }
    this->processAllConnectorsPos();
    this->update();
}

int CGMulti::getRayon(){
    return rayon;
}

CGMultiConnector* CGMulti::addConnector(QString mode){
    CGMultiConnector* multic = new CGMultiConnector(connectorMenu,mode,innerView,outerView);
    multiconnectorsL.append(multic);
    CGMultiConnector* inconnector = multic->getInnerConnector();
    this->getInnerView()->addConnector(inconnector);
    inconnector->setRayon(this->innerView->getRayon());
    inconnector->setZValue(-900);
    inconnector->setId(nbConnectors);

    CGMultiConnector* outconnector = multic->getOuterConnector();
    this->getOuterView()->addConnector(outconnector);
    outconnector->setZValue(-900);
    outconnector->setId(nbConnectors);
    nbConnectors++;
    return multic;
}

void CGMulti::setModel(CGCompositionModel* model){
    this->model = model;
}

CGCompositionModel* CGMulti::getModel(){
    return model;
}

int CGMulti::getMode(){
    return mode;
}

void CGMulti::setMode(MultiMode mode){
    this->mode = mode;
}

void CGMulti::disconnect(){
    foreach (CGMultiConnector* c, this->multiconnectorsL) {
        if(c->getConnectedConnector() != NULL){
            CGDisconnectConnector* disc = new CGDisconnectConnector();
            disc->setConnector(c);
            disc->doCommand();
        }
    }
    foreach (CGConnector* c, this->connectorL) {
        if(c->getConnectedConnector() != NULL){
            CGDisconnectConnector* disc = new CGDisconnectConnector();
            disc->setConnector(c);
            disc->doCommand();
        }
    }
}

void CGMulti::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget *  ){
    //tool tip
    //

    CLogger::getInstance()->log("PAINT",CLogger::DEBUG,"CGMulti::PAINT");
    if(mode != TOP){
        foreach (CGConnector *connector, connectorL) {
            connector->setRayon(this->rayon);
            connector->updatePosition();
            if(connector->isConnected()){
                connector->getConnectedConnector()->updatePosition();
            }
        }
        if(compatibleMode){
            if(mode == INNER){
                QColor c = Qt::yellow;
                c.setAlpha(150);
                painter->setBrush(c);
                painter->drawEllipse(-(10+rayon+50)/2, -(10+rayon+50+2)/2, rayon+50+10, rayon+50+10); //origin on circle center
            }else{
                QColor c = Qt::yellow;
                c.setAlpha(150);
                painter->setBrush(c);
                painter->drawEllipse(-(10+rayon+10)/2, -(10+rayon+10+2)/2, rayon+10+10, rayon+10+10); //origin on circle center
            }
        }

        if(this->isUp){
            QColor c = Qt::cyan;
            c.setAlpha(150);
            painter->setBrush(c);
            painter->drawEllipse(-(10+rayon+100)/2, -(10+rayon+100+2)/2, rayon+100+10, rayon+100+10); //origin on circle center
        }else if(this->isSelected()){
            QPen pen = painter->pen();
            pen.setWidth(3);
            painter->setPen(pen);
            painter->setBrush(Qt::white);
        }else if(this->hasFocus()){
            QPen pen = painter->pen();
            pen.setWidth(3);
            painter->setPen(pen);
            painter->setBrush(Qt::red);
        }else{
            painter->setBrush(outerColor);
            painter->setPen(Qt::SolidLine);
        }
        if(mode == INNER){
            painter->setBrush(Qt::white);
            painter->drawEllipse(-(10+rayon)/2, -(10+rayon+2)/2, rayon+10, rayon+10); //origin on circle center
        }
        painter->drawEllipse(-rayon/2, -rayon/2, rayon, rayon); //origin on circle center
        if(mode == INNER){
            painter->setBrush(Qt::black);
            painter->drawEllipse(-(rayon-100)/2, -(rayon-100)/2, rayon-100, rayon-100); //origin on circle center
        }
        if(mode == OUTER){
            QString name2(this->getModel()->getName());

            QFont f("times new roman,utopia");
            f.setStyleStrategy(QFont::ForceOutline);
            f.setPointSize(6);
            f.setStyleHint(QFont::Times);
            if(name2.size() > 12){
                name2.resize(9);
                name2.append("...");
            }
            painter->setFont(f);

            int c = -30;
            if(name2.size() < 12){
                int unity = 5;
                int width = name2.size()*unity;
                c = -width/2;
            }
            painter->drawText(c, -10, QString::number(this->getModel()->getId()));
            painter->drawText(c, 0, name2);
        }
    }
    this->updateDocs();
}

QRectF CGMulti::boundingRect() const{
    if(isUp){
        if(mode == INNER){
            return QRectF(-(10+rayon+10)/2, -(10+rayon+10+2)/2, rayon+10+10, rayon+10+10); //origin on circle center
        }else{
            return QRectF(-(10+rayon+50)/2, -(10+rayon+50+2)/2, rayon+50+10, rayon+50+10); //origin on circle center
        }
    }else if(compatibleMode){
        if(mode == INNER){
            return QRectF(-(10+rayon+10)/2, -(10+rayon+10+2)/2, rayon+10+10, rayon+10+10); //origin on circle center
        }else{
            return QRectF(-(10+rayon+10)/2, -(10+rayon+10+2)/2, rayon+10+10, rayon+10+10); //origin on circle center
        }
    }else{
        if(mode == INNER){
            return QRectF(-(10+rayon+50)/2, -(10+rayon+50+2)/2, rayon+50+10, rayon+50+10); //origin on circle center
        }else{
            return QRectF(-(rayon)/2, -(rayon)/2, rayon, rayon); //origin on circle center
        }
    }
}

void CGMulti::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *  ){
    if(mode == OUTER){
        CGChangeView* command = new CGChangeView();
        command->setCompositionModel(this->model);
        command->doCommand();
    }
}

void CGMulti::setIsCompatibleMode(bool compatible){
    this->compatibleMode = compatible;
    this->update();
}

void CGMulti::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    setSelected(true);
    multiMenu->exec(event->screenPos());
}

QVariant CGMulti::itemChange(GraphicsItemChange change, const QVariant &value){
    if(!this->isSelected()){
        isUp = false;
    }
    if (change == QGraphicsItem::ItemPositionChange) {
        foreach (CGConnector *connector, connectorL) {
            connector->updatePosition();
            if(connector->isConnected()){
                connector->getConnectedConnector()->updatePosition();
            }
        }
    }
    if(CGLayoutManager::getInstance()->getCurrentLayout())CGLayoutManager::getInstance()->getCurrentLayout()->update();
    if(CGCompositionManager::getInstance()->getCurrentComposerScene())CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
    return QGraphicsItem::itemChange(change, value);;
}

QPointF CGMulti::processInnerCenter(){
    return QPointF(0,0);
}

void CGMulti::setIsUp(bool b){
    this->isUp = b;
}

int CGMulti::processInnerRay(bool ){
    //recherche du point le plus éloigné du centre
    int distance = 0;
    foreach(CGOperatorModel*ope, this->getModel()->getOperatorModelList()){
        int distance2 = sqrt((ope->getView()->scenePos().x())*(ope->getView()->scenePos().x())+(ope->getView()->scenePos().y())*(ope->getView()->scenePos().y()));
        if(distance2>distance) distance = distance2;
    }
    foreach(CGCompositionModel*compo, this->getModel()->getCompositionModelList()){
        int distance2 = sqrt((compo->getOuterView()->scenePos().x())*(compo->getOuterView()->scenePos().x())+(compo->getOuterView()->scenePos().y())*(compo->getOuterView()->scenePos().y()));
        if(distance2>distance) distance = distance2;
    }
    int ray = 2*(distance) + 200;

    if(ray>1000){rayon = ray;}else{rayon=1000;}
    foreach(CGConnector* c, connectorL){
        c->setRayon(rayon);
    }
    this->processAllConnectorsPos();
    return rayon;
}

CGMulti* CGMulti::getOuterView(){
    return outerView;
}

CGMulti* CGMulti::getInnerView(){
    return innerView;
}

QPointF CGMulti::getOuterCenter(){
    return QPointF(0,0);
}

int CGMulti::getOuterRay(){
    return 0;
}

CGMultiConnector::CGMultiConnector(QMenu *contextMenu, QString mode,
                                   QGraphicsItem *inner, QGraphicsItem *outer, QGraphicsScene *scene)
    : CGConnector(contextMenu,"IN",NULL,0, 0){
    //TOP
    if(mode.compare("INOUT") == 0){
        this->innerConnector = new CGMultiConnector(contextMenu,"IN",inner,scene);
        this->outerConnector = new CGMultiConnector(contextMenu,"OUT",outer,scene);
    }else{
        this->innerConnector = new CGMultiConnector(contextMenu,"OUT",inner,scene);
        this->outerConnector = new CGMultiConnector(contextMenu,"IN",outer,scene);
    }
    this->innerConnector->setIsInner(true);
    this->innerConnector->setZValue(-100);
    this->innerConnector->setRayon(1000);
    this->outerConnector->setZValue(-100);
    this->innerConnector->setParentConnector(this);
    this->outerConnector->setParentConnector(this);
    this->outerConnector->setRayon(90);
    teta = 0;
    this->isInner = false;
    this->name = "";
}

void CGMultiConnector::setName(QString name){
    this->name = name;
}

QString CGMultiConnector::getName(){
    return name;
}

void CGMultiConnector::setParentConnector(CGMultiConnector* connector){
    this->parentConnector = connector;
}

CGMultiConnector::CGMultiConnector(QMenu *contextMenu, QString side,
                                   QGraphicsItem *parent, QGraphicsScene *scene)
    : CGConnector(contextMenu,side,NULL,parent, scene)
{
    //CHILD
    this->isInner = false;
    teta = 0;
}

void CGMultiConnector::toolT(){
    QString type = "";
    if(!isInner){
        if(this->getName().compare("")!=0){
            name = this->getName();
        }else{
            if(this->getOtherSide()->isConnected()){
                if(this->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                    this->getOtherSide()->getConnectedConnector()->getModel()->getName();
                }else{
                    name = "Can't find name";
                }
            }else{
                name = "Other side not connected";
            }
        }

        QString type = "";
        if(this->getOtherSide()->isConnected()){
            if(this->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
                type =this->getOtherSide()->getConnectedConnector()->getModel()->getDataType();
            }else{
                type ="UNKNOWN";
            }
        }else{
            type = "UNKNOWN";
        }
    }
    this->setToolTip(type+" "+name);
}

CGMulti* CGMultiConnector::getMulti(){
    return multi;
}

void CGMultiConnector::setInitialPos(int countinmax,int countoutmax,int countin, int countout){
    if(!this->isConnected()){
        if(isInner){
            if(this->getSide().compare("OUT")==0){
                double angle = (countout-1)*3.14/(2*countoutmax);
                double x = -cos(angle)*rayon/2;
                double y = sin(angle)*rayon/2;
                this->setPos(x,y);
                teta = angle;
            }
            else{
                double angle = (countin-1)*3.14/(2*countinmax);
                double x = cos(angle)*rayon/2;
                double y = sin(angle)*rayon/2;
                this->setPos(x,y);
                teta = angle;
            }
        }else{
            if(this->getSide().compare("IN")==0){
                double angle = (countin-1)*3.14/(2*countinmax);
                double x = -cos(angle)*((rayon-20)+20)/2;
                double y = sin(angle)*((rayon-20)+20)/2;
                this->setPos(x,y);
                teta = angle;
            }
            else{
                double angle = (countout-1)*3.14/(2*countoutmax);
                double x = cos(angle)*((rayon-20)+20)/2;
                double y = sin(angle)*((rayon-20)+20)/2;
                this->setPos(x,y);
                teta = angle;
            }
        }
    }
}

void CGMultiConnector::updatePosition(){
    if(CGProject::getInstance()->getConnectorOrderMode().compare("HARD") != 0){
        //Link plugs position inner & outter
        /*
      Rules:
      * if inner & connected, outer must get his angle
      * else if outer & connected, inner get his angle
      */
        if(isInner){
            if(isConnected()){
                QPointF posM = this->parentItem()->mapFromScene(this->connectedConnector->scenePos());
                /*
                    homothécie: OM * length(OP)/lenght(OM)= OP
                    avec P (px,py) le point sur le cercle et O (0,0) le centre du cercle et M(x,y) le point distant
                    length(OM)/length(OP):
                    length(OP) = rayon de ton cercle
                    length(OM) = sqrt(x^2+y^2)

                    or OM * length(OM)/lenght= OP
                    d'où,
                    px = x * (R/sqrt(x^2+y^2))
                    py = y * (R/sqrt(x^2+y^2))
                 */
                int i=0;
                if(this->isInner){
                    i=100;
                }
                double r = (rayon-i)/2;
                double carre = posM.x()*posM.x()+posM.y()*posM.y();
                double px = posM.x() * (r/sqrt(carre));
                double py = posM.y() * (r/sqrt(carre));
                this->setPos(px,py);
                //record angle
                teta = acos(px/r)*(SCD::sgn(py));
            }else if(this->getOtherSide()->isConnected()){
                //other side angle
                teta = this->getParentConnector()->getOuterConnector()->getTeta();
                double x = cos(teta)*((rayon-20)+20)/2;
                double y = sin(teta)*((rayon-20)+20)/2;
                this->setPos(x,y);
            }
        }else{
            if(isConnected() && !this->getOtherSide()->isConnected()){
                QPointF posM = this->parentItem()->mapFromScene(this->connectedConnector->scenePos());
                /*
                    homothécie: OM * length(OP)/lenght(OM)= OP
                    avec P (px,py) le point sur le cercle et O (0,0) le centre du cercle et M(x,y) le point distant
                    length(OM)/length(OP):
                    length(OP) = rayon de ton cercle
                    length(OM) = sqrt(x^2+y^2)

                    or OM * length(OM)/lenght= OP
                    d'où,
                    px = x * (R/sqrt(x^2+y^2))
                    py = y * (R/sqrt(x^2+y^2))
                 */
                int i=0;
                if(this->isInner){
                    i=100;
                }
                double r = (rayon-i)/2;
                double carre = posM.x()*posM.x()+posM.y()*posM.y();
                double px = posM.x() * (r/sqrt(carre));
                double py = posM.y() * (r/sqrt(carre));
                this->setPos(px,py);
                //record angle
                teta = acos(px/r)*(SCD::sgn(py));
            }else if(this->getOtherSide()->isConnected()){
                //other side angle
                //teta = argcos(x/rayon)*(signe(y))
                teta = this->getParentConnector()->getInnerConnector()->getTeta();
                double x = cos(teta)*((rayon-20)+20)/2;
                double y = sin(teta)*((rayon-20)+20)/2;
                this->setPos(x,y);
            }
        }
    }
}

double CGMultiConnector::getTeta(){
    return teta;
}

CGConnector* CGMultiConnector::getOtherSide(){
    if(this->getParentConnector()!=NULL){
        if(this->getParentConnector()->getInnerConnector() == this){
            return this->getParentConnector()->getOuterConnector();
        }else{
            return this->getParentConnector()->getInnerConnector();
        }
    }
    return NULL;
}

void CGMultiConnector::setIsInner(bool isInner){
    this->isInner = isInner;
}

void CGMultiConnector::mousePressEvent(QGraphicsSceneMouseEvent *event){
    if(event->button() == Qt::LeftButton){
        if(controlMode){
            isControlConnected = true;
            CControl* control = CGCompositionManager::getInstance()->getCurrentComposerScene()->getControlToConnect();
            CGConnector* connectortoconnect = CGCompositionManager::getInstance()->getCurrentComposerScene()->getConnectorToConnect();
            //MOCK CONNECTION, real connection is in the other side
            this->connectedLayoutConnector = qgraphicsitem_cast<CGLayoutItemConnector *>(connectortoconnect);;
            //REAL CONNECTION
            CGCommandManager::getInstance()->startAction("CONNECT");
            CGConnectConnector* connect = new CGConnectConnector();
            connect->setControl(control);
            connect->setControlConnector(connectortoconnect);
            connect->setConnector(this->getOtherSide()->getConnectedConnector());
            connect->doCommand();
            connectortoconnect->parentItem()->setSelected(false);
            CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
            CGCommandManager::getInstance()->endAction("CONNECT");
        }
    }
}

void CGMultiConnector::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget* ){
    CLogger::getInstance()->log("PAINT",CLogger::DEBUG,"CGMultiConnector::PAINT");
    int i=0;
    if(this->isInner){
        i=40;
        this->setZValue(-500);
        if(compatibleMode){
            QColor c = Qt::yellow;
            c.setAlpha(150);

            painter->setBrush(c);
            painter->drawEllipse(-60, -60, 120, 120);
        }
        if(isMoving){
            QColor c = Qt::darkCyan;
            c.setAlpha(150);

            painter->setBrush(c);
            painter->drawEllipse(-60, -60, 120, 120);
        }
    }else{
        if(compatibleMode || controlMode){
            QColor c = Qt::yellow;
            c.setAlpha(150);

            painter->setBrush(c);
            painter->drawEllipse(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
        }
        if(isMoving){
            QColor c = Qt::darkCyan;
            c.setAlpha(150);

            painter->setBrush(c);
            painter->drawEllipse(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
        }
        if(isControlConnected
                || (this->getOtherSide()->isConnected()
                    && this->getOtherSide()->getConnectedConnector()->getIsControlConnected())){
            QColor c = Qt::white;
            c.setAlpha(150);
            painter->setBrush(c);
            painter->drawEllipse(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
        }
    }
    if(isIn()){
        painter->setPen(outerColor);
        painter->setBrush(outerColor);
        painter->drawEllipse(-(crayon+i)/2, -(crayon+i)/2, crayon+i, crayon+i);
        painter->setBrush(innerColor);
        painter->drawEllipse(-(crayon-10+i)/2, -(crayon-10+i)/2, crayon+i-10, crayon+i-10);
    }else{
        painter->setPen(innerColor);
        painter->setBrush(innerColor);
        painter->drawEllipse(-(crayon+i)/2, -(crayon+i)/2, crayon+i, crayon+i);
        painter->setBrush(outerColor);
        painter->drawEllipse(-(crayon-10+i)/2, -(crayon-10+i)/2, crayon+i-10, crayon+i-10);
    }
}

QRectF CGMultiConnector::boundingRect() const{
    int i=0;
    if(this->isInner){
        i=40;
        if(compatibleMode || isMoving){
            return QRectF(-60, -60, 120, 120);
        }
    }
    if(compatibleMode || isMoving){
        return QRectF(-(crayon+20)/2, -(crayon+20)/2, crayon+20, crayon+20);
    }
    return QRectF(-(crayon+i)/2, -(crayon+i)/2, crayon+i, crayon+i);
}

CGMultiConnector* CGMultiConnector::getInnerConnector(){
    return this->innerConnector;
}

CGMultiConnector* CGMultiConnector::getOuterConnector(){
    return this->outerConnector;
}

CGMultiConnector* CGMultiConnector::getParentConnector(){
    return this->parentConnector;
}

void CGMultiConnector::setId(int id){
    this->id = id;
}

int CGMultiConnector::getId(){
    return this->id;
}

CGShape::CGShape(QMenu *contextMenu,
                 QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    persistent = false;
    mode = NA;
    model = new CGShapeModel();
    myContextMenu = contextMenu;

    rect = QRect(-100, -100, 200, 200);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setAcceptHoverEvents(true);

    textItem = new QGraphicsTextItem(this);
    textItem->setPos(100,-100);

    //8 bulls
    ellipse1 = new CGEllipse(this,scene);
    ellipse2 = new CGEllipse(this,scene);
    ellipse3 = new CGEllipse(this,scene);
    ellipse4 = new CGEllipse(this,scene);
    ellipse5 = new CGEllipse(this,scene);
    ellipse6 = new CGEllipse(this,scene);
    ellipse7 = new CGEllipse(this,scene);
    ellipse8 = new CGEllipse(this,scene);

    ellipse1->setRect(-5,-5,10,10);
    ellipse1->setBrush(Qt::gray);
    ellipse8->setRect(this->rect.width()-5,this->rect.height()-5,10,10);
    ellipse8->setBrush(Qt::gray);

    ellipse2->setRect(-5,this->rect.height()/2-5,10,10);
    ellipse2->setBrush(Qt::gray);
    ellipse7->setRect(this->rect.width()-5,this->rect.height()/2-5,10,10);
    ellipse7->setBrush(Qt::gray);

    ellipse4->setRect(this->rect.width()/2-5,-5,10,10);
    ellipse4->setBrush(Qt::gray);
    ellipse5->setRect(this->rect.width()/2-5,this->rect.height()-5,10,10);
    ellipse5->setBrush(Qt::gray);

    ellipse6->setRect(this->rect.width()-5,-5,10,10);
    ellipse6->setBrush(Qt::gray);
    ellipse3->setRect(-5,this->rect.height()-5,10,10);
    ellipse3->setBrush(Qt::gray);

    ellipse1->hide();
    ellipse2->hide();
    ellipse3->hide();
    ellipse4->hide();
    ellipse5->hide();
    ellipse6->hide();
    ellipse7->hide();
    ellipse8->hide();

    shapeColor = Qt::white;
}

void CGShape::setPersistent(bool per){
    this->persistent = per;
}

bool CGShape::getPersistent(){
    return persistent;
}

void CGText::setPersistent(bool per){
    this->persistent = per;
}

bool CGText::getPersistent(){
    return persistent;
}

CGShapeModel* CGShape::getModel(){
    return this->model;
}

QRectF CGShape::boundingRect() const{
    int x = rect.x();
    int y = rect.y();
    int w = rect.width();
    int h = rect.height();
    return QRectF(x,y,w,h);
}

void CGShape::setRect(QRectF rect){
    this->rect = rect;
}

CGShape* CGShape::clone(){
    CGShape* s = new CGShape(0,0,0);
    s->setRect(this->rect);
    s->setPos(this->pos());
    s->setColor(this->shapeColor);
    return s;
}

void CGShape::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    if(scene() == CGCompositionManager::getInstance()->getCurrentComposerScene()){
        CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMenu()->exec(event->screenPos());
    }else{
        CGLayoutManager::getInstance()->getControlMenu()->exec(event->screenPos());
    }
}

QColor CGShape::getColor(){
    return shapeColor;
}


void CGShape::setColor(QColor color){
    shapeColor = color;
}

void CGShape::paint(QPainter * painter, const QStyleOptionGraphicsItem * , QWidget *  )
{
    CLogger::getInstance()->log("PAINT",CLogger::DEBUG,"CGShape::PAINT");
    painter->setBrush(shapeColor);
    painter->drawRect(rect);

    ellipse1->setRect(-5+rect.x(),-5+rect.y(),10,10);
    ellipse8->setRect(this->rect.width()-5+rect.x(),this->rect.height()-5+rect.y(),10,10);
    ellipse2->setRect(-5+rect.x(),this->rect.height()/2-5+rect.y(),10,10);
    ellipse7->setRect(this->rect.width()-5+rect.x(),this->rect.height()/2-5+rect.y(),10,10);
    ellipse4->setRect(this->rect.width()/2-5+rect.x(),-5+rect.y(),10,10);
    ellipse5->setRect(this->rect.width()/2-5+rect.x(),this->rect.height()-5+rect.y(),10,10);
    ellipse6->setRect(this->rect.width()-5+rect.x(),-5+rect.y(),10,10);
    ellipse3->setRect(-5+rect.x(),this->rect.height()-5+rect.y(),10,10);

}

QVariant CGShape::itemChange(GraphicsItemChange change,
                             const QVariant &value)
{
    if (change == ItemSelectedChange && scene()) {
        if(!isSelected()){
            ellipse1->show();
            ellipse2->show();
            ellipse3->show();
            ellipse4->show();
            ellipse5->show();
            ellipse6->show();
            ellipse7->show();
            ellipse8->show();
        }else{
            ellipse1->hide();
            ellipse2->hide();
            ellipse3->hide();
            ellipse4->hide();
            ellipse5->hide();
            ellipse6->hide();
            ellipse7->hide();
            ellipse8->hide();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}


void CGShape::hoverEnterEvent ( QGraphicsSceneHoverEvent *  ){

}

void CGShape::hoverLeaveEvent ( QGraphicsSceneHoverEvent *  ){
    if(scene() == CGCompositionManager::getInstance()->getCurrentComposerScene()){
        CGCompositionManager::getInstance()->getCurrentComposerScene()->reloadCursor();
    }else{
        CGLayoutManager::getInstance()->getCurrentLayout()->reloadCursor();
    }
}

int CGShape::getMode(){
    return mode;
}

void CGShape::resize(QPointF toscenepoint){
    toscenepoint = this->mapFromScene(toscenepoint);
    if(this->getMode() == CGShape::BottomLeft){
        rect.setX(toscenepoint.x());
        rect.setBottom(toscenepoint.y());
        rect.setLeft(toscenepoint.x());
    }else if(this->getMode() == CGShape::TopLeft){
        rect.setY(toscenepoint.y());
        rect.setX(toscenepoint.x());
        rect.setTop(toscenepoint.y());
        rect.setLeft(toscenepoint.x());
    }else if(this->getMode() == CGShape::TopRight){
        rect.setRight(toscenepoint.x());
        rect.setTop(toscenepoint.y());
        rect.setY(toscenepoint.y());
    }else if(this->getMode() == CGShape::BottomRight){
        rect.setRight(toscenepoint.x());
        rect.setBottom(toscenepoint.y());
    }else if(this->getMode() == CGShape::VerBottom){
        rect.setBottom(toscenepoint.y());
    }else if(this->getMode() == CGShape::VerTop){
        rect.setTop(toscenepoint.y());
        rect.setY(toscenepoint.y());
    }else if(this->getMode() == CGShape::HorLeft){
        rect.setLeft(toscenepoint.x());
    }else if(this->getMode() == CGShape::HorRight){
        rect.setRight(toscenepoint.x());
    }
    if(rect.width()<1){//TODO minimum size managment
        rect.setWidth(1);
    }
    if(rect.height()<1){//TODO minimum size managment
        rect.setHeight(1);
    }
    scene()->update();
}

void CGShape::hoverMoveEvent ( QGraphicsSceneHoverEvent * event ){
    if(isSelected()){
        if(scene()->itemAt(event->scenePos()) == ellipse1){
            setCursor(Qt::SizeFDiagCursor);
            this->mode = TopLeft;
        }else if(scene()->itemAt(event->scenePos()) == ellipse8){
            setCursor(Qt::SizeFDiagCursor);
            this->mode = BottomRight;
        }else if(scene()->itemAt(event->scenePos()) == ellipse2){
            setCursor(Qt::SizeHorCursor);
            this->mode = HorLeft;
        }else if(scene()->itemAt(event->scenePos()) == ellipse7){
            setCursor(Qt::SizeHorCursor);
            this->mode = HorRight;
        }else if(scene()->itemAt(event->scenePos()) == ellipse3){
            setCursor(Qt::SizeBDiagCursor);
            this->mode = BottomLeft;
        }else if(scene()->itemAt(event->scenePos()) == ellipse6){
            setCursor(Qt::SizeBDiagCursor);
            this->mode = TopRight;
        }else if(scene()->itemAt(event->scenePos()) == ellipse4){
            setCursor(Qt::SizeVerCursor);
            this->mode = VerTop;
        }else if(scene()->itemAt(event->scenePos()) == ellipse5){
            setCursor(Qt::SizeVerCursor);
            this->mode = VerBottom;
        }else{
            setCursor(Qt::ArrowCursor);
        }
    }
}

CGEllipse::CGEllipse(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsEllipseItem(parent, scene){

}

CGText::CGText(QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsTextItem(parent, scene)
{
    persistent = false;
    model = new CGTextModel();
    setFlag(QGraphicsItem::ItemIsMovable);
    setFlag(QGraphicsItem::ItemIsSelectable);
}

QVariant CGText::itemChange(GraphicsItemChange change,
                            const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedHasChanged)
        emit selectedChange(this);
    return value;
}

void CGText::focusOutEvent(QFocusEvent *event)
{
    setTextInteractionFlags(Qt::NoTextInteraction);
    emit lostFocus(this);
    QGraphicsTextItem::focusOutEvent(event);
}

void CGText::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (textInteractionFlags() == Qt::NoTextInteraction)
        setTextInteractionFlags(Qt::TextEditorInteraction);
    QGraphicsTextItem::mouseDoubleClickEvent(event);
}

CGTextModel* CGText::getModel(){
    return model;
}

CGText* CGText::clone(){
    CGText* t = new CGText(0,0);
    QString s = this->toHtml();
    t->setHtml(s);
    t->setDefaultTextColor(this->defaultTextColor());
    t->setPos(this->scenePos());
    return t;
}

void CGText::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    setSelected(true);
    if(scene() == CGCompositionManager::getInstance()->getCurrentComposerScene()){
        CGCompositionManager::getInstance()->getCurrentComposerScene()->getOperatorMenu()->exec(event->screenPos());
    }else{
        CGLayoutManager::getInstance()->getControlMenu()->exec(event->screenPos());
    }
}

const qreal Pi = 3.14;

CGArrow::CGArrow(CGConnector *startItem, CGConnector *endItem,
                 QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsLineItem(parent, scene)
{
    myStartItem = startItem;
    myEndItem = endItem;
    setFlag(QGraphicsItem::ItemIsSelectable, false);
    myColor = Qt::black;
    isMulti = false;
    out = false;
    setPen(QPen(myColor, 2, Qt::SolidLine));
}

QRectF CGArrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath CGArrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void CGArrow::setIsMulti(bool isMulti, bool out){
    this->isMulti = isMulti;
    this->out = out;
    if(out){
        CGConnector *myEndItemSave = myEndItem;
        myEndItem = myStartItem;
        myStartItem = myEndItemSave;
    }
}


void CGArrow::updatePosition()
{
    QLineF line(mapFromItem(myStartItem, 0, 0), mapFromItem(myEndItem, 0, 0));
    setLine(line);

    update();

}

void CGArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
                    QWidget *)
{
    CLogger::getInstance()->log("PAINT",CLogger::DEBUG,"CGArrow::PAINT");
    if (myStartItem->collidesWithItem(myEndItem))
        return;

    QPen myPen = pen();
    myPen.setColor(myColor);
    painter->setPen(myPen);
    painter->setBrush(myColor);

    QPointF startPoint = myEndItem->parentItem()->mapToScene(myEndItem->pos());
    QPointF endPoint = myStartItem->parentItem()->mapToScene(myStartItem->pos());
    QLineF l( startPoint, endPoint);
    CGConnector * cend = qgraphicsitem_cast<CGConnector *> (myEndItem);
    if(cend==0) cend = qgraphicsitem_cast<CGMultiConnector *> (myEndItem);
    if(isMulti) l.setLength(l.length()-32);
    setLine(l);

    QLineF myLine = line();
    painter->drawLine(myLine);

    double angle = ::acos(line().dx() / line().length());

    if(isMulti && cend->getSide().compare("IN") == 0){
        QLineF l2(endPoint, startPoint);
        angle = ::acos(l2.dx() / l2.length()) ;
        if (l2.dy() >= 0)
            angle = (Pi * 2) - angle;
        QPointF middle((l2.p1().x()+l2.p2().x())/2,(l2.p1().y()+l2.p2().y())/2);

        QPointF arrowP1 = middle + QPointF(sin(angle + Pi / 3) * -20,
                                           cos(angle + Pi / 3) * -20);
        QPointF arrowP2 = middle + QPointF(sin(angle + Pi - Pi / 3) * -20,
                                           cos(angle + Pi - Pi / 3) * -20);
        arrowHead.clear();
        arrowHead << middle << arrowP1 << arrowP2;
        //! [6] //! [7]
        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        //        QLineF l1(middle, arrowP1);
        //        QLineF l2(middle, arrowP2);

        //        painter->drawLine(l1);
        //        painter->drawLine(l2);

        if (isSelected()) {
            painter->setPen(QPen(myColor, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0,-8.0);
            painter->drawLine(myLine);
        }
    }else{
        if (line().dy() >= 0)
            angle = (Pi * 2) - angle;
        QPointF middle((line().p1().x()+line().p2().x())/2,(line().p1().y()+line().p2().y())/2);

        QPointF arrowP1 = middle + QPointF(sin(angle + Pi / 3) * -20,
                                           cos(angle + Pi / 3) * -20);
        QPointF arrowP2 = middle + QPointF(sin(angle + Pi - Pi / 3) * -20,
                                           cos(angle + Pi - Pi / 3) * -20);
        arrowHead.clear();
        arrowHead << middle << arrowP1 << arrowP2;
        //! [6] //! [7]
        painter->drawLine(line());
        painter->drawPolygon(arrowHead);
        //        QLineF l1(middle, arrowP1);
        //        QLineF l2(middle, arrowP2);

        //        painter->drawLine(l1);
        //        painter->drawLine(l2);

        if (isSelected()) {
            painter->setPen(QPen(myColor, 1, Qt::DashLine));
            QLineF myLine = line();
            myLine.translate(0, 4.0);
            painter->drawLine(myLine);
            myLine.translate(0,-8.0);
            painter->drawLine(myLine);
        }
    }
}

QRectF CGShape::getRect(){
    return rect;
}
