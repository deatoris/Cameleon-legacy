#include "MarkerImage.h"

#include<DataImageGrid.h>
#include<DataImageQt.h>
#include<DataNumber.h>
#include <CGProject.h>
MarkerImage::MarkerImage(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Image");
    this->setName("MarkerImage");
    this->setKey("MarkerImage");
    this->structurePlug().plugIn().clear();
    this->structurePlug().plugIn().clear();
    this->structurePlug().addPlugIn(DataImageGrid::KEY,"background.bmp");
    this->structurePlug().addPlugIn(DataImageGrid::KEY,"marker.bmp");
    this->structurePlug().addPlugOut(DataImageGrid::KEY,"draw.bmp");


    layout = new QVBoxLayout;
    scene = new QGraphicsScene();
    view = new MarkerImageView(scene);
    view->setControl(this);
    scene->setSceneRect(QRectF(0, 0, 10000, 10000));
    scene->setBackgroundBrush(Qt::black);
    imageLabel = new QLabel;
    proxy = scene->addWidget(imageLabel);;

    QHBoxLayout *hlayout = new QHBoxLayout;

    this->createCommentBar();
    hlayout->addWidget(bar);
    layout->addLayout(hlayout);
    layout->addWidget(view);
    this->setLayout(layout);

    mode = Navigation;
    currentShapeType = Square;

    QString qstr=  CGProject::getInstance()->getRessourcePath()+"/"+this->getKey().c_str()+QString::number(this->getId())+".png";
    _file = qstr.toStdString();

    currentColor = Qt::white;
    norm =2;
    currentPaintSize = 1;
    initBall();
    image = new QImage;
    imagebackground = new QImage;
    imageToSave= new QImage;
}
void MarkerImage::setView(){

    QRect rect1 =  image->rect();
    imageLabel->setGeometry(rect1);
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    scene->setSceneRect(rect1);
}
//string MarkerImage::toString(){
//    return "";
//    //    return _file;
//}

//void MarkerImage::fromString(string file){
//    //    if(file!=""){
//    //        delete image;
//    //        delete imageToSave;
//    //        delete imagebackground;
//    //        _file = file;
//    //        this->createBlankImage(file);
//    //    }
//}
void MarkerImage::convertRGB(QImage * &in){
    QImage* prev = in;
    in = new QImage(in->width(), in->height(), QImage::Format_RGB32);
    for(int i=0;in->width()>i;i++){
        for(int j=0;in->height()>j;j++){
            in->setPixel(i,j,prev->pixel(i,j));
        }
    }
    delete prev;
}
void MarkerImage::updatePlugInControl(int indexplugin ,CData* data){
    if(indexplugin==0 ){
        string str = data->toString();
        image->load(str.c_str());
        convertRGB(image);
        imagebackground->load(str.c_str());
        convertRGB(imagebackground);
        if(image->isNull()==false){

            if(imageToSave->width() == image->width() && imageToSave->height() == image->height() ){
                for(int i=0;image->width()>i;i++){
                    for(int j=0;image->height()>j;j++){
                        QRgb rgb  = imageToSave->pixel (i, j);
                        if(qRed( rgb )!=0||qGreen( rgb ) !=0||qBlue( rgb )!=0 )
                            image->setPixel(i,j,rgb);
                    }
                }
            }
            else{
                delete imageToSave;
                imageToSave = new QImage(image->width(), image->height(), QImage::Format_RGB32);
                for(int i=0;imageToSave->width()>i;i++){
                    for(int j=0;imageToSave->height()>j;j++){
                        QRgb rgb = QColor(0,0,0).rgb();
                        imageToSave->setPixel(i,j,rgb);
                    }
                }
            }
            setView();
            this->update();

        }
        else{
            this->error("Cannot read input image"+str);
        }
    }
    else if(indexplugin==1){
        string str = data->toString();
        imageToSave->load(str.c_str());
        convertRGB(imageToSave);
        if(imageToSave->width() == image->width() && imageToSave->height() == image->height() ){
            for(int i=0;image->width()>i;i++){
                for(int j=0;image->height()>j;j++){
                    QRgb rgb  = imageToSave->pixel (i, j);
                    QRgb rgb2  = imagebackground->pixel (i, j);
                    if(qRed( rgb )!=0||qGreen( rgb ) !=0||qBlue( rgb )!=0 )
                        image->setPixel(i,j,rgb);
                    else
                        image->setPixel(i,j,rgb2);
                }
            }
        }
        this->update();
    }



}

