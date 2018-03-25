/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGLayoutItem.h"
#include "CGLayoutManager.h"
#include "CGCompositionManager.h"
#include "CGLayout.h"
#include "CLogger.h"

CGLayoutItem::CGLayoutItem(QMenu *,
                           QGraphicsItem *parent, QGraphicsScene *scene)
    : QGraphicsItem(parent, scene)
{
    myContextMenu = CGLayoutManager::getInstance()->getControlMenu();

    isPersistant = false;
    rect = QRect(0, 0, 200, 200);
    grect = new QGraphicsRectItem(this,scene);
    grect->setRect(rect);
    grect->hide();
    QPen pen(Qt::darkGray);
    grect->setPen(pen);
    QColor c(Qt::darkGray);
    c.setAlpha(150);
    QBrush brush(c);
    grect->setBrush(brush);

    grecterror = new QGraphicsRectItem(this,scene);
    grecterror->setRect(rect);
    grecterror->hide();
    QPen penerr(Qt::darkRed);
    grecterror->setPen(penerr);
    QColor cerr(Qt::darkRed);
    cerr.setAlpha(150);
    QBrush brusherror(cerr);
    grecterror->setBrush(brusherror);

    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    this->setAcceptHoverEvents(true);
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

    shapeColor = Qt::gray;

    layout = 0;
}

CGLayout* CGLayoutItem::getLayout(){
    return layout;
}

void CGLayoutItem::setLayout(CGLayout* layout){
    this->layout = layout;
}

void CGLayoutItem::setControl(CControl* control){
    this->innercontrol = control;
//    control->setStyleSheet("background: yellow");
    QWidget *widget;
    QHBoxLayout *layout;
    layout = new QHBoxLayout;
    layout->addWidget(control);
    widget = new QWidget;
    layout->setMargin(0);
    layout->setSpacing(0);
    widget->setLayout(layout);
//    widget->setStyleSheet("background: transparent");
    //    QPalette p;
    //    p.setColor(QPalette::Background, Qt::darkGray);
    //    widget->setPalette(p);
    proxy = scene()->addWidget(widget);
    proxy->setParentItem(this);
    proxy->setPos(20,20);
    rect.setWidth(proxy->minimumWidth()+40);
    rect.setHeight(proxy->minimumHeight()+40);
    this->resize(rect.toRect());
    this->setConnectorsPos();

    QString name = control->getName().c_str();
    itemName = new QGraphicsTextItem(this);
    QString pers = "(*)";
    if(!isPersistant){
        pers = "";
    }
    QString sid = QString::number(control->getId());
    itemName->setPlainText("["+sid+"]"+name+" "+pers);
    itemName->setDefaultTextColor(Qt::white);
    itemName->setPos(30,-3);
    itemName->hide();

//    widget->setWindowOpacity(0);
//    control->setWindowOpacity(1);
    if(control->isHidden()){
        widget->setWindowOpacity(0.2);
        rect.setWidth(500+40);
        rect.setHeight(300+40);
        this->resize(rect.toRect());
    }
    this->setToolTip(control->getInformation().c_str());
    QObject::connect(control, SIGNAL(errorMsg(int,string)),
                     CGLayoutManager::getInstance()->getController(), SLOT(controllerError(int,string)));
}

QString CGLayoutItem::getError(){
    return this->error;
}

void CGLayoutItem::setError(QString errormsg){
    this->error = errormsg;
    this->update();
}

CGLayoutItemConnector* CGLayoutItem::getConnectorById(int id){
    foreach(CGLayoutItemConnector*c, connectorsList){
        if(c->getId() == id) return c;
    }
    return NULL;
}

QRectF CGLayoutItem::boundingRect() const{
    if(this->isSelected()){
        int x = rect.x()-20;
        int y = rect.y()-20;
        int w = rect.width()+40;
        int h = rect.height()+40;
        return QRectF(x,y,w,h);
    }else{
        int x = rect.x()+20;
        int y = rect.y()+20;
        int w = rect.width()-40;
        int h = rect.height()-40;
        return QRectF(x,y,w,h);
    }
}


void CGLayoutItem::setRect(QRectF rect){
    this->rect = rect;
    this->proxy->setPos(rect.x()+20,rect.y()+20);
    this->proxy->resize(rect.width()-40, rect.height()-40);
    this->setConnectorsPos();
}

