/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGCOMPOSITIONITEM_H
#define CGCOMPOSITIONITEM_H

#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QtGui>
#include <cmath>
#include "CGCompositionModel.h"
#include<COperator.h>
#include<CPlug.h>

QT_BEGIN_NAMESPACE
class QGraphicsPolygonItem;
class QGraphicsLineItem;
class QGraphicsScene;
class QRectF;
class QGraphicsSceneMouseEvent;
class QPainterPath;
QT_END_NAMESPACE
class CGConnector;
class CGArrow;
class CGOperatorModel;
class CGConnectorModel;
class CGCompositionModel;
class CGMulti;
class CGMultiConnector;
class CGLayoutItemConnector;
class CGOperator : public QGraphicsItem
{
public:
    enum { Type = UserType + 20 };
    CGOperator(QMenu *operatorMenu, QMenu *connectorMenu,
               QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    //enum State {RUNNING, RUNNABLE, NOTRUNNABLE};

    void updateState(COperator::State state);

    void removeConnector(CGConnector *);
    void addConnector(CGConnector *connector);
    QPixmap image();
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget *  widget= 0 );
    virtual QRectF boundingRect() const;
    int type() const
    { return Type;}

    void disconnect();
    void setModel(CGOperatorModel* model);
    CGOperatorModel* getModel();
    CGConnector* getConnectorById(int id);
    QList<CGConnector*> getConnectorsList();
    void renderModel();
    CGOperator* clone();
    void drawInfo(QPainter* painter);
    int centerName(int size);
    void setError(QString errorMsg);
    void setDoc(bool doc);
    void drawDocs();

    QString getError();
    QMenu* getActionMenu();
    void saveAction(QString action);
    QList<QString> getActionList();
    void setIsHighlight(bool isH);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
private:

    bool isHighLight;
    QList<QString> actionsL;
    QString errormsg;
    bool doc;
    int nbConnectors;
    void updateDocs();
    QList<CGConnector*> connectorsL;
    QMenu* operatorMenu;
    QMenu* connectorMenu;
    QGraphicsEllipseItem* outerEllipse;
    QGraphicsEllipseItem* innerEllipse;

    QColor innerColor;
    QColor outerColor;
    QColor textColor;

    int rayon;
    CGOperatorModel* model;
    //DOCS
    QGraphicsEllipseItem* ellipse1;
    QGraphicsEllipseItem* ellipse2;
    QList<QGraphicsTextItem*> itemDocList;
    QMap<int, QGraphicsTextItem*> mapDoc;
    QGraphicsTextItem* informations;
    QGraphicsRectItem* docrect1;
    QGraphicsRectItem* docrect2;

    QMenu* actionsMenu;
};

class CGConnector : public QGraphicsItem
{
public:
    enum { Type = UserType + 25 };
    CGConnector(QMenu *contextMenu, QString side, CGConnectorModel* model,
                QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);


    //enum State {EMPTY, OLD, NEW};

    void updateState(CPlug::State state);
    QColor getColor();
    void setColor(QColor c);

    virtual void updatePosition();
    virtual void disconnect();
    virtual void connect(CGConnector* connectedConnector);
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    virtual QRectF boundingRect() const;
    virtual bool isConnected();

    void setIsMoving(bool b);
    virtual void freePosition();
    void rePosition();

    void removeArrow();
    int type() const
    { return Type;}
    bool isIn();
    CGConnector* getConnectedConnector();
    CGConnector* getConnectedControlConnector();
    void setModel(CGConnectorModel* model);
    CGConnectorModel* getModel();

    CGArrow* getArrow();
    void setArrow(CGArrow* arrow);

    void setIsUp(bool isup);
    void setRayon(int rayon);
    int getRayon();
    QString getSide();
    virtual int getId();

    CGOperator* getOperator();
    void setOperator(CGOperator* ope);
    void connectToNearest(CGConnector* connector);
    void disconnectNearest(CGConnector* connector);
    void setCompatibleMode(bool compatibleMode);
    bool getCompatibleMod();
    void setControlMode(bool controlMode);
    void setIsControlConnected(bool isControlConnected);
    bool getIsControlConnected();
    bool getControlMode();
    CGConnector* getNearest();
    void setIsHighLight(bool highlight);
    bool getIsHightLight();
    void moveTo(QPointF scenePos);

    void up();
protected:
    QPointF checkPosition(double px, double py);
    QPointF checkPosition2(double px, double py);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    QRect controlConnectionRect;

    QMenu* connectorMenu;
    CGOperator* ope;
    int rayon;
    bool isUp;
    int id;
    CGArrow* arrow;

    bool highlight;
    bool compatibleMode;
    bool controlMode;
    bool isControlConnected;

    QGraphicsEllipseItem* outerEllipse;
    QGraphicsEllipseItem* innerEllipse;
    QColor innerColor;
    QColor outerColor;
    int crayon;

    QString side;
    bool isPressed;
    bool isMoving;
    CGConnector* connectedConnector;
    CGConnectorModel* model;
    CGLayoutItemConnector* connectedLayoutConnector;
};

class CGMulti : public QGraphicsItem
{
public:
    enum { Type = UserType + 30 };
    enum MultiMode {INNER, OUTER, TOP};

    CGMulti(QMenu *operatorMenu,  QMenu *connectorMenu,
            QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );
    virtual QRectF boundingRect() const;

    void setMode(MultiMode mode);
    void renderModel();

    CGMultiConnector* addConnector(QString mode);
    void addConnector(CGConnector* connector);
    void addConnection(CGConnector* inner, CGConnector* outer);

