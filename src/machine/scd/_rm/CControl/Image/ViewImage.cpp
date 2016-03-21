#include "ViewImage.h"

#include<DataImageQt.h>
ViewImage::ViewImage(QWidget * parent)
    :CControl(parent),image(new QImage)
{
    this->path().push_back("Image");
    this->setName("ViewImage");
    this->setKey("ViewImage");
    this->structurePlug().addPlugIn(DataImageQt::KEY,"in.bmp");


    layout = new QVBoxLayout;
    scene = new QGraphicsScene();
    view = new ViewImageView(scene);
    scene->setSceneRect(QRectF(0, 0, 10000, 10000));
    scene->setBackgroundBrush(Qt::black);
    imageLabel = new QLabel;
    proxy = scene->addWidget(imageLabel);;
    layout->addWidget(view);
    this->setLayout(layout);
}

void ViewImage::setView(){

    QRect rect1 =  image->rect();
    imageLabel->setGeometry(rect1);
    imageLabel->setBackgroundRole(QPalette::Base);
//    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    scene->setSceneRect(rect1);
}

//void ViewImage::mousePressEvent ( QMouseEvent * event ){

//    QPointF p = this->proxy->mapFromScene(event->posF());
//    qDebug() << "void ViewImage::mousePressEvent  POSITION : " << p;
//    QWidget::mousePressEvent(event);
//}

CControl * ViewImage::clone(){
    return new ViewImage();
}

void ViewImage::updatePlugInControl(int ,CData* data){


        string str = data->toString();
        image->load(str.c_str());
        if(image->isNull()==false){
            setView();
            this->update();
        }
        else{
            this->error("Cannot read input image"+str);
        }

}

ViewImageView::ViewImageView(QGraphicsScene * scene, QWidget * parent)
    : QGraphicsView(scene,parent)
{
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
    leftIsPressed = false;
    this->setAcceptDrops(true);


}

void ViewImageView::resizeEvent ( QResizeEvent *  ){

}

void ViewImageView::wheelEvent(QWheelEvent *e)
{
    int scaleFactor = 50; //zoom sensibility
    if(e->delta()<0) {
        scaleFactor = - scaleFactor;
    }
    QPoint vpos1 = e->pos();
    QPointF spos1 = mapToScene(vpos1);
    qreal s = pow((double)2, scaleFactor / 360.0);
    if (!scaleView(s)) return;
    QPoint vpos2 = mapFromScene(spos1);
    QPoint dp = vpos2 - vpos1;
    scrollView(-dp);
}

void ViewImageView::mouseMoveEvent ( QMouseEvent * e ){
    if(leftIsPressed){
        QPoint dp = previousPos - e->pos();
        scrollView(-dp);
        previousPos = e->pos();
    }
}

bool ViewImageView::scaleView(qreal scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    return true;
}

void ViewImageView::scrollView(QPoint dp)
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
}

void ViewImageView::mousePressEvent ( QMouseEvent * e ){
    if(e->button() == Qt::LeftButton){
        leftIsPressed = true;
        previousPos = e->pos();
        this->setCursor(Qt::ClosedHandCursor);
    }
//    QGraphicsView::mousePressEvent(e);

}

void ViewImageView::mouseReleaseEvent ( QMouseEvent * e ){
    this->setCursor(Qt::OpenHandCursor);
    leftIsPressed = false;
    QGraphicsView::mouseReleaseEvent(e);
}

void ViewImageView::keyPressEvent ( QKeyEvent *  ){
    this->setCursor(Qt::OpenHandCursor);
}

void ViewImageView::keyReleaseEvent ( QKeyEvent *  ){
}
