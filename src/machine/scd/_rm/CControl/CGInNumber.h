#ifndef CGINNUMBER_H
#define CGINNUMBER_H
#include<CControl.h>
#include<QtGui>
class CGInNumber :  public CControl
{
    Q_OBJECT
public:
    CGInNumber(QWidget * parent = 0);
        virtual CControl * clone();

    void updatePlugInControl(int indexplugin,CData* data,CPlug::State state);
    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation();
private:
    QPushButton* applyButton;
    QLineEdit *lineEdit;
    double val;
    bool test;
};

#endif // CGINNUMBER_H
