#ifndef CGDELETECONTROL_H
#define CGDELETECONTROL_H
#include "../CGComposer/CGCompositionItem.h"
#include "../CGCommandManager.h"
#include "../CGComposer/CGComposition.h"
class CGDeleteControl: public CGCommand
{
public:
    CGDeleteControl();
    virtual void exec();
    virtual void unexec();
    void setItem(QGraphicsItem* item);
    void setScene(QGraphicsScene* scene);
private:
    QGraphicsScene* scene;
    QGraphicsItem* item;
};

#endif // CGDELETECONTROL_H
