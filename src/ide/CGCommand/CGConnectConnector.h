#ifndef CGCONNECTCONNECTOR_H
#define CGCONNECTCONNECTOR_H
#include "CControl.h"
#include "../CGCommandManager.h"
#include "../CGComposer/CGCompositionManager.h"
#include "../CGComposer/CGCompositionModel.h"
#include "../CGComposer/CGComposition.h"
#include "../CGComposer/CGCompositionItem.h"
class CGConnectConnector : public CGCommand
{
public:
    CGConnectConnector();
    virtual void exec();
    virtual void unexec();
    void setStartItem(CGConnector* startItem);
    void setEndItem(CGConnector* endItem);
    void setColor(QColor color);
    void setMulti(CGMulti* multi);

    void setControl(CControl* control);
    void setControlConnector(CGConnector* controlconnector);
    void setConnector(CGConnector* connector);
private:
    CGArrow *arrow;

    QColor color;
    CGConnector* startItem;
    CGConnector* endItem;
    CGMulti* multi;

    CControl* control;
    int connectorid;
    CGConnector* connector;
    CGConnector* controlconnector;
};

#endif // CGCONNECTCONNECTOR_H
