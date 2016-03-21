#ifndef editorstringtext2_H
#define editorstringtext2_H

#include <QtGui>
#include "CControl.h"

class editorstringtext2 :  public CControl
{
    Q_OBJECT
public:
    editorstringtext2(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void keyPressEvent ( QKeyEvent * keyEvent );
    void updatePlugInControl(int indexplugin,CData* data);
public slots:
    void geInformation();
private:
    bool test;
    QTextEdit*textEdit;
    QString val;
};
#endif // editorstringtext2_H
