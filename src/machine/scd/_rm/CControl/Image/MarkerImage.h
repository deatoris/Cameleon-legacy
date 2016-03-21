#ifndef MARKERIMAGE_H
#define MARKERIMAGE_H

#include<CControl.h>
#include<QtGui>
#include<ViewImage.h>
#include <tr1/memory>
#include<tr1/shared_ptr.h>
class MarkerImageView;

class MarkerImage :  public CControl
{
    Q_OBJECT
public:
    enum Mode {Navigation, Paint,Eraser};
    enum ShapeType {Round, Square, Losange};
        void setView();
    MarkerImage(QWidget * parent = 0);
    virtual CControl * clone();
    int getMode();
    virtual void updatePlugInControl(int indexplugin,CData* data);
    void drawPoint(QPointF point);
//    virtual string toString();
//    virtual void fromString(string file);
    void initBall();
    virtual void apply();
public slots:
    void navigationMode();
    void paintMode();
     void eraserMode();
    void colorChanged();
    void shapeChanged();
    int paintSizeChanged();
    void save();
    virtual void clear();
protected:
    void convertRGB(QImage *&in);
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QMenu *createShapeMenu(const char *slot, ShapeType defaultShapeType);
    QIcon createColorToolButtonIcon(QColor color);
    QIcon createShapeToolButtonIcon(ShapeType defaultShapeType);
    QIcon createColorIcon(QColor color);
    QIcon createShapeIcon(ShapeType defaultShapeType);
    void createBlankImage(string file);
    void createCommentBar();

    Mode mode;
    QPushButton* applyButton;
    QLineEdit *lineEdit;
    MarkerImageView* imageView;
    double val;
    QGraphicsProxyWidget* proxy;
    MarkerImageView* view;
    QGraphicsScene* scene;
    QToolBar* bar;
    QComboBox* fontSizeCombo;

    QToolButton* colorToolButton;
    QImage* imagebackground;
    QImage* imageToSave;
    QImage* image;
    QLabel* imageLabel;
    QAction* pencilAction;
    QAction* eyeAction;
    QAction* eraserAction;

    QToolButton* shapeToolButton;
    QVBoxLayout *layout;
    QColor currentColor;
    vector<pair<int,int> >_v_ball;
    int norm;
    int currentPaintSize;
    ShapeType currentShapeType;
    bool _test;
    string _file;
};


class MarkerImageView: public QGraphicsView
{

public:
    MarkerImageView(QGraphicsScene * scene=0, QWidget * parent = 0);
    void setControl(MarkerImage* control);
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
    MarkerImage* control;
    QPointF targetPoint;
    int duration;
    int currentDuration;
    int scaleNum;
    bool leftIsPressed;
    QPoint previousPos;
    QGraphicsRectItem* item;
    bool isPreview;

};


#endif // MARKERIMAGE_H