void MarkerImage::createCommentBar(){

    bar = new QToolBar();

    pencilAction = new QAction(QIcon(":/icons/pencil.png"),
                               tr("open project ..."), this);
    pencilAction->setCheckable(true);
    pencilAction->setChecked(false);
    connect(pencilAction, SIGNAL(triggered()),
            this, SLOT(paintMode()));

    eraserAction = new QAction(QIcon(":/icons/eraser-minus-icon.png"),
                               tr("open project ..."), this);
    eraserAction->setCheckable(true);
    eraserAction->setChecked(false);
    connect(eraserAction, SIGNAL(triggered()),
            this, SLOT(eraserMode()));


    eyeAction = new QAction(QIcon(":/icons/eye.png"),
                            tr("open project ..."), this);
    eyeAction->setCheckable(true);
    eyeAction->setChecked(true);
    connect(eyeAction, SIGNAL(triggered()),
            this, SLOT(navigationMode()));

    colorToolButton = new QToolButton;
    colorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    colorToolButton->setMenu(createColorMenu(SLOT(colorChanged()),
                                             Qt::white));
    colorToolButton->setIcon(createColorToolButtonIcon(Qt::white));
    //    connect(colorToolButton, SIGNAL(clicked()),
    //            this, SLOT(buttonTriggered()));

    shapeToolButton = new QToolButton;
    shapeToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    shapeToolButton->setMenu(createShapeMenu(SLOT(shapeChanged()),
                                             Square));
    shapeToolButton->setIcon(createShapeToolButtonIcon(Square));
    connect(shapeToolButton, SIGNAL(clicked()),
            this, SLOT(shapeChanged()));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 1; i < 200; i++)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(1, 200, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(paintSizeChanged()));


    QPushButton* save = new QPushButton("save");

    if(!QObject::connect(save, SIGNAL(clicked()),this, SLOT(save()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect save button to control" ;
    }
    QPushButton* clear = new QPushButton("clear");

    if(!QObject::connect(clear, SIGNAL(clicked()),this, SLOT(clear()),Qt::DirectConnection)){
        //qDebug << "[WARN] Can't connect clear button to control" ;
    }


    bar->addSeparator();
    bar->addAction(eyeAction);
    bar->addSeparator();
    bar->addAction(pencilAction);
    bar->addAction(eraserAction);
    bar->addWidget(colorToolButton);
    bar->addWidget(shapeToolButton);
    bar->addWidget(fontSizeCombo);
    bar->addSeparator();
    bar->addWidget(save);
    bar->addWidget(clear);

}

void MarkerImage::save(){
    _test=true;
    this->apply();
}
void MarkerImage::clear(){
    for(int x = 0; x < image->width(); x++)
    {
        for(int y = 0; y < image->height(); y++)
        {
            image->setPixel(x,y,imagebackground->pixel(x,y));
            imageToSave->setPixel(x,y,QColor(0,0,0).rgb());
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    imageLabel->update();
}

void MarkerImage::apply(){
    if(_test==true)
    {
        DataImageQt * b = new DataImageQt;
        b->setData(imageToSave);
        this->sendPlugOutControl(0,b,CPlug::NEW);
        _test=false;
    }
}


void MarkerImage::initBall()
{
    _v_ball.clear();
    for(int i =- ceil(currentPaintSize);i<= ceil(currentPaintSize);i++){
        for(int j =- ceil(currentPaintSize);j<= ceil(currentPaintSize);j++){
            if(norm!=0){
                if( pow(abs(pow(i,norm)) + abs(pow(j,norm)),1.0/norm )<currentPaintSize){
                    _v_ball.push_back(make_pair(i,j));
                }

            }else{
                if( max(i,j)<currentPaintSize){
                    _v_ball.push_back(make_pair(i,j));
                }
            }
        }

    }
}
int MarkerImage::paintSizeChanged(){
    currentPaintSize = fontSizeCombo->currentText().toInt();
    initBall();
    return fontSizeCombo->currentText().toInt();
}

int MarkerImage::getMode(){
    return mode;
}

void MarkerImage::shapeChanged(){
    int cur = qVariantValue<int>(qobject_cast<QAction *>(sender())->data());
    if(cur == 0){
        currentShapeType = Round;
        norm = 2; initBall();
    }else if(cur == 1){
        currentShapeType = Square;
        norm = 0; initBall();
    }else if(cur == 2){
        currentShapeType = Losange;
        norm = 1; initBall();
    }
    shapeToolButton->setIcon(createShapeToolButtonIcon(currentShapeType));
}

void MarkerImage::colorChanged(){
    currentColor = qVariantValue<QColor>(qobject_cast<QAction *>(sender())->data());
    colorToolButton->setIcon(createColorToolButtonIcon(currentColor));
}

void MarkerImage::navigationMode(){
    eraserAction->setChecked(false);
    pencilAction->setChecked(false);
    eyeAction->setChecked(true);

    mode = Navigation;
}

void MarkerImage::paintMode(){
    eraserAction->setChecked(false);
    pencilAction->setChecked(true);
    eyeAction->setChecked(false);
    mode = Paint;
}
void MarkerImage::eraserMode(){
    eraserAction->setChecked(true);
    pencilAction->setChecked(false);
    eyeAction->setChecked(false);
    mode = Eraser;
}
QMenu *MarkerImage::createShapeMenu(const char *slot, ShapeType defaultShapeType)
{
    QMenu *shapeMenu = new QMenu(this);
    QAction *action = new QAction("Square", this);
    action->setData(Square);
    action->setIcon(createShapeIcon(Square));
    connect(action, SIGNAL(triggered()),
            this, slot);
    shapeMenu->addAction(action);
    if (Square == defaultShapeType) {
        shapeMenu->setDefaultAction(action);
    }

    action = new QAction("Round", this);
    action->setData(Round);
    action->setIcon(createShapeIcon(Round));
    connect(action, SIGNAL(triggered()),
            this, slot);
    shapeMenu->addAction(action);
    if (Round == defaultShapeType) {
        shapeMenu->setDefaultAction(action);
    }

    action = new QAction("Losange", this);
    action->setData(Losange);
    action->setIcon(createShapeIcon(Losange));
    connect(action, SIGNAL(triggered()),
            this, slot);
    shapeMenu->addAction(action);
    if (Losange == defaultShapeType) {
        shapeMenu->setDefaultAction(action);
    }

    return shapeMenu;
}

QIcon MarkerImage::createShapeToolButtonIcon(ShapeType shape)
{
    QPixmap pixmap(50, 50);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    if(shape == Round){
        painter.drawEllipse(QRect(0, 0, 40, 40));
    }else if(shape == Square){
        painter.drawRect(QRect(0, 0, 40, 40));
    }else if(shape == Losange){
        QPolygon polygon;
        polygon << QPoint(20,0) << QPoint(0,20) << QPoint(20,40) << QPoint(40,20);
        painter.drawPolygon(polygon);
    }
    return QIcon(pixmap);
}

QIcon MarkerImage::createShapeIcon(ShapeType shape)
{
    QPixmap pixmap(50, 50);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    if(shape == Round){
        painter.drawEllipse(QRect(10, 10, 30, 30));
    }else if(shape == Square){
        painter.drawRect(QRect(10, 10, 30, 30));
    }else if(shape == Losange){
        QPolygon polygon;
        polygon << QPoint(20,0) << QPoint(0,20) << QPoint(20,40) << QPoint(40,20);
        painter.drawPolygon(polygon);
    }

    return QIcon(pixmap);
}

QMenu *MarkerImage::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::white
           << Qt::red
           << Qt::darkRed
           << Qt::green
           << Qt::darkGreen
           << Qt::blue
           << Qt::darkBlue
           << Qt::cyan
           << Qt::darkCyan
           << Qt::magenta
           << Qt::darkMagenta
           << Qt::yellow
           << Qt::darkYellow
           << Qt::gray
           << Qt::darkGray
           << Qt::lightGray;
    QStringList names;
    names <<tr("white")
         <<tr("red")
        <<tr("darkRed")
       <<tr("green")
      <<tr("darkGreen")
     <<tr("blue")
    <<tr("darkBlue")
    <<tr("cyan")
    <<tr("darkCyan")
    <<tr("magenta")
    <<tr("darkMagenta")
    <<tr("yellow")
    <<tr("darkYellow")
    <<tr("gray")
    <<tr("darkGray")
    <<tr("lightGray");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}

QIcon MarkerImage::createColorToolButtonIcon(QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, 50, 80), color);
    return QIcon(pixmap);
}

