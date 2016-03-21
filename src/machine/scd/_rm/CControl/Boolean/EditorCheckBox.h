#ifndef EDITORCHECKBOX_H
#define EDITORCHECKBOX_H

#include<CControl.h>
#include<QtGui>
class EditorCheckBox :  public CControl
{
    Q_OBJECT
public:
    EditorCheckBox(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void updatePlugInControl(int ,CData* data,CPlug::State state);
public slots:
    void geInformation();
private:
    QCheckBox *box;
    bool test;
};

#endif // EDITORCHECKBOX_H
