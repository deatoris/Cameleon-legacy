/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGMERGE_H
#define CGMERGE_H
#include "../CGCommandManager.h"
#include "../CGModel.h"
class CGComposer;
class CGConnector;

class CGMerge : public CGCommand
{
public:
    CGMerge();
    virtual void exec();
    virtual void unexec();

    void setParentComposition(CGCompositionModel* parentComposition);
    void setOperatorsList(QList<CGOperatorModel*> operatorsList);
    void setModelList(QList<CGCompositionModel*> modelsList);
    void setShapesList(QList<CGShapeModel*> shapesList);
    void setTextsList(QList<CGTextModel*> textsList);
    void setName(QString name);
    void setMenu(QMenu* operatormenu, QMenu* connectormenu);
    void setEventPos(QPointF eventScenePos);
    void setComposer(CGComposer* composer);

private:
    bool belongToThis(CGConnector* conn);
    CGComposer* composer;
    CGCompositionModel* parentComposition;
    QList<CGCompositionModel*> modelsList;
    QList<CGOperatorModel*> operatorsList;
    QList<CGShapeModel*> shapesList;
    QList<CGTextModel*> textsList;
    QString name;
    QMenu* operatormenu;
    QMenu* connectormenu;
    QPointF eventScenePos;
    CGMulti* multi;
    CGCompositionModel* compositionModel;

    QMap<CGConnector*, CGConnector*> recordConnectedConnector;


};

#endif // CGMERGE_H
