#ifndef BACKGROUNDCONTROL_H
#define BACKGROUNDCONTROL_H
#include<CControl.h>
#include<QtGui>
class BackgroundControl :  public CControl
{
    Q_OBJECT
public:
    BackgroundControl(QWidget * parent = 0);
    virtual CControl * clone();


    static string KEY;


};

#endif // BACKGROUNDCONTROL_H
