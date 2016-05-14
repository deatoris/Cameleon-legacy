#ifndef slidernumber2_H
#define slidernumber2_H

#include<CControl.h>
#include<QtGui>
class slidernumber2 :  public CControl
{
    Q_OBJECT
public:
    slidernumber2(QWidget * parent = 0);
    virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data);
    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation();
    void sliderMove(int m);
    void next();
    void previous();

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
#endif // slidernumber2_H
