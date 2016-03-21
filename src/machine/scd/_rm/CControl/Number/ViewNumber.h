#ifndef VIEWNUMBER_H
#define VIEWNUMBER_H

#include<CControl.h>
#include<QtGui>
class ViewNumber :  public CControl
{
    Q_OBJECT
public:
    ViewNumber(QWidget * parent = 0);
        virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data);
public slots:
    void geInformation();
private:
    QLabel *label;
};

#endif // VIEWNUMBER_H
