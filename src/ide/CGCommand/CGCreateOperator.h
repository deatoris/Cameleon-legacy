#ifndef CGCREATEOPERATOR_H
#define CGCREATEOPERATOR_H
#include "../CGCommandManager.h"
#include "../CGComposer/CGCompositionModel.h"
#include "CGLayout.h"

class CGOperator;

class CGCreateOperator : public CGCommand
{
public:
    CGCreateOperator();
    virtual void exec();
    virtual void unexec();

    //mandatory
    void setEventPos(QPointF eventScenePos);
    void setType(int type);
    void setScene(CGComposition* scene);
    void setLayout(CGLayout* layout);

    //optional
    void setOperatorName(QString operatorName);
    void setItem(QGraphicsItem* item);
    void setRect(QRectF rect);
    QGraphicsItem* getItem();
    void setIsPattern(bool isPattern);
private:
    QGraphicsItem* item;
    QString operatorName;
    QPointF eventScenePos;
    int typeItem;
    CGComposition* scene;
    QRectF rect;
    CGLayout* layout;
    bool isPattern;
};

#endif // CGCREATEOPERATOR_H
