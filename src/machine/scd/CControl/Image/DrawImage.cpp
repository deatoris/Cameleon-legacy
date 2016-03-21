#include "DrawImage.h"

#include<DataImageQt.h>
#include<DataNumber.h>

#include<CGProject.h>
#include<cmath>
DrawImage::DrawImage(QWidget * parent)
    :CControl(parent),_test(false)
{
    this->path().push_back("Image");
    this->setName("DrawImage");
    this->setKey("DrawImage");
    this->structurePlug().addPlugIn(DataNumber::KEY,"width.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"height.num");
    this->structurePlug().addPlugIn(DataImageQt::KEY,"initdraw.pgm");
    this->structurePlug().addPlugOut(DataImageQt::KEY,"draw.pgm");


    QVBoxLayout *layout = new QVBoxLayout;
    scene = new QGraphicsScene();
    view = new DrawImageView(scene);
    view->setControl(this);

    scene->setSceneRect(QRectF(-10000, -5000, 20000, 10000));
    scene->setBackgroundBrush(Qt::lightGray);

    proxy = NULL;

    QHBoxLayout *hlayout = new QHBoxLayout;

    this->createCommentBar();
    hlayout->addWidget(bar);
    layout->addLayout(hlayout);
    layout->addWidget(view);
    this->setLayout(layout);

    mode = Navigation;
    currentShapeType = Square;

    wi = 512;
    hei = 512;

    this->createBlankImage();

    currentColor = Qt::white;
    norm =2;
    currentPaintSize = 1;
    initBall();
    this->setMinimumSize(400,300);
}

void DrawImage::createBlankImage(){
    image = new QImage(wi, hei, QImage::Format_RGB32);

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::AlternateBase);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    imageLabel->setScaledContents(true);

    proxy = scene->addWidget(imageLabel);

    QRectF rect = imageLabel->rect();
    view->setSceneRect(rect);
}

void DrawImage::resizeImage(int width,int height){
    QImage* prev = image;
    image = new QImage(width, height, QImage::Format_RGB32);
    for(int i=0;prev->width()>i;i++){
        for(int j=0;prev->height()>j;j++){
            if(image->valid(i,j) && prev->valid(i,j)) image->setPixel(i,j,prev->pixel(i,j));
        }
    }
    wi = width;
    hei = height;
    imageLabel->resize(wi,hei);
    QRectF rect = imageLabel->rect();
    view->setSceneRect(rect);
}

void DrawImage::updatePlugInControl(int indexplugin,CData* data){


        if(indexplugin == 0){
            DataNumber * num = dynamic_cast<DataNumber *>(data);
            this->resizeImage(num->getValue(),hei);
        }else if(indexplugin == 1){
            DataNumber * num = dynamic_cast<DataNumber *>(data);
            this->resizeImage(wi,num->getValue());
        }else if(indexplugin == 2){
            DataImageQt * imageGrid = dynamic_cast<DataImageQt *>(data);
            image = new QImage(imageGrid->toString().c_str());
            wi = image->width();
            hei = image->height();
            this->resizeImage(wi,hei);
        }

        imageLabel->setPixmap(QPixmap::fromImage(*image));
        imageLabel->update();

}

void DrawImage::createCommentBar(){

    bar = new QToolBar();

    pencilAction = new QAction(QIcon(":/icons/pencil.png"),
                               tr("open project ..."), this);
    pencilAction->setShortcut(tr("Ctrl+O"));
    pencilAction->setStatusTip(tr("open composition ..."));
    pencilAction->setCheckable(true);
    pencilAction->setChecked(false);
    connect(pencilAction, SIGNAL(triggered()),
            this, SLOT(paintMode()));

    eyeAction = new QAction(QIcon(":/icons/eye.png"),
                            tr("open project ..."), this);
    eyeAction->setShortcut(tr("Ctrl+O"));
    eyeAction->setCheckable(true);
    eyeAction->setChecked(true);
    eyeAction->setStatusTip(tr("open composition ..."));
    connect(eyeAction, SIGNAL(triggered()),
            this, SLOT(navigationMode()));

    colorToolButton = new QToolButton;
    colorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    colorToolButton->setMenu(createColorMenu(SLOT(colorChanged()),
                                             Qt::white));
    colorToolButton->setIcon(createColorToolButtonIcon(Qt::white));
    connect(colorToolButton, SIGNAL(clicked()),
            this, SLOT(buttonTriggered()));

    shapeToolButton = new QToolButton;
    shapeToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    shapeToolButton->setMenu(createShapeMenu(SLOT(shapeChanged()),
                                             Square));
    shapeToolButton->setIcon(createShapeToolButtonIcon(Square));
    connect(shapeToolButton, SIGNAL(clicked()),
            this, SLOT(buttonTriggered()));

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
        //qDebug << "[WARN] Can't connect save button to control" ;
    }
    bar->addSeparator();
    bar->addAction(eyeAction);
    bar->addSeparator();
    bar->addAction(pencilAction);
    bar->addWidget(colorToolButton);
    bar->addWidget(shapeToolButton);
    bar->addWidget(fontSizeCombo);
    bar->addSeparator();
    bar->addWidget(save);
    bar->addWidget(clear);

}

