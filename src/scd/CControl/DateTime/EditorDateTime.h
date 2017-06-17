#ifndef EditorDateTimeTIME_H
#define EditorDateTimeTIME_H

#include<CControl.h>
#include<QtGui>
class EditorDateTime :  public CControl
{
    Q_OBJECT
public:
    EditorDateTime(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation();
private:
    QDateTimeEdit *box;
    bool test;
};

#endif // EditorDateTimeTIME_H
