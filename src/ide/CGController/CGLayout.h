/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGLAYOUT_H
#define CGLAYOUT_H
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE

#include "CGLayoutItem.h"
#include "CGLayoutView.h"
#include "CGController.h"

class CGLayout : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode {ControlConnection,Target, Resize, Connect, Composition, MoveView, InsertText, InsertShape, MultiSelection};

    CGLayout(QMenu *layoutMenu, QMenu * connectorMenu, QObject *parent = 0);

    void setName(QString name);
    QString getName();
    int getId();

    QPointF getLastSceneRightPressedPos();
    QPointF getLastSceneLeftPressedPos();

    CGLayoutView* getView();
    void setView(CGLayoutView* view);

    QGraphicsRectItem* getRect();
    void load();
    void unselectAll();

    void setMode(Mode mode);
    int getMode();
    QMenu* getCommentShapeMenu();
    QMenu* getCommentTextMenu();
    QMenu* getOperatorMenu();
    QMenu* getConnectorMenu();
    QMenu* getMultiMenu();
    QMenu* getCompositionMenu();
    QMenu* getOperatorMultiMenu();

    void setTextColor(const QColor &color);
    void setItemColor(const QColor &color);
    void setFont(const QFont &font);

    void activate(QGraphicsItem* item);
    void unactivate(QGraphicsItem* item);
    void reloadCursor();
    static int lid;

    QDockWidget* getGlossary();
    QDockWidget* getProject();

    void setType(QString type);
    QString getType();

    QString getResolution();
    void setResolution(QString s);

    bool getIsPopup();
    void setIsPopup(bool isPopup);
public slots:
    void editorLostFocus(CGText *item);

signals:
    void itemSelected(QGraphicsItem *item);

protected:
    QString resolution;
    bool isPopup;
    QString type;
    int id;

    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void dropEvent ( QGraphicsSceneDragDropEvent * event );
    void dragEnterEvent ( QGraphicsSceneDragDropEvent * event );
    void dragMoveEvent ( QGraphicsSceneDragDropEvent * event );
    void keyPressEvent ( QKeyEvent * keyEvent );
    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent);

    bool isItemChange(int type);

    CGLayoutView* view;
    QGraphicsRectItem* rect;
    QMenu *operatorMenu;
    QMenu *connectorMenu;
    QMenu *commentMenu;
    QMenu *shapeMenu;

    Mode sceneMode;
    CGShape* shape;

    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;

    bool showOrigin;

    QGraphicsRectItem* selectionRect;
    QPointF multiSelectionPos;
    QPointF lastRightPressScenePos;
    QPointF lastLeftPressScenePos;

    CGLayoutItem* layoutItem;

    QString name;

};

#endif // CGLAYOUT_H
