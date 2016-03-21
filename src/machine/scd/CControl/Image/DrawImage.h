#ifndef DRAWIMAGE_H
#define DRAWIMAGE_H

#include<CControl.h>
#include<QtGui>
class DrawImageView;

class DrawImage :  public CControl
{
    Q_OBJECT
public:
    enum Mode {Navigation, Paint};
    enum ShapeType {Round, Square, Losange};

    DrawImage(QWidget * parent = 0);
    virtual CControl * clone();
    int getMode();
    void updatePlugInControl(int indexplugin,CData* data);
    void drawPoint(QPointF point);
    virtual void apply();
    void initBall();
public slots:
    void navigationMode();
    void paintMode();
    void colorChanged();
    void shapeChanged();
    int paintSizeChanged();
    void save();
    void clear();
private:
    void resizeImage(int width,int height);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QMenu *createShapeMenu(const char *slot, ShapeType defaultShapeType);
    QIcon createColorToolButtonIcon(QColor color);
    QIcon createShapeToolButtonIcon(ShapeType defaultShapeType);
    QIcon createColorIcon(QColor color);
    QIcon createShapeIcon(ShapeType defaultShapeType);
    void createBlankImage();
    void createCommentBar();

    Mode mode;
    void loadImage(QString fileName);
    QPushButton* applyButton;
    QLineEdit *lineEdit;
    DrawImageView* imageView;
    double val;
    QGraphicsProxyWidget* proxy;
    DrawImageView* view;
    QGraphicsScene* scene;
    QToolBar* bar;
    QComboBox* fontSizeCombo;

    QToolButton* colorToolButton;
    QImage* image;
    QLabel* imageLabel;
    QAction* pencilAction;
    QAction* eyeAction;

    QToolButton* shapeToolButton;

    QColor currentColor;
    vector<pair<int,int> >_v_ball;
    int norm;
    int currentPaintSize;
    ShapeType currentShapeType;
    bool _test;
    int wi;
    int hei;

};


class DrawImageView: public QGraphicsView
{

public:
    DrawImageView(QGraphicsScene * scene=0, QWidget * parent = 0);
    void setControl(DrawImage* control);
protected:
    bool scaleView(qreal scaleFactor);
    void scrollView(QPoint dp) ;
    void setPreview(bool is);

    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void keyReleaseEvent ( QKeyEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void resizeEvent ( QResizeEvent * event );
    virtual void wheelEvent ( QWheelEvent * event );
private:
    DrawImage* control;
    QPointF targetPoint;
    int duration;
    int currentDuration;
    int scaleNum;
    bool leftIsPressed;
    QPoint previousPos;
    QGraphicsRectItem* item;
    bool isPreview;

};


#endif // DRAWIMAGE_H
