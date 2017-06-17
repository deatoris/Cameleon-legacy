#ifndef INFILE_H
#define INFILE_H

#include<CControl.h>
#include<QtGui>
#include<string>
using namespace std;
class InFile :  public CControl
{
    Q_OBJECT
public:
    InFile(QWidget * parent = 0);
    virtual CControl * clone();
    virtual string toString();
    virtual void fromString(string str);
    void updatePlugInControl(int indexplugin, CData* data);

    void apply();
public slots:
    virtual void geInformation();
protected:
    QPushButton* applyButton;
    string _file;
    QStringList filterList;
    QString filter;
    bool record;

};
class OutFile :  public InFile
{
    Q_OBJECT
public:
    OutFile(QWidget * parent = 0);
        virtual CControl * clone();
public slots:
    void geInformation();
};
#endif // INFILE_H
