#ifndef EDITORMAP_H
#define EDITORMAP_H

#include<CControl.h>
#include<QtGui>
#include "DataMap.h"

class EditorMap :  public CControl
{
    Q_OBJECT
public:
    EditorMap(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();

public slots:
    void addelement();
    void geInformation();

private:
    DataMap::DATAMAP  _data;
    QTableWidget *box;
    QPushButton* button;
    QPushButton* save;
    bool test;
};

#endif // EDITORMAP_H
