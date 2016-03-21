#ifndef CHANGELAYOUT_H
#define CHANGELAYOUT_H

#include<CControl.h>
#include<QtGui>
class ChangeLayout :  public CControl
{
    Q_OBJECT
public:
    ChangeLayout(QWidget * parent = 0);
    virtual CControl * clone();
    void updatePlugInControl(int indexplugin,CData* data);
private:
    QPixmap pixMap;

};

#endif // CHANGELAYOUT_H
