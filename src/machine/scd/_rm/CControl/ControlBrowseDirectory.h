#ifndef CONTROLBROWSEDIRECTORY_H
#define CONTROLBROWSEDIRECTORY_H

#include<CControl.h>
#include<QtGui>
#include<string>
using namespace std;
class ControlBrowseDirectory :  public CControl
{
    Q_OBJECT
public:
    ControlBrowseDirectory(QWidget * parent = 0);
    virtual CControl * clone();
    virtual string toString();
    virtual void fromString(string str);
    void updatePlugInControl(int id, CData* data);

    void apply();
public slots:
    void geInformation();
private:
    QPushButton* applyButton;
    string _file;
    QStringList filterList;
    QString filter;
    bool record;
};
#endif // CONTROLBROWSEDIRECTORY_H
