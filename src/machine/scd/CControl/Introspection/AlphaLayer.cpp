#include "AlphaLayer.h"
#include "DataNumber.h"
#include "DataImageQt.h"
#include "DataBoolean.h"
#include "CGLayoutManager.h"
#include "CGProject.h"
AlphaLayer::AlphaLayer(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Introspection");
    this->setName("Snapshot");
    this->setKey("snapshot");
    this->structurePlug().addPlugIn(DataBoolean::KEY,"takeapicture.bool");
    this->structurePlug().addPlugOut(DataImageQt::KEY,"picture.bmp");

    this->setMinimumSize(500,300);
    this->hide();
}

void AlphaLayer::updatePlugInControl(int ,CData* ){
//    QGraphicsView* view = CGLayoutManager::getInstance()->getCurrentLayout()->getView();

    QString fileName = CGProject::getInstance()->getTmpPath()+"/"+this->getKey().c_str()+QString::number(this->getId())+".bmp";
    CGLayoutItem* item = CGLayoutManager::getInstance()->getItemById(this->getId());
    QRect r = item->boundingRect().toRect();
    QPointF p(item->pos().x(),item->pos().y());
    r.setX(item->pos().x());
    r.setY(item->pos().y());
    r.setWidth(item->getRect().toRect().width());
    r.setHeight(item->getRect().toRect().height());
    QGraphicsView* view = new QGraphicsView(item->scene());

    item->hide();
    QPixmap pixMap = QPixmap::grabWidget(view->viewport(),item->pos().x(),item->pos().y(),r.width(),r.height());
    pixMap.save(fileName);
    item->show();
    //save data
    DataImageQt * b = new DataImageQt;
    b->setFile(fileName.toStdString());
    this->sendPlugOutControl(0,b,CPlug::NEW);

}

CControl * AlphaLayer::clone(){
    return new AlphaLayer();
}

