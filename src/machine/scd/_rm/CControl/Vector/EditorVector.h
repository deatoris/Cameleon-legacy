#ifndef EDITORVECTOR_H
#define EDITORVECTOR_H

#include<CControl.h>
#include<QtGui>
class EditorVector :  public CControl
{
    Q_OBJECT
public:
    EditorVector(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();

public slots:
    void geInformation();
    void addelement();
    void rmelement();
private:
    QTableWidget *box;
    QPushButton* button;
    QPushButton* buttonrm;
    QPushButton* save;
    bool test;
};

#endif // EDITORVECTOR_H
