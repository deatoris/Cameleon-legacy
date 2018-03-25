/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGCONTROLCOMPONENT_H
#define CGCONTROLCOMPONENT_H
#include "CControl.h"
#include <QtGui>
#include "CGCompositionItem.h"

class CGLayoutItemConnector;
class CGLayout;
class CGLayoutItem : public QGraphicsItem
{
public:
    enum { Type = UserType + 79 };
    enum LayoutMode {NA,BottomLeft, BottomRight, TopLeft, TopRight, VerTop, VerBottom, HorRight, HorLeft};

    CGLayoutItem(QMenu *contextMenu,
                 QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    int type() const
        { return Type;}

    QRectF boundingRect() const;
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    int getMode();

    void resize(QPointF toscenepoint);
    void resize(QRect toscenepoint);
    void resize();

    void setColor(QColor color);

    void setRect(QRectF rect);

    void setControl(CControl* control);
    CControl* getControl();
    void render();
    QList<CGLayoutItemConnector*> getConnectorsL();
    CGLayoutItemConnector* getConnectorById(int id);

    QRectF getRect();
    CGLayout* getLayout();
    void setLayout(CGLayout* layout);
    bool getIsPersistant();
    void setIsPersistant(bool pers);
    QString getError();
    void setError(QString errormsg);
protected:
    QString error;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void hoverEnterEvent ( QGraphicsSceneHoverEvent * event );
    void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );
    void hoverMoveEvent ( QGraphicsSceneHoverEvent * event );

private:
    bool isPersistant;
    QGraphicsRectItem* grect ;
    QGraphicsRectItem* grecterror ;
    void setConnectorsPos();
    QPolygonF myPolygon;
    QMenu *myContextMenu;
    QGraphicsTextItem* textItem;

    QGraphicsTextItem* itemName;
    CGEllipse* ellipse1;
    CGEllipse* ellipse2;
    CGEllipse* ellipse3;
    CGEllipse* ellipse4;
    CGEllipse* ellipse5;
    CGEllipse* ellipse6;
    CGEllipse* ellipse7;
    CGEllipse* ellipse8;

    LayoutMode mode;

    QRectF rect;

    QColor shapeColor;

    CControl* innercontrol; //widget to be proxied
    QGraphicsProxyWidget* proxy;

    QList<CGLayoutItemConnector*> connectorsList;

    CGLayout* layout;

};

class CGLayoutItemConnector : public CGConnector
{//proxy
public:
    enum { Type = UserType + 82 };
    int type() const
    { return Type;}

    //child build
    CGLayoutItemConnector(QMenu *contextMenu, QString side, CGConnectorModel* model,
                          QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);

    virtual void updatePosition();
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = 0);
    virtual QRectF boundingRect() const;

protected:
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

};

#endif // CGCONTROLCOMPONENT_H
