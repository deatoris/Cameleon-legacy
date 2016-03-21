/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGLAYOUTVIEW_H
#define CGLAYOUTVIEW_H
#include <QGraphicsView>

class CGLayoutView: public QGraphicsView
{

public:
    CGLayoutView(QWidget *parent = 0);
    CGLayoutView(QGraphicsScene * scene, QWidget * parent = 0, bool isPreview = false);
    void renderScale(int scale);
    int getScaleNum();

protected:
    virtual void wheelEvent ( QWheelEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    bool scaleView(qreal scaleFactor);
    void scrollView(QPoint dp) ;
    void setPreview(bool is);

    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
    virtual void resizeEvent ( QResizeEvent * event );

private:
    QPoint previousPosPM;
    QPointF targetPoint;
    int duration;
    int currentDuration;
    int scaleNum;
    bool leftIsPressed;
    QPoint previousPos;
    QGraphicsRectItem* item;
    bool isPreview;
    bool zoomMode;
};
#endif // CGLAYOUTVIEW_H
