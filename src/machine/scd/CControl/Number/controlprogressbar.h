#ifndef CONTROLPROGRESSBAR_H
#define CONTROLPROGRESSBAR_H

#include<CControl.h>
#include<QtGui>
class ControlProgressBar :  public CControl
{
    Q_OBJECT
public:
    ControlProgressBar(QWidget * parent = 0);
    virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data);
private:
    QProgressBar *label;
};

#endif // CONTROLPROGRESSBAR_H
