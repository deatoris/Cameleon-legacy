#ifndef VIEWIMAGE_H
#define VIEWIMAGE_H

#include<CControl.h>
#include<QtGui>
class ViewImageView;

class ViewImage :  public CControl
{
public:
    ViewImage(QWidget * parent = 0);
    virtual CControl * clone();

    virtual void updatePlugInControl(int indexplugin,CData* data);

    void setView();

protected:
//    virtual void mousePressEvent ( QMouseEvent * event );

    ViewImageView* imageView;
    QVBoxLayout *layout;
    QImage *image;
    QLabel* imageLabel;
    QGraphicsProxyWidget* proxy;
    ViewImageView* view;
    QGraphicsScene* scene;
};


class ViewImageView: public QGraphicsView
{

public:
    ViewImageView(QGraphicsScene * scene=0, QWidget * parent = 0);

protected:
    bool scaleView(qreal scaleFactor);
    void scrollView(QPoint dp) ;
    void setPreview(bool is);

    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void wheelEvent ( QWheelEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
private:

    QPointF targetPoint;
    bool leftIsPressed;
    QPoint previousPos;
    QGraphicsRectItem* item;
    bool isPreview;

};

#endif // VIEWIMAGE_H
