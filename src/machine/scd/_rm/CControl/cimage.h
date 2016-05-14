#ifndef CIMAGE_H
#define CIMAGE_H

#include<CControl.h>
#include<QtGui>
class CImageView;

class CImage :  public CControl
{
    Q_OBJECT
public:
    CImage(QWidget * parent = 0);
    virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data,CPlug::State state);

public slots:
    void paintEvent ( QPaintEvent * event );
private:
    void loadImage(QString fileName);
    QPushButton* applyButton;
    QLineEdit *lineEdit;
    CImageView* imageView;
    double val;
    QLabel* imageLabel;
    QGraphicsProxyWidget* proxy;
    CImageView* view;
    QGraphicsScene* scene;
};


class CImageView: public QGraphicsView
{

public:
    CImageView(QGraphicsScene * scene=0, QWidget * parent = 0);

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
    int duration;
    int currentDuration;
    int scaleNum;
    bool leftIsPressed;
    QPoint previousPos;
    QGraphicsRectItem* item;
    bool isPreview;

};

#endif // CIMAGE_H
