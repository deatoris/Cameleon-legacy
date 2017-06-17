#ifndef VIEWVECTOR_H
#define VIEWVECTOR_H

#include<CControl.h>
#include<QtGui>
class ViewVector :  public CControl
{

public:
    ViewVector(QWidget * parent = 0);
        virtual CControl * clone();

    void updatePlugInControl(int indexplugin, CData* data);

private:
    QListWidget *box;

};
#endif // VIEWVECTOR_H
