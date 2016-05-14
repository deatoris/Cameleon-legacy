#ifndef COLORSURFACE_H
#define COLORSURFACE_H

#include<CControl.h>
#include<QtGui>
class ColorSurface :  public CControl
{
    Q_OBJECT
public:
    ColorSurface(QWidget * parent = 0);
    virtual CControl * clone();
    void updatePlugInControl(int ,CData*data);
private:
    int r;
    int g;
    int b;
    QLabel* label;
    QVBoxLayout *lay;
};

#endif // COLORSURFACE_H
