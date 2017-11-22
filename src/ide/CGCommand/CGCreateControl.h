#ifndef CGCreateControl_H
#define CGCreateControl_H
#include "../CGCommandManager.h"
#include "CGComposition.h"
class CGCreateControl: public CGCommand
{
public:
    virtual void exec();
    virtual void unexec();
    CGCreateControl();

    void setEventPos(QPointF eventScenePos);
    void setName(QString name);
    void setType(int type);
    void setPath(QString path);
    void setScene(CGComposition* composition);
    void setItem(QGraphicsItem* item);
private:
    CGComposition* composition;
    QString name;
    QString path;
    QPointF eventScenePos;
    int typeItem;
    QGraphicsItem * item;
};

#endif // CGCreateControl_H
