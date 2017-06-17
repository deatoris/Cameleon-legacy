#ifndef EDITORTABLE_H
#define EDITORTABLE_H

#include<CControl.h>
#include<QtGui>
#include "DataTable.h"

class EditorTable :  public CControl
{
    Q_OBJECT
public:
    EditorTable(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
protected:
    virtual QString defaultString();
    virtual QString defaultHeaderColumn(int col);
    virtual QString defaultHeaderRaw(int raw);
public slots:
    void addColumn();
    void addRaw();
    void rmColumn();
    void rmRaw();
    void geInformation();
    void showEdit(int edit);
    void changeColumnName();
protected:
    QHBoxLayout* bLay;
    QHBoxLayout* bLay1;
    QTableWidget *box;
    QPushButton* buttonraw;
    QPushButton* buttoncolumn;
    QPushButton* rmbuttonraw;
    QPushButton* rmbuttoncolumn;
    QPushButton* save;
    bool test;
    QMap<int, QLineEdit*> mapEdit;
    int currentEdit;
};

#endif // EDITORTABLE_H
