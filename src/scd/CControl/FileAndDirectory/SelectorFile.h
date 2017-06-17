#ifndef SELECTORFILE_H
#define SELECTORFILE_H

#include<CControl.h>
#include<QtGui>
class SelectorFile :  public CControl
{
    Q_OBJECT
public:
    SelectorFile(QWidget * parent = 0);
        virtual CControl * clone();

    virtual string toString();
    virtual void fromString(string str);
    void apply();
public slots:
    void geInformation(const QModelIndex & index);
private:
    QTreeView* tree;
    QFileSystemModel* model;
    QString currentPath;
    bool test;
};
#endif // SELECTORFILE_H
