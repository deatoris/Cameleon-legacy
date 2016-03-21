#ifndef EDITORSTRING_H
#define EDITORSTRING_H

#include <QtGui>
#include "CControl.h"

class EditorString :  public CControl
{
    Q_OBJECT
public:
    EditorString(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void keyPressEvent ( QKeyEvent * keyEvent );

protected:
    bool test;
    QLineEdit *lineEdit;
    QString val;
};
#endif // EDITORSTRING_H
