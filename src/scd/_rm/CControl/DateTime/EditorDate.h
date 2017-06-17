#ifndef EDITORDATE_H
#define EDITORDATE_H

#include<CControl.h>
#include<QtGui>
class EditorDate :  public CControl
{
    Q_OBJECT
public:
    EditorDate(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation();
private:
    QDateEdit *box;
    bool test;
};


#endif // EDITORDATE_H
