/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGModel_H
#define CGModel_H
#include <QtGui>
class CGCompositionModel;
class CGOperatorModel;
class CGConnector;
class CGConnectorModel;
class CGShape;
class CGText;
class CGOperator;
class CGMulti;
class CGComposition;
class CGCompositionView;
class CGComposer;
#include "COperator.h"
#include "CData.h"

class CGCompositionModel : public QObject
{
    Q_OBJECT

public:
    CGCompositionModel();
    CGCompositionModel(CGComposer* composer);
    CGCompositionModel(CGCompositionModel* parentComposition);

    void initialise();

    void save();
    void saveAs();

    QString getName();
    void setName(QString name);

    QString getPath();
    void setPath(QString path);

    CGOperatorModel* addOperatorByName(QString name);

    void activate(QGraphicsItem* item);
    void unactivate(QGraphicsItem* item);

    CGCompositionModel* getParentCompositionModel();
    void setParentCompositionModel(CGCompositionModel* composition);

    CGOperatorModel* getOperatorById(int id);
    CGCompositionModel* getModelById(int id);

    void setView(CGMulti* multiView);

    CGMulti* getOuterView();
    CGMulti* getInnerView();

    void setScene(CGComposition* scene);
    CGComposition* getScene();

    CGCompositionView* getSceneView();
    void setSceneView(CGCompositionView* view);

    void setId(int id);
    int getId();

    QList<CGOperatorModel*> getOperatorModelList();
    QList<CGCompositionModel*> getCompositionModelList();
    QList<CGShape*> getShapeModelList();
    QList<CGText*> getTextModelList();

    bool belongToThis(CGConnector* conn);

    static int sid;

    QString getInformation();
    void setInformation(QString infos);
    void replace(CGOperatorModel* oldmodel, CGOperatorModel* newmodel);
private:
    QString infos;
    int id;
    QString name;
    QString path;
    CGCompositionModel* parentComposition; //if null, top compo
    CGMulti* multiView; //null if parent is null
    CGComposition* scene;
    CGCompositionView* sceneview;
    QList<CGCompositionModel*> modelsList;
    QList<CGOperatorModel*> operatorsList;
    QList<CGShape*> shapesList;
    QList<CGText*> textsList;

    QMap<int, CGOperatorModel*> operatorMap;
    QMap<int, CGCompositionModel*> multiMap;
};


class CGOperatorModel
{
public:
    CGOperatorModel();

    void addConnector(CGConnectorModel* connector);

    int getNumPlugIn();
    int getNumPlugOut();
    QString getKey();
    void setKey(QString key);
    QList<CGConnectorModel*> getConnectorsList();
    void setView(CGOperator* operatorView);
    CGOperator* getView();
    void setId(int id);
    int getId();
    void setPos(QPointF scenePos);
    QPointF getPos();
    void setName(QString name);
    CGConnectorModel* getConnectorById(int id);
    QString getName();
    bool has(CGConnectorModel* connector);
    QString getInformation();
    void setInformation(QString information);
    void setActionList(vector<string> actionList);
    vector<string> getActionList();
    void setNumPlugIn(int num);
    void setWaiting(bool updateWaiting);
    bool getWaiting();
private:
    bool isUpdateWaiting;
    vector<string> actionList;
    QString key;
    int id;
    int numPlugIn;
    QString name;
    QPointF scenePos;
    CGOperator* operatorView;
    QList<CGConnectorModel*> connectorsList;
    QMap<int, CGConnectorModel*> connectorMap;
    QString information;

};

class CGConnectorModel
{
public:
    CGConnectorModel(CGOperatorModel* parent);
    void setDataType(CData::Key dataType);
    QString getDataType();
    void setSide(QString side);
    QString getSide();
    int getId();
    void setId(int id);
    void connect(CGConnectorModel* connectedConnector);
    void disconnect();
    bool getIsConnected();
    void setName(QString name);
    QString getName();
    CGConnectorModel* getConnectedConnector();
    CGOperatorModel* getCGOperatorModel();
private:
    QString name;
    int id;
    CData::Key dataType;
    QString side;
    CGConnectorModel* connectedConnector;
    CGOperatorModel* parent;
    QPoint position;
};

class CGShapeModel
{
public:
    CGShapeModel();
};

class CGTextModel
{
public:
    CGTextModel();
};

#endif // CGModel_H
