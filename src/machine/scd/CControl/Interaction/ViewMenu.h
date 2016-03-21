#ifndef VIEWMENU_H
#define VIEWMENU_H

#include<CControl.h>
#include<QtGui>
#include<string>
using namespace std;
class ViewMenu :  public CControl
{
    Q_OBJECT
public:
    ViewMenu(QWidget * parent = 0);
    virtual CControl * clone();
    void updatePlugInControl(int indexplugin, CData* data);

    void apply();
public slots:
    virtual void actionClicked();
protected:
    QMenuBar* bar;
    QString currentActionName;
};

#endif // VIEWMENU_H