QIcon MarkerImage::createColorIcon(QColor color)
{
    QPixmap pixmap(40, 40);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 40, 40), color);

    return QIcon(pixmap);
}

void MarkerImage::drawPoint(QPointF point){
    //TODO: use currentPaintSize
    vector<pair<int,int> >::iterator it;
    for(it=_v_ball.begin();it!=_v_ball.end();it++)
    {
        if(point.x()+it->first>=0 && point.x()+it->first<image->width()&& point.y()+it->second>=0 &&  point.y()+it->second<image->height()){
            if(mode==Paint){
                image->setPixel(point.x()+it->first,point.y()+it->second ,currentColor.rgb());
                imageToSave->setPixel(point.x()+it->first,point.y()+it->second ,currentColor.rgb());
            }
            else{
                image->setPixel(point.x()+it->first,point.y()+it->second ,imagebackground->pixel(point.x()+it->first,point.y()+it->second ));
                QRgb rgb =QColor(0,0,0).rgb();
                imageToSave->setPixel(point.x()+it->first,point.y()+it->second ,rgb);
            }
        }
    }
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    imageLabel->update();
}

CControl * MarkerImage::clone(){
    return new MarkerImage();
}





MarkerImageView::MarkerImageView(QGraphicsScene * scene, QWidget * parent)



    : QGraphicsView(scene,parent)


