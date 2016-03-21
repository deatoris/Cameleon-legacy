#ifndef ALPHALAYER_H
#define ALPHALAYER_H

#include<CControl.h>
#include<QtGui>
class AlphaLayer :  public CControl
{
    Q_OBJECT
public:
    AlphaLayer(QWidget * parent = 0);
    virtual CControl * clone();
    void updatePlugInControl(int indexplugin,CData* data);
private:
    QPixmap pixMap;

};

#endif // ALPHALAYER_H
