/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGCompositionManager_H
#define CGCompositionManager_H
#include <QtGui>
#include "CGCompositionModel.h"
#include "CGComposition.h"
#include "CGErrorDialog.h"
#include "ControlEditorArgument.h"
#include "CArguments.h"
#include "CGLayoutItem.h"
#include<QMutex>
class CGComposer;

class CGCompositionManager
{
public:
    static CGCompositionManager* getInstance();

    void init(CGComposer* composer);

    void showSceneFromModel(CGCompositionModel* compositionModel);

    void up();

    bool setArguments(QList<argument> commandArgs);
    QList<CGLayoutItem*> searchLayoutItemByType(QString layoutItemType);

    CGComposition * getCurrentComposerScene();
    void setCurrentCompositionModel(CGCompositionModel* currentCompositionModel);
    CGCompositionModel* getCurrentCompositionModel();
    CGComposer * getComposer();
    void clear();
    CGOperatorModel * searchOperatorById(int idOperator);
    QList<CGConnector*> searchCompatibleConnector(QString type, QString side, bool connected = false);
    QList<CGConnector*> searchAllConnector();
    void showConnected(CGConnector* connector);
    QList<CGConnector*> searchCompatibleConnectorForControl(QString dataType, QString side, bool connected);
    CGCompositionModel* getCompositionById(int id);
    QList<CGCompositionModel*> getCompositionList(CGCompositionModel* top);
    QList<CGOperatorModel*> getOperatorList(CGCompositionModel* top);
    QList<CGCompositionModel*> getCompositionList();
    CGConnector* getOtherSide(CGMultiConnector* tc);
    CGCompositionModel* updateStructOperator(int opId, COperatorStructure opstructure);

    //save&load managment
    QString createPattern(CGCompositionModel* model);
    CGCompositionModel* createCompositionFromPattern(QString pattern);
    CGCompositionModel* getParentCompositionModel();
    void reinit();

    void addOpeError(int opId, QString message);
    void addCtlError(int opId, QString message);
    void clearError();
    CGErrorDialog* getErrorDial();

    QMap<int,QString> getOpeErrorMap();
    QMap<int,QString> getCtlErrorMap();

private:
    QMutex _mutex;
    QMap<int,QString> errorOpeMap;
    QMap<int,QString> errorCtlMap;
    CGErrorDialog * error;
    CGCompositionManager();

    static CGCompositionManager* sceneManagerInstance;

    CGComposer* composer;
    CGComposition* composerScene;
    CGCompositionView* scenceview;
    CGCompositionModel* currentCompositionModel;
};

#endif // CGCompositionManager_H
