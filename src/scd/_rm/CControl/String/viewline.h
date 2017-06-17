#ifndef VIEWLINE_H
#define VIEWLINE_H

#include <QtGui>
#include "CControl.h"
class ViewLine :  public CControl
{
    Q_OBJECT
public:
    ViewLine(QWidget * parent = 0);
        virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data);

public slots:
    void geInformation();
private:
    QLineEdit *label;
};
#endif // VIEWLINE_H
