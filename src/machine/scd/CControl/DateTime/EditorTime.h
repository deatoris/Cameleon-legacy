#ifndef EDITORTIME_H
#define EDITORTIME_H

#include<CControl.h>
#include<QtGui>
class EditorTime :  public CControl
{
    Q_OBJECT
public:
    EditorTime(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation();
private:
    QTimeEdit *box;
    bool test;
};
#endif // EDITORTIME_H
