#ifndef VIEWSTRING_H
#define VIEWSTRING_H
#include <QtGui>
#include "CControl.h"
class ViewString :  public CControl
{
    Q_OBJECT
public:
    ViewString(QWidget * parent = 0);
        virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data);

public slots:
    void geInformation();
private:
    QTextBrowser *label;
};

#endif // VIEWSTRING_H
