/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGLayoutView.h"
#include <QtGui>
#include "CGLayoutItem.h"
#include "CGLayoutManager.h"
CGLayoutView::CGLayoutView(QWidget *parent)
    : QGraphicsView(parent)
{
    scaleNum = 0;
    setRenderHints(QPainter::Antialiasing
                   | QPainter::SmoothPixmapTransform
                   | QPainter::TextAntialiasing
                   | QPainter::HighQualityAntialiasing);
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
    leftIsPressed = false;
    this->isPreview = isPreview;
    this->setAcceptDrops(true);
    duration = 10000;

}

CGLayoutView::CGLayoutView(QGraphicsScene * scene, QWidget *parent, bool isPreview)
    : QGraphicsView(scene, parent)
{
    scaleNum = 0;
    setRenderHints(QPainter::Antialiasing
                   | QPainter::TextAntialiasing
                   | QPainter::HighQualityAntialiasing);
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
    leftIsPressed = false;
    this->isPreview = isPreview;
    this->setAcceptDrops(true);
}

void CGLayoutView::resizeEvent ( QResizeEvent *  ){

}

void CGLayoutView::wheelEvent(QWheelEvent *e)
{
    if(zoomMode
            && this->itemAt(e->pos()) != 0
            && this->itemAt(e->pos())->type() == QGraphicsRectItem::Type){
        int scaleFactor = 50; //zoom sensibility
        if(e->delta()>0) {
            scaleNum ++;
        }else{
            scaleNum --;
            scaleFactor = - scaleFactor;
        }
        QPoint vpos1 = e->pos();
        QPointF spos1 = mapToScene(vpos1);
        qreal s = pow((double)2, scaleFactor / 360.0);
        if (!scaleView(s)) return;
        QPoint vpos2 = mapFromScene(spos1);
        QPoint dp = vpos2 - vpos1;
        scrollView(-dp);
    }else{
        QGraphicsView::wheelEvent(e);
    }
}

void CGLayoutView::mouseMoveEvent ( QMouseEvent * e ){
    previousPosPM = e->pos();
    if(zoomMode && leftIsPressed
            && this->itemAt(e->pos()) != 0
            && this->itemAt(e->pos())->type() == QGraphicsRectItem::Type){
        QPoint dp = previousPos - e->pos();
        scrollView(-dp);
        previousPos = e->pos();
        //        this->setCursor(Qt::ClosedHandCursor);
    }else{
        QGraphicsView::mouseMoveEvent(e);
    }
}

bool CGLayoutView::scaleView(qreal scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    return true;
}

void CGLayoutView::scrollView(QPoint dp)
{
    QScrollBar* hscroll = horizontalScrollBar();
    QScrollBar* vscroll = verticalScrollBar();
    if (hscroll)
    {
        int v = hscroll->value();
        hscroll->setValue(v - dp.x());
    }
    if (vscroll)
    {
        int v = vscroll->value();
        vscroll->setValue(v - dp.y());
    }
    qDebug()<<this->verticalScrollBar()->value();
    qDebug()<<this->horizontalScrollBar()->value();
}

void CGLayoutView::mousePressEvent ( QMouseEvent * e ){
    if(zoomMode
            && this->itemAt(e->pos()) != 0
            && this->itemAt(e->pos())->type() == QGraphicsRectItem::Type
            && e->button() == Qt::LeftButton){
        leftIsPressed = true;
        previousPos = e->pos();
        //        this->setCursor(Qt::ClosedHandCursor);
    }else{
        QGraphicsView::mousePressEvent(e);
    }
}

void CGLayoutView::renderScale(int scale){
    int scaleFactor = 50; //zoom sensibility
    if(scale<0){
        scaleFactor = - scaleFactor;
    }
    while(scale != 0){
        if(scale>0) {
            scale--;
            scaleNum ++;
        }else{
            scaleNum --;
            scale++;
        }
        qreal s = pow((double)2, scaleFactor / 360.0);
        if (!scaleView(s)) return;
    }
}

int CGLayoutView::getScaleNum(){
    return scaleNum;
}

void CGLayoutView::mouseReleaseEvent ( QMouseEvent * e ){
    if(zoomMode
            && this->itemAt(e->pos()) != 0
            && this->itemAt(e->pos())->type() == QGraphicsRectItem::Type){
        //        this->setCursor(Qt::OpenHandCursor);
    }
    leftIsPressed = false;
    QGraphicsView::mouseReleaseEvent(e);
}

void CGLayoutView::keyPressEvent ( QKeyEvent * e ){
    if(!CGLayoutManager::getInstance()->getController()->getStandaloneMode()){
        if(e->key() == Qt::Key_Plus){
            int scaleFactor = 50; //zoom sensibility
            scaleNum ++;
            QPoint vpos1 = previousPosPM;
            QPointF spos1 = mapToScene(vpos1);
            qreal s = pow((double)2, scaleFactor / 360.0);
            if (!scaleView(s)) return;
            QPoint vpos2 = mapFromScene(spos1);
            QPoint dp = vpos2 - vpos1;
            scrollView(-dp);
        }
        if(e->key() == Qt::Key_Minus){
            int scaleFactor = 50; //zoom sensibility
            scaleNum --;
            scaleFactor = - scaleFactor;
            QPoint vpos1 = previousPosPM;
            QPointF spos1 = mapToScene(vpos1);
            qreal s = pow((double)2, scaleFactor / 360.0);
            if (!scaleView(s)) return;
            QPoint vpos2 = mapFromScene(spos1);
            QPoint dp = vpos2 - vpos1;
            scrollView(-dp);

        }
        //    if(CGLayoutManager::getInstance()->getCurrentLayout()->getMode()!=CGLayout::InsertText
        //            &&CGLayoutManager::getInstance()->getCurrentLayout()->getMode()!=CGLayout::InsertShape
        //            &&CGLayoutManager::getInstance()->getCurrentLayout()->getMode()!=CGLayout::Target)this->setCursor(Qt::OpenHandCursor);
        if(!zoomMode && e->key() == Qt::Key_Q){
            //        this->setCursor(Qt::OpenHandCursor);
            zoomMode = true;
        }
    }
    QGraphicsView::keyPressEvent(e);
}

void CGLayoutView::keyReleaseEvent ( QKeyEvent * e ){
    if(zoomMode && e->key() == Qt::Key_Q){
        zoomMode = false;
        CGLayoutManager::getInstance()->getCurrentLayout()->reloadCursor();

    }
    QGraphicsView::keyReleaseEvent(e);
}