void CGLayoutItem::setConnectorsPos(){
    foreach(CGConnector* connector, connectorsList){
        if(connector->getModel()->getSide().compare("OUT") == 0){
            connector->setPos(rect.x()+rect.width()-10,connector->pos().y());
        }else{
            connector->setPos(rect.x()+10,connector->pos().y());
        }
    }
}

void CGLayoutItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    if(!this->isSelected()){
        scene()->clearSelection();
        setSelected(true);
    }
    if(this->scene() == CGLayoutManager::getInstance()->getCurrentLayout()){
        CGLayoutManager::getInstance()->setControlExtentionMenu(this->getControl()->getMenu());
        CGLayoutManager::getInstance()->getControlMenu()->exec(event->screenPos());
    }else{
        CGLayoutManager::getInstance()->setControlExtentionMenu(this->getControl()->getMenu());
        CGCompositionManager::getInstance()->getComposer()->getControlComposerMenu()->exec(event->screenPos());
    }
}

void CGLayoutItem::setColor(QColor color){
    shapeColor = color;
}

void CGLayoutItem::render(){
    //render plugs in & out
    //TODO render control
    for(int i =0;i<(int)this->innercontrol->structurePlug().plugIn().size();i++){
        CGConnectorModel* connector = new CGConnectorModel(NULL);
        connector->setSide("IN");
        connector->setName(QString::fromStdString(this->innercontrol->structurePlug().plugIn()[i].second));
        connector->setDataType(this->innercontrol->structurePlug().plugIn()[i].first);
        connector->setId(i);
        CGLayoutItemConnector* connector2 = new CGLayoutItemConnector(NULL, connector->getSide(), connector, this, scene());
        connector2->setModel(connector);
        connectorsList.append(connector2);
        connector2->setY(i*20+20);
    }
    for(int i =0;i<(int)this->innercontrol->structurePlug().plugOut().size();i++){
        CGConnectorModel* connector = new CGConnectorModel(NULL);
        connector->setSide("OUT");
        connector->setName(QString::fromStdString(this->innercontrol->structurePlug().plugOut()[i].second));
        connector->setDataType(this->innercontrol->structurePlug().plugOut()[i].first);
        connector->setId(this->innercontrol->structurePlug().plugIn().size()+i);
        CGLayoutItemConnector* connector2 = new CGLayoutItemConnector(NULL, connector->getSide(), connector, this, scene());
        connector2->setModel(connector);
        connectorsList.append(connector2);
        connector2->setY(i*20+20);
    }
    this->setConnectorsPos();

    foreach(CGConnector* connector, connectorsList){
        connector->hide();
    }
}

void CGLayoutItem::paint(QPainter * , const QStyleOptionGraphicsItem * , QWidget *  )
{
    //    QString message = "control position x: "+QString::number(this->scenePos().x())+" y: "+QString::number(this->scenePos().y());
    //    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,message);

    grect->setRect(rect);
    grecterror->setRect(rect);
    ellipse1->setRect(-5+rect.x(),-5+rect.y(),10,10);
    ellipse8->setRect(this->rect.width()-5+rect.x(),this->rect.height()-5+rect.y(),10,10);
    ellipse2->setRect(-5+rect.x(),this->rect.height()/2-5+rect.y(),10,10);
    ellipse7->setRect(this->rect.width()-5+rect.x(),this->rect.height()/2-5+rect.y(),10,10);
    ellipse4->setRect(this->rect.width()/2-5+rect.x(),-5+rect.y(),10,10);
    ellipse5->setRect(this->rect.width()/2-5+rect.x(),this->rect.height()-5+rect.y(),10,10);
    ellipse6->setRect(this->rect.width()-5+rect.x(),-5+rect.y(),10,10);
    ellipse3->setRect(-5+rect.x(),this->rect.height()-5+rect.y(),10,10);

}

