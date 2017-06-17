#ifndef LISTBOXVECTOR_H
#define LISTBOXVECTOR_H

#include<CControl.h>
#include<QtGui>
class ListBoxVector :  public CControl
{
    Q_OBJECT
public:
    ListBoxVector(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void updatePlugInControl(int indexplugin, CData* data);
public slots:
    void geInformation();
private:
    QComboBox *box;
    bool test;
};
#endif // LISTBOXVECTOR_H
