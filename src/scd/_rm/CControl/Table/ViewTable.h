#ifndef VIEWTABLE_H
#define VIEWTABLE_H

#include<CControl.h>
#include<QtGui>
class ViewTable :  public CControl
{

public:
    ViewTable(QWidget * parent = 0);
    virtual CControl * clone();

    void updatePlugInControl(int indexplugin, CData* data);

private:
    QTableWidget *box;
};

#endif // VIEWTABLE_H