{


    scaleNum = 0;



    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);



    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);



    leftIsPressed = false;



    this->setAcceptDrops(true);


    duration = 10000;




}



void MarkerImageView::resizeEvent ( QResizeEvent *  ){




}



void MarkerImageView::wheelEvent(QWheelEvent *e)



{


    if(this->control->getMode() == MarkerImage::Navigation){



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


    }



}



void MarkerImageView::mouseMoveEvent ( QMouseEvent * e ){



    if(leftIsPressed && this->control->getMode() == MarkerImage::Navigation){



        QPoint dp = previousPos - e->pos();



        scrollView(-dp);


        previousPos = e->pos();



    }else if(leftIsPressed ){



        this->control->drawPoint(mapToScene(e->pos()));



    }


}



bool MarkerImageView::scaleView(qreal scaleFactor)



{


    scale(scaleFactor, scaleFactor);


    return true;


}



void MarkerImageView::scrollView(QPoint dp)



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



void MarkerImageView::mousePressEvent ( QMouseEvent * e ){



    leftIsPressed = true;



    if(e->button() == Qt::LeftButton && this->control->getMode() == MarkerImage::Navigation){



        previousPos = e->pos();



    }else if(e->button() == Qt::LeftButton ){



        this->control->drawPoint(mapToScene(e->pos()));



    }


}



void MarkerImageView::mouseReleaseEvent ( QMouseEvent * e ){



    leftIsPressed = false;



    QGraphicsView::mouseReleaseEvent(e);


}



void MarkerImageView::keyPressEvent ( QKeyEvent *  ){



}



void MarkerImageView::keyReleaseEvent ( QKeyEvent *  ){



}



void MarkerImageView::setControl(MarkerImage* control){



    this->control = control;



}






