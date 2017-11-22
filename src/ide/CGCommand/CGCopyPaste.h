#ifndef CGCOPYPASTE_H
#define CGCOPYPASTE_H
#include "../CGCommandManager.h"
#include "../CGComposer/CGCompositionModel.h"
#include "../CGComposer/CGCompositionItem.h"

class CGCopyPaste
{
public:
    CGCopyPaste();

    void copy(QList<QGraphicsItem*> items);
    void paste(CGComposition* scene,QPointF pos);

private:
    CGCompositionModel* parentComposition;
    QList<CGCompositionModel*> modelsList;
    QList<CGOperatorModel*> operatorsList;
    QList<CGShape*> shapesList;
    QList<CGText*> textsList;

    QList<CGMulti*> modelsListCopy;
    QList<CGOperator*> operatorsListCopy;
    QList<CGShape*> shapesListCopy;
    QList<CGText*> textsListCopy;

    QMap<CGConnector*, CGConnector*> mapNewPrevious;
    QMap<CGConnector*, CGConnector*> mapPreviousNew;
    QMap<CGConnector*, CGConnector*> mapPreviousConnection;

    QMap<CGConnector*, CGConnector*> recordConnectedConnector;

    bool belongToThis(CGConnector* conn);
};

#endif // CGCOPYPASTE_H
