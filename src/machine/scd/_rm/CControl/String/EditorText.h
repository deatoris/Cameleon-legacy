#ifndef EDITORTEXT_H
#define EDITORTEXT_H

#include <QtGui>
#include "CControl.h"

class EditorText :  public CControl
{
public:
    EditorText(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void keyPressEvent ( QKeyEvent * keyEvent );

private:
    bool test;
    QTextEdit*textEdit;
    QString val;
};
#endif // EDITORTEXT_H
