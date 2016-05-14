#ifndef SELECTORVECTOR_H
#define SELECTORVECTOR_H

#include<CControl.h>
#include<QtGui>
class SelectorVector :  public CControl
{
    Q_OBJECT
public:
    SelectorVector(QWidget * parent = 0);
    virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void updatePlugInControl(int indexplugin, CData* data);
public slots:
    void geInformation(int i);
private:
    QComboBox *box;
    bool test;
    vector<string> _v_key;
    int _index;
};

#endif // SELECTORVECTOR_H
