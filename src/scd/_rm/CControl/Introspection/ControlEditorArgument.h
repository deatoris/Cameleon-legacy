#ifndef CONTROLEDITORARGUMENT_H
#define CONTROLEDITORARGUMENT_H
#include <QtGui>
#include <CControl.h>

class ControlEditorArgument:  public CControl
{
    Q_OBJECT
public:
    ControlEditorArgument(QWidget * parent = 0);
    virtual CControl * clone();
    virtual string toString();
    virtual void fromString(string str);
    void apply();
    void setArgument(string argument);
    static string KEY;
    string getKeyArgument();
    bool getOptional();
public slots:
    void geInformation();
private:
    string _argument;
    QLineEdit * _arg;
    QLineEdit * _default;
    QLineEdit * _info;
    QLabel * _l_arg;
    QLabel * _l_default;
    QLabel * _l_info;

    QLabel * _label;
    QCheckBox * _box;
};

#endif // CONTROLEDITORARGUMENT_H
