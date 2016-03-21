#ifndef EditorStringWithInputWITHINPUT_H
#define EditorStringWithInputWITHINPUT_H

#include <QtGui>
#include "EditorString.h"

class EditorStringWithInput :  public EditorString
{
    Q_OBJECT
public:
    EditorStringWithInput(QWidget * parent = 0);
    virtual CControl * clone();
    void updatePlugInControl(int , CData* data);
};

#endif // EditorStringWithInputWITHINPUT_H
