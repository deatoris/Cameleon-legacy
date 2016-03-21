#include "ColorSurface.h"

#include "DataNumber.h"
ColorSurface::ColorSurface(QWidget * parent)
    :CControl(parent)
{
    this->path().push_back("Introspection");
    this->setName("ColorSurface");
    this->setKey("ColorSurface");
    this->structurePlug().addPlugIn(DataNumber::KEY,"R.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"G.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"B.num");

    r = 255;
    g = 255;
    b = 255;

    label = new QLabel();
    QPalette palette;

    QColor c;
    c.setRgb(r,g,b);
    palette.setColor(label->backgroundRole(),c);

    label->setPalette(palette);
    label->setAutoFillBackground(true);

    lay = new QVBoxLayout;
    lay->addWidget(label);
    this->setLayout(lay);
}

CControl * ColorSurface::clone(){
    return new ColorSurface();
}
void ColorSurface::updatePlugInControl(int id, CData*data ){
//    QGraphicsView* view = CGLayoutManager::getInstance()->getCurrentLayout()->getView();
    if(id == 0 ){
        int c = dynamic_cast<DataNumber *>(data)->getValue();
        r = c;
    }
    if(id == 1){
        int c = dynamic_cast<DataNumber *>(data)->getValue();
        g = c;
    }
    if(id == 2 ){
        int c = dynamic_cast<DataNumber *>(data)->getValue();
        b = c;
    }

    QPalette palette;

    lay->removeWidget(label);
    label->hide();
    label = new QLabel();
    QColor c;
    c.setRgb(r,g,b);

    palette.setColor(label->backgroundRole(),c);

    label->setPalette(palette);
    label->setAutoFillBackground(true);
    lay->addWidget(label);
    this->update();

}
