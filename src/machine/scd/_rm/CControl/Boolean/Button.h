#ifndef BUTTON_H
#define BUTTON_H

#include<CControl.h>
#include<QtGui>
class EditorButton :  public CControl
{
    Q_OBJECT
public:
    EditorButton(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void updatePlugInControl(int indexplugin,CData* data);

public slots:
    void geInformation();
private:
    QPushButton *button;
    bool test;
};

#endif // BUTTON_H
