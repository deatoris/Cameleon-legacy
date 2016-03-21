#ifndef VIEWMAP_H
#define VIEWMAP_H

#include<CControl.h>
#include<QtGui>
class ViewMap :  public CControl
{
    Q_OBJECT
public:
    ViewMap(QWidget * parent = 0);
        virtual CControl * clone();


    void updatePlugInControl(int indexplugin, CData* data);
public slots:
    void geInformation();
private:
    QTableWidget *box;
    QPushButton* button;
    QPushButton* save;
};

#endif // VIEWMAP_H
