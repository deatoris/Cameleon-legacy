/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGComposition_H
#define CGComposition_H

#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include <QGraphicsScene>
#include "CGLayoutItem.h"
QT_BEGIN_NAMESPACE
class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
class QFont;
class QGraphicsTextItem;
class QColor;
QT_END_NAMESPACE
#include "CGCompositionItem.h"
#include "CGCompositionView.h"
#include "CGComposer.h"
#include "CControl.h"

class CGComposition : public QGraphicsScene
{
    Q_OBJECT

public:
    enum Mode {MovePlug, ShowControlConnection, ControlConnection, Target, Resize, Connect, Composition, MoveView, InsertText, InsertShape, MultiSelection};

    CGComposition(CGCompositionModel* model,QMenu *operatorMenu, QMenu *operatorMultiMenu, QMenu * connectorMenu, QMenu* multiMenu, QMenu* compositionMenu, QObject *parent = 0);

    QPointF getLastSceneRightPressedPos();
    QPointF getLastSceneLeftPressedPos();
    void addBackgroundRec();

    CGCompositionView* getView();
    void setView(CGCompositionView* view);

    QGraphicsRectItem* getRect();
    void load();
    void unselectAll();

    void reloadCursor();
    void setMode(Mode mode);
    int getMode();
    int distance(QPointF p1, QPointF p2);

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

    CGCompositionModel* getModel();
    void setToConnect(CControl* control, CGConnector* connectorToConnect);
    CControl* getControlToConnect();
    CGConnector* getConnectorToConnect();
    void showConnected(CGConnector* connector);

    void clearAllFocus();
    static QRectF srect;
public slots:
    void editorLostFocus(CGText *item);
    void action();
    void action(QAction* act);
signals:
    void itemSelected(QGraphicsItem *item);

protected:
    void ungraball();
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void dropEvent ( QGraphicsSceneDragDropEvent * event );
    void dragEnterEvent ( QGraphicsSceneDragDropEvent *  );
    void dragMoveEvent ( QGraphicsSceneDragDropEvent *  );
    void keyPressEvent ( QKeyEvent * keyEvent );
    void mouseDoubleClickEvent (QGraphicsSceneMouseEvent * mouseEvent);
    bool isItemChange(int type);

    CGCompositionView* view;
    QGraphicsRectItem* rect;
    QMenu *operatorMenu;
    QMenu *connectorMenu;
    QMenu *multiMenu;
    QMenu *compositionMenu;
    QMenu *operatorMultiMenu;
    CGConnector *toConnector;
    CGMultiConnector *toMultiConnector;
    Mode sceneMode;
    QGraphicsLineItem *line;
    QColor lineColor;

    QGraphicsRectItem* selectionRect;
    QPointF multiSelectionPos;
    QPointF lastRightPressScenePos;
    QPointF lastLeftPressScenePos;

    CGCompositionModel* model;

    CGShape* shape;

    QFont myFont;
    QColor myTextColor;
    QColor myItemColor;

    QGraphicsLineItem* cross;

    bool showOrigin;

    CControl* control;
    CGConnector* connectorToConnect;
    QGraphicsTextItem* textPo;
    CGLayoutItem* layoutItem;
    QAction* selectedAction;

};

#endif
