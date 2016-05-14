#ifndef VIEWBOOLEAN_H
#define VIEWBOOLEAN_H

#include<CControl.h>
#include<QtGui>
class ViewBoolean :  public CControl
{
    Q_OBJECT
public:
    ViewBoolean(QWidget * parent = 0);
        virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data);

public slots:
    void geInformation();
private:
    QLabel *label;
};

#endif // VIEWBOOLEAN_H
