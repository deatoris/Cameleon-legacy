#ifndef SLIDERNUMBER_H
#define SLIDERNUMBER_H

#include<CControl.h>
#include<QtGui>
class SliderNumber :  public CControl
{
    Q_OBJECT
public:
    SliderNumber(QWidget * parent = 0);
    virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data);
    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation();
    void sliderMove(int m);
private:
    bool test;
    QSlider * slider;
    QLabel* max;
    QLabel* min;
    QLabel* current;
    double valmax;
    double valmin;
    double step;
};
#endif // SLIDERNUMBER_H