    void buildViews();
    bool belongToThis(CGConnector* conn);
    void setIsUp(bool b);
    CGMulti* getOuterView();
    CGMulti* getInnerView();
    CGMulti* getParentView();
    void setParentView(CGMulti* parent);

    QList<CGConnector*> getConnectorsList();
    QList<CGMultiConnector*> getMultiConnectorsList();

    void disconnect();

    int type() const
    { return Type;}
    void setModel(CGCompositionModel* model);
    CGCompositionModel* getModel();

    int getMode();
    CGConnector* getConnectorById(int id);

    CGMulti* clone();
    int processInnerRay(bool atCreation);
    int getRayon();
    void processAllConnectorsPos();

    void setIsCompatibleMode(bool compatible);

    void setDoc(bool doc);
    void updateDocs();
    void drawDocs();
    void updateState(COperator::State state);
    void setError(QString error);
    void setRayon(int rayon);
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
private:
    bool isUp;
    QString errormsg;
    QMap<int, QGraphicsTextItem*> mapDoc;

    bool doc;
    bool compatibleMode;

    CGMulti* outerView; //null if parent is null
    CGMulti* innerView; //null if parent is null
    CGMulti* parentView; //null if parent is null

    QList<CGMultiConnector*> multiconnectorsL;

    QList<CGConnector*> connectorL;

    CGCompositionModel* model;

    MultiMode mode;

    QPointF processInnerCenter();
    QPointF getOuterCenter();
    int getOuterRay();

    QMenu* multiMenu;
    QMenu* connectorMenu;

    QGraphicsEllipseItem* outerEllipse;
    QGraphicsEllipseItem* innerEllipse;

    QColor innerColor;
    QColor outerColor;

    int countin;
    int countout;
    int rayon;
    int nbConnectors;


    QGraphicsEllipseItem* ellipse1;
    QGraphicsEllipseItem* ellipse2;
    QList<QGraphicsTextItem*> itemDocList;
};

class CGMultiConnector : public CGConnector
{//proxy
public:
    enum { Type = UserType + 26 };
    //Top build
    CGMultiConnector(QMenu *contextMenu,QString mode,
                QGraphicsItem *innerparent = 0, QGraphicsItem *outerparent = 0, QGraphicsScene *scene = 0);

    //child build
    CGMultiConnector(QMenu *contextMenu, QString side,
                QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual void updatePosition();
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual QRectF boundingRect() const;

    int type() const
    { return Type;}

    CGMultiConnector* getInnerConnector();
    CGMultiConnector* getOuterConnector();
    CGMultiConnector* getParentConnector();
    void setParentConnector(CGMultiConnector* connector);
    void setInitialPos(int countinmax,int countoutmax,int countin, int countout);
    void setIsInner(bool isInner);

    void setName(QString name);
    QString getName();

    void setId(int id);
    int getId();
    CGMulti* getMulti();
    CGConnector* getOtherSide();
    double getTeta();
    void toolT();
private:
    double teta;
    int id;
    CGMultiConnector* innerConnector;
    CGMultiConnector* outerConnector;
    CGMultiConnector* parentConnector;
    CGMulti* multi;
    bool isInner;
    QString name;
};

class CGArrow : public QGraphicsLineItem
{
public:
    enum { Type = UserType + 4 };

    CGArrow(CGConnector *startItem, CGConnector *endItem,
      QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setColor(const QColor &color)
        { myColor = color; }
    CGConnector *startItem() const
        { return myStartItem; }
    CGConnector *endItem() const
        { return myEndItem; }
    void updatePosition();
    void setIsMulti(bool isMulti, bool out);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);

private:
    bool isMulti;
    bool out;
    CGConnector *myStartItem;
    CGConnector *myEndItem;
    QColor myColor;
    QPolygonF arrowHead;
};

class CGEllipse;

class CGShape : public QGraphicsItem
{
public:
    enum { Type = UserType + 29 };
    enum ShapeMode {NA,BottomLeft, BottomRight, TopLeft, TopRight, VerTop, VerBottom, HorRight, HorLeft};

    CGShape(QMenu *contextMenu,
        QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type;}

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    int getMode();

    void resize(QPointF toscenepoint);

    void setColor(QColor color);

    CGShapeModel* getModel();

    CGShape* clone();

    QColor getColor();
    void setRect(QRectF rect);
    QRectF getRect();
    void setPersistent(bool per);
    bool getPersistent();
protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );
private:
    bool persistent;
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QGraphicsTextItem* textItem;

    CGEllipse* ellipse1;
    CGEllipse* ellipse2;
    CGEllipse* ellipse3;
    CGEllipse* ellipse4;
    CGEllipse* ellipse5;
    CGEllipse* ellipse6;
    CGEllipse* ellipse7;
    CGEllipse* ellipse8;

    ShapeMode mode;

    QRectF rect;

    QColor shapeColor;

    CGShapeModel* model;
};

class CGEllipse : public QGraphicsEllipseItem
{
public:
    CGEllipse(QGraphicsItem *parent, QGraphicsScene *scene);
    enum { Type = UserType + 27 };
    int type() const
        { return Type;}
};

class CGText : public QGraphicsTextItem
{
    Q_OBJECT

public:
    enum { Type = UserType + 3 };

    CGText(QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type; }
    CGTextModel* getModel();

    CGText* clone();
    void setPersistent(bool per);
    bool getPersistent();
signals:
    void lostFocus(CGText *item);
    void selectedChange(QGraphicsItem *item);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void focusOutEvent(QFocusEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    CGTextModel* model;
    bool persistent;
};

#endif // CGCOMPOSITIONITEM_H
