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
    imageLabel = new QLabel;
    proxy = scene->addWidget(imageLabel);
    layout->addWidget(view);
    this->setLayout(layout);

    view->setBackgroundBrush(Qt::black);
    scene->setBackgroundBrush(Qt::black);
    imageLabel->setBackgroundRole(QPalette::Dark);
    view->setBackgroundRole(QPalette::Dark);
    this->setBackgroundRole(QPalette::Dark);
    imageLabel->setContentsMargins(2,2,2,2);
    this->setContentsMargins(2,2,2,2);
    layout->setContentsMargins(2,2,2,2);
    layout->setSpacing(0);
    layout->setMargin(0);

    view->setContentsMargins(2,2,2,2);
    proxy->setContentsMargins(2,2,2,2);

    layout->update();
    view->update();
    proxy->update();
    imageLabel->update();

}

string ViewImage::toString(){
    //view position

    QPoint p = this->view->getDp();
    int count = this->view->getWheelCount();
    QString save = QString::number(view->horizontalScrollBar()->value())
            +";"+QString::number(view->verticalScrollBar()->value())
            +";"+QString::number(count)
            +";"+QString::number(rect1.x())
    +";"+QString::number(rect1.y())
    +";"+QString::number(rect1.width())
    +";"+QString::number(rect1.height());
    return save.toStdString();
}

void ViewImage::fromString(string sin){
    QString s(sin.c_str());
    if(!s.isEmpty()){
        QList<QString> sL = s.split(";");
        if(sL.size() == 7){
            QString px = sL[0];
            QString py = sL[1];
            QString wheelcount = sL[2];
            int ipx = px.toInt();
            int ipy = py.toInt();
            int iwheelcount = wheelcount.toInt();
            QPoint dp(ipx,ipy);

            QString rx = sL[3];
            QString ry = sL[4];
            QString rw = sL[5];
            QString rh = sL[6];

            rect1.setX(rx.toInt());
            rect1.setY(ry.toInt());
            rect1.setWidth(rw.toInt());
            rect1.setHeight(rh.toInt());

            imageLabel->setGeometry(rect1);

            scene->setSceneRect(rect1);

            view->scaleView(iwheelcount);
            view->scrollView(ipx,ipy);
        }
    }
}

void ViewImage::setView(){

    rect1 =  image->rect();
    imageLabel->setGeometry(rect1);
    imageLabel->setBackgroundRole(QPalette::Dark);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    //imageLabel->adjustSize();
    scene->setSceneRect(rect1);
}


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
        this->error("Cannot read input image "+str);
    }

}

ViewImageView::ViewImageView(QGraphicsScene * scene, QWidget * parent)
    : QGraphicsView(scene,parent)
{
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
    leftIsPressed = false;
    this->setAcceptDrops(true);
    this->setContentsMargins(0,0,0,0);
    dpcount= QPoint(0,0);
    wheelcount = 0;

}

void ViewImageView::resizeEvent ( QResizeEvent *  ){

}

int ViewImageView::getWheelCount(){
    return wheelcount;
}

QPoint ViewImageView::getDp(){
    return dpcount;
}

void ViewImageView::wheelEvent(QWheelEvent *e)
{
    int scaleFactor = 50; //zoom sensibility
    if(e->delta()<0) {
        scaleFactor = - scaleFactor;
        //count
    }

    if (!scaleView(scaleFactor)) return;

    QPoint vpos1 = e->pos();
    QPointF spos1 = mapToScene(vpos1);
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

    wheelcount = wheelcount + scaleFactor;
    qreal s = pow((double)2, scaleFactor / 360.0);

    scale(s, s);
    return true;
}

void ViewImageView::scrollView(int hs, int vs)
{
    QScrollBar* hscroll = horizontalScrollBar();
    QScrollBar* vscroll = verticalScrollBar();
    if (hscroll)
    {
        hscroll->setValue(hs);
    }
    if (vscroll)
    {
        vscroll->setValue(vs);
    }
}

void ViewImageView::scrollView(QPoint dp)
{
    QScrollBar* hscroll = horizontalScrollBar();
    QScrollBar* vscroll = verticalScrollBar();
    if (hscroll)
    {
        int v = hscroll->value();
        hscroll->setValue(v - dp.x());
        dpcount.setX(v - dp.x());
    }
    if (vscroll)
    {
        int v = vscroll->value();
        vscroll->setValue(v - dp.y());
        dpcount.setY(v - dp.y());
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