void DrawImage::clear(){
    image = new QImage(wi, hei, QImage::Format_RGB32);

    imageLabel->setPixmap(QPixmap::fromImage(*image));

    QRectF rect = imageLabel->rect();
    view->setSceneRect(rect);
    imageLabel->update();
}

void DrawImage::save(){

    _test=true;
    this->apply();

}
void DrawImage::apply(){
    if(_test==true)
    {
        QString file;
        file =  CGProject::getInstance()->getTmpPath()+"/"+this->getKey().c_str()+QString::number(this->getId())+".pgm";
        image->save(file);
        DataImageQt * b = new DataImageQt;
        b->setFile(file.toStdString());
        this->sendPlugOutControl(0,b,CPlug::NEW);
    }
}
void DrawImage::initBall()
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
int DrawImage::paintSizeChanged(){
    currentPaintSize = fontSizeCombo->currentText().toInt();
    initBall();
    return fontSizeCombo->currentText().toInt();

}


int DrawImage::getMode(){
    return mode;
}

void DrawImage::shapeChanged(){
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

void DrawImage::colorChanged(){
    currentColor = qVariantValue<QColor>(qobject_cast<QAction *>(sender())->data());
    colorToolButton->setIcon(createColorToolButtonIcon(currentColor));
}

void DrawImage::navigationMode(){
    pencilAction->setChecked(false);
    eyeAction->setChecked(true);
    mode = Navigation;
}

void DrawImage::paintMode(){
    pencilAction->setChecked(true);
    eyeAction->setChecked(false);
    mode = Paint;
}

QMenu *DrawImage::createShapeMenu(const char *slot, ShapeType defaultShapeType)
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

QIcon DrawImage::createShapeToolButtonIcon(ShapeType shape)
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

QIcon DrawImage::createShapeIcon(ShapeType shape)
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

QMenu *DrawImage::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black
           << Qt::white
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
    names <<tr("black")
         <<tr("white")
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

QIcon DrawImage::createColorToolButtonIcon(QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, 50, 80), color);
    return QIcon(pixmap);
}

QIcon DrawImage::createColorIcon(QColor color)
{
    QPixmap pixmap(40, 40);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 40, 40), color);

    return QIcon(pixmap);
}

void DrawImage::loadImage(QString fileName){
    if(proxy!=NULL) scene->removeItem(proxy);
    if(proxy!=NULL) delete proxy;
    if (!fileName.isEmpty()) {
        QImage image(fileName);
        imageLabel = new QLabel;

        imageLabel->setBackgroundRole(QPalette::Base);
        imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
        imageLabel->setScaledContents(true);
        imageLabel->setPixmap(QPixmap::fromImage(image));
        imageLabel->setScaledContents(true);

        proxy = scene->addWidget(imageLabel);

        QRectF rect = imageLabel->rect();
        view->setSceneRect(rect);
    }
}

void DrawImage::drawPoint(QPointF point){
    //qDebug<<"void DrawImage::drawPoint() " << point ;
    //TODO: use currentPaintSize
    vector<pair<int,int> >::iterator it;
    for(it=_v_ball.begin();it!=_v_ball.end();it++)
    {
        image->setPixel(point.x()+it->first,point.y()+it->second ,currentColor.rgb());
    }
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    imageLabel->update();
}

CControl * DrawImage::clone(){
    return new DrawImage();
}

DrawImageView::DrawImageView(QGraphicsScene * scene, QWidget * parent)
    : QGraphicsView(scene,parent)
{
    scaleNum = 0;
    setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOff);
    leftIsPressed = false;
    this->setAcceptDrops(true);
    duration = 10000;

}

void DrawImageView::resizeEvent ( QResizeEvent *  ){

}

void DrawImageView::wheelEvent(QWheelEvent *e)
{
    if(this->control->getMode() == DrawImage::Navigation){
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

void DrawImageView::mouseMoveEvent ( QMouseEvent * e ){
    if(leftIsPressed && this->control->getMode() == DrawImage::Navigation){
        QPoint dp = previousPos - e->pos();
        scrollView(-dp);
        previousPos = e->pos();
    }else if(leftIsPressed ){
        this->control->drawPoint(mapToScene(e->pos()));
    }
}

bool DrawImageView::scaleView(qreal scaleFactor)
{
    scale(scaleFactor, scaleFactor);
    return true;
}

void DrawImageView::scrollView(QPoint dp)
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

void DrawImageView::mousePressEvent ( QMouseEvent * e ){
    leftIsPressed = true;
    if(e->button() == Qt::LeftButton && this->control->getMode() == DrawImage::Navigation){
        previousPos = e->pos();
    }else if(e->button() == Qt::LeftButton ){
        this->control->drawPoint(mapToScene(e->pos()));
    }
}

void DrawImageView::mouseReleaseEvent ( QMouseEvent * e ){
    leftIsPressed = false;
    QGraphicsView::mouseReleaseEvent(e);
}

void DrawImageView::keyPressEvent ( QKeyEvent *  ){
}

void DrawImageView::keyReleaseEvent ( QKeyEvent *  ){
}

void DrawImageView::setControl(DrawImage* control){
    this->control = control;
}
