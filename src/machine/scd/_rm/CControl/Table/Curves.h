#ifndef CURVES_H
#define CURVES_H

#include<CControl.h>
#include<QtGui>
class Curves :  public CControl
{
    Q_OBJECT
public:
    Curves(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);

    void updatePlugInControl(int indexplugin, CData* data);
public slots:
    void geInformation();
private:
    QTableWidget *box;
    QPushButton* button;
    QPushButton* save;
    bool test;
};

#endif // CURVES_H
