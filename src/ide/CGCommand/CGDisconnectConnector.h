#ifndef CGDISCONNECTCONNECTOR_H
#define CGDISCONNECTCONNECTOR_H
#include "CControl.h"
#include "../CGComposer/CGCompositionItem.h"
#include "../CGCommandManager.h"

class CGDisconnectConnector : public CGCommand
{
public:
    CGDisconnectConnector();
    virtual void exec();
    virtual void unexec();

    void setConnector(CGConnector* connector);

    void setControl(CControl* control);
    void setControlConnector(CGConnector* controlconnector);
private:
    CGConnector* connector;
    CGConnector* wasConnectedTo;
    CGArrow* arrow;

    CControl* control;
    CGConnector* controlconnector;
};

#endif // CGDISCONNECTCONNECTOR_H