QVariant CGLayoutItem::itemChange(GraphicsItemChange change,
                                  const QVariant &value)
{
    if (change == ItemSelectedChange && scene()) {
        if(!isSelected()){
            ////            ellipse1->show();
            ////            ellipse2->show();
            //            ellipse3->show();
            ////            ellipse4->show();
            //            ellipse5->show();
            //            ellipse6->show();
            //            ellipse7->show();
            ellipse8->show();
            if(error.compare("") == 0) grect->show();
            if(error.compare("") != 0) grecterror->show();
            itemName->show();
            foreach(CGConnector* connector, connectorsList){
                connector->show();
            }
        }else{
            //            ellipse1->hide();
            ellipse2->hide();
            ellipse3->hide();
            ellipse4->hide();
            ellipse5->hide();
            ellipse6->hide();
            ellipse7->hide();
            ellipse8->hide();
            itemName->hide();
            grect->hide();
            grecterror->hide();
            foreach(CGConnector* connector, connectorsList){
                connector->setCompatibleMode(false);
                connector->hide();
            }
        }
    }

    if(CGLayoutManager::getInstance()->getCurrentLayout())CGLayoutManager::getInstance()->getCurrentLayout()->update();
    if(CGCompositionManager::getInstance()->getCurrentComposerScene())CGCompositionManager::getInstance()->getCurrentComposerScene()->update();

    return QGraphicsItem::itemChange(change, value);
}

bool CGLayoutItem::getIsPersistant(){
    return isPersistant;
}

void CGLayoutItem::setIsPersistant(bool pers){
    this->isPersistant = pers;
    QString name = this->getControl()->getName().c_str();
    QString perss = "(*)";
    if(!isPersistant){
        perss = "";
    }
    QString sid = QString::number(this->getControl()->getId());
    itemName->setPlainText("["+sid+"]"+name+" "+perss);
    itemName->setDefaultTextColor(Qt::white);
    itemName->setPos(30,-3);
    if(!isSelected())itemName->hide();
}

void CGLayoutItem::hoverEnterEvent ( QGraphicsSceneHoverEvent *  ){

}

void CGLayoutItem::hoverLeaveEvent ( QGraphicsSceneHoverEvent *  ){

}

CControl* CGLayoutItem::getControl(){
    return this->innercontrol;
}

int CGLayoutItem::getMode(){
    return mode;
}

QList<CGLayoutItemConnector*> CGLayoutItem::getConnectorsL(){
    return connectorsList;
}

QRectF CGLayoutItem::getRect(){
    return rect;
}

void CGLayoutItem::resize(QRect rect){
    this->rect.setRect(rect.x(),rect.y(),rect.width(),rect.height());
    scene()->update();
    this->update();
    this->proxy->setPos(rect.x()+20,rect.y()+20);
    this->proxy->resize(rect.width()-40, rect.height()-40);
    this->setConnectorsPos();
}

void CGLayoutItem::resize(){
    if(rect.width()<proxy->minimumSize().width()+40){
        rect.setWidth(proxy->minimumSize().width()+40);
    }

    if(rect.height()<proxy->minimumSize().height()+40){
        rect.setHeight(proxy->minimumSize().height()+40);
    }
    scene()->update();
    this->proxy->setPos(rect.x()+20,rect.y()+20);
    this->proxy->resize(rect.width()-40, rect.height()-40);
    this->setConnectorsPos();
}

void CGLayoutItem::resize(QPointF toscenepoint){
    toscenepoint = this->mapFromScene(toscenepoint);
    if(this->getMode() == CGLayoutItem::BottomLeft){
        rect.setX(toscenepoint.x());
        rect.setBottom(toscenepoint.y());
        rect.setLeft(toscenepoint.x());
    }else if(this->getMode() == CGLayoutItem::TopLeft){
        rect.setY(toscenepoint.y());
        rect.setX(toscenepoint.x());
        rect.setTop(toscenepoint.y());
        rect.setLeft(toscenepoint.x());
    }else if(this->getMode() == CGLayoutItem::TopRight){
        rect.setRight(toscenepoint.x());
        rect.setTop(toscenepoint.y());
        rect.setY(toscenepoint.y());
    }else if(this->getMode() == CGLayoutItem::BottomRight){
        rect.setRight(toscenepoint.x());
        rect.setBottom(toscenepoint.y());
    }else if(this->getMode() == CGLayoutItem::VerBottom){
        rect.setBottom(toscenepoint.y());
    }else if(this->getMode() == CGLayoutItem::VerTop){
        rect.setTop(toscenepoint.y());
        rect.setY(toscenepoint.y());
    }else if(this->getMode() == CGLayoutItem::HorLeft){
        rect.setLeft(toscenepoint.x());
    }else if(this->getMode() == CGLayoutItem::HorRight){
        rect.setRight(toscenepoint.x());
    }

    if(rect.width()<proxy->minimumSize().width()+40){
        rect.setWidth(proxy->minimumSize().width()+40);
    }

    if(rect.height()<proxy->minimumSize().height()+40){
        rect.setHeight(proxy->minimumSize().height()+40);
    }
    scene()->update();
    this->proxy->setPos(rect.x()+20,rect.y()+20);
    this->proxy->resize(rect.width()-40, rect.height()-40);
    this->setConnectorsPos();

}

