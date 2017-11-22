#ifndef CGDELETEOPERATOR_H
#define CGDELETEOPERATOR_H
#include "../CGComposer/CGCompositionItem.h"
#include "../CGCommandManager.h"
#include "../CGComposer/CGComposition.h"
class CGDeleteOperator: public CGCommand
{
public:
    CGDeleteOperator();
    virtual void exec();
    virtual void unexec();
    void setItem(QGraphicsItem* item);
    void setScene(CGComposition* scene);
    void setMove(bool move);
private:
    QGraphicsItem* item;
    CGComposition* scene;
    bool move;

};

#endif // CGDELETEOPERATOR_H
