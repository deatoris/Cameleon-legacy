#ifndef EDITORNUMBER_H
#define EDITORNUMBER_H

#include<CControl.h>
#include<QtGui>
class EditorNumber :  public CControl
{
public:
    EditorNumber(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void keyPressEvent ( QKeyEvent * keyEvent );
private:
    bool test;
    QLineEdit *lineEdit;
    double val;
};

#endif // EDITORNUMBER_H