void CGLayoutItem::hoverMoveEvent ( QGraphicsSceneHoverEvent * event ){
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

CGLayoutItemConnector:: CGLayoutItemConnector(QMenu *, QString side, CGConnectorModel* model,
                                              QGraphicsItem *parent, QGraphicsScene *scene)
    : CGConnector(CGLayoutManager::getInstance()->getConnectorMenu(),side,model,parent, scene)
{

}

void CGLayoutItemConnector::mouseReleaseEvent(QGraphicsSceneMouseEvent *){

}

void CGLayoutItemConnector::contextMenuEvent(QGraphicsSceneContextMenuEvent *event){
    setSelected(true);
    if(this->scene()==CGLayoutManager::getInstance()->getCurrentLayout()){
        CGLayoutManager::getInstance()->getConnectorMenu()->exec(event->screenPos());
    }else{
        CGCompositionManager::getInstance()->getComposer()->getControlConnectorMenu()->exec(event->screenPos());
    }

}

void CGLayoutItemConnector::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent *  ){
    if(isControlConnected){
        CGCompositionManager::getInstance()->showConnected(this->getConnectedConnector());
    }
}

void CGLayoutItemConnector::mousePressEvent(QGraphicsSceneMouseEvent *){
    if(!this->isConnected() && !compatibleMode){
        CGLayoutItem *layoutItem =
                qgraphicsitem_cast<CGLayoutItem *>(this->parentItem());
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setToConnect(layoutItem->getControl(), this);
        this->setCompatibleMode(true);
        this->parentItem()->setSelected(true);
        //        CGLayoutManager::getInstance()->getCurrentLayout()->setMode(CGLayout::ControlConnection);
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::ControlConnection);
    }else{
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
        this->setCompatibleMode(false);
    }
    CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
    this->scene()->update();
}

void  CGLayoutItemConnector::updatePosition(){

}

void  CGLayoutItemConnector::paint(QPainter* painter, const QStyleOptionGraphicsItem* , QWidget* ){
    if(highlight){
        QColor c = Qt::lightGray;
        c.setAlpha(50);
        painter->setBrush(c);
        painter->drawEllipse(-50, -50, 100, 100);

    }
    if(!this->isConnected() && compatibleMode){
        QColor c = Qt::yellow;
        c.setAlpha(150);

        painter->setBrush(c);
        painter->drawEllipse(-20, -20, 40, 40);

    }
    if(this->isConnected()){
        QColor c = Qt::white;
        c.setAlpha(150);
        painter->setBrush(c);
        painter->drawEllipse(-20, -20, 40, 40);
    }
    if(isIn()){
        painter->setPen(outerColor);
        painter->setBrush(outerColor);
        painter->drawEllipse(-10, -10, 20, 20);
        painter->setBrush(innerColor);
        painter->drawEllipse(-6, -6, 12, 12);
    }else{
        painter->setPen(innerColor);
        painter->setBrush(innerColor);
        painter->drawEllipse(-10, -10, 20, 20);
        painter->setBrush(outerColor);
        painter->drawEllipse(-6, -6, 12, 12);
    }
}

QRectF  CGLayoutItemConnector::boundingRect() const{
    if(highlight){
        return QRectF(-50, -50, 100, 100);
    }else if(compatibleMode){
        return QRectF(-20, -20, 40, 40);
    }else{
        return QRectF(-10, -10, 20, 20);
    }
}


