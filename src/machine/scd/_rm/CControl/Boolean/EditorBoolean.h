#ifndef EDITORBOOLEAN_H
#define EDITORBOOLEAN_H

#include<CControl.h>
#include<QtGui>
class EditorBoolean :  public CControl
{
    Q_OBJECT
public:
    EditorBoolean(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation();
private:
    QComboBox *box;
    bool test;
};

#endif // EDITORBOOLEAN_H
