#include "cimage.h"
#include<DataImageGrid.h>
CImage::CImage(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Image");
    this->setName("In");
    this->setKey("Image");
    this->structurePlug().addPlugIn(DataImageGrid::KEY,"in.num");


    QVBoxLayout *layout = new QVBoxLayout;
    scene = new QGraphicsScene();
    view = new CImageView(scene);

    scene->setSceneRect(QRectF(-10000, -5000, 20000, 10000));
    scene->setBackgroundBrush(Qt::lightGray);

    proxy = NULL;

    layout->addWidget(view);
    this->setLayout(layout);

    //loadImage("");

}
void CImage::loadImage(QString fileName){
    //fileName = "E:/root/documents/00-Cameleon/03-Src/share/Cameleon1.0/trunk/deliverable/Lena.pgm";
    //fileName ="/home/vincent/Desktop/lena.pgm";
    if(proxy!=NULL) scene->removeItem(proxy);
    if(proxy!=NULL) delete proxy;
    if (!fileName.isEmpty()) {
        QImage image(fileName);
//        if (image.isNull()) {
//            QMessageBox::information(this, tr("Image Viewer"),
//                                     tr("Cannot load %1.").arg(fileName));
//            return;
//        }
        imageLabel = new QLabel;

        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);
        imageLabel->setPixmap(QPixmap::fromImage(image));
        imageLabel->setScaledContents(true);

        proxy = scene->addWidget(imageLabel);

        QRectF rect = imageLabel->rect();
//        view->ensureVisible(proxy);
        view->setSceneRect(rect);
//        view->fitInView(proxy, Qt::KeepAspectRatio);

    }
}

CControl * CImage::clone(){
    return new CImage();
}

void CImage::paintEvent ( QPaintEvent * event ){

}

void CImage::updatePlugInControl(int indexplugin,CData* data,CPlug::State state){
    if(DataImageGrid * d = dynamic_cast<DataImageGrid *>(data)){
        this->loadImage(d->getFile().c_str());
        this->update();
    }
}

CImageView::CImageView(QGraphicsScene * scene, QWidget * parent)
    : QGraphicsView(scene,parent)
{
    scaleNum = 0;
//    setRenderHints(QPainter::Antialiasing
//                   | QPainter::SmoothPixmapTransform
//                   | QPainter::TextAntialiasing
//                   | QPainter::HighQualityAntialiasing);
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
    leftIsPressed = false;
    this->setAcceptDrops(true);
    duration = 10000;

}

void CImageView::resizeEvent ( QResizeEvent * event ){

}

void CImageView::wheelEvent(QWheelEvent *e)
{
    if(e->modifiers().testFlag(Qt::AltModifier)){
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

void CImageView::mouseMoveEvent ( QMouseEvent * e ){
    if(e->modifiers().testFlag(Qt::AltModifier) && leftIsPressed){
        QPoint dp = previousPos - e->pos();
        scrollView(-dp);
        previousPos = e->pos();
    }else{
        QGraphicsView::mouseMoveEvent(e);
    }
}

bool CImageView::scaleView(qreal scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    return true;
}

void CImageView::scrollView(QPoint dp)
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

void CImageView::mousePressEvent ( QMouseEvent * e ){
    if(e->modifiers().testFlag(Qt::AltModifier)
            && e->button() == Qt::LeftButton){
        leftIsPressed = true;
        previousPos = e->pos();
        this->setCursor(Qt::ClosedHandCursor);
    }else{
        QGraphicsView::mousePressEvent(e);
    }
}

void CImageView::mouseReleaseEvent ( QMouseEvent * e ){
    if(e->modifiers().testFlag(Qt::AltModifier)){
        this->setCursor(Qt::OpenHandCursor);
    }
    leftIsPressed = false;
    QGraphicsView::mouseReleaseEvent(e);
}

void CImageView::keyPressEvent ( QKeyEvent * e ){
    if(e->modifiers().testFlag(Qt::AltModifier)){
        this->setCursor(Qt::OpenHandCursor);
    }else{
        QGraphicsView::keyPressEvent(e);
    }
}

void CImageView::keyReleaseEvent ( QKeyEvent * e ){
}
