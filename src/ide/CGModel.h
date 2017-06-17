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
class CGScene;
class CGView;
class CGComposer;

class CGWorkspace
{
public:
    void setName(QString name);
    void setPath(QString path);
    void setLogPath(QString logPath);
    void setLibPath(QString libPath);
    void setCurrentComposition(QString currentComposition);
    void setTmpPath(QString tmpPath);
    void setRessourcePath(QString ressourcePath);
    void addRecentComposition(QString composition);
    void addInstalledLib(QString name, QString version);
    void removeInstalledLib(QString name, QString version);

    QString getName();
    QString getPath();
    QString getLogPath();
    QString getLibPath();
    QString getCurrentComposition();
    QString getTmpPath();
    QString getRessourcePath();
    QList<QString> getRecentCompositions();

    CGCompositionModel* getCurrentModel();

    void save();
    void createWorkspace(QString path);

    void loadComposition();

    void init(QString path, CGComposer* composer);

    static CGWorkspace* getInstance();

private:
    CGWorkspace();

    QString name;
    QString path;
    QString logPath;
    QString libPath;
    QString tmpPath;
    QString ressourcePath;
    QString currentComposition;
    QList<QString> recentCompositions;
    QMap<QString, QString> installedLib; //name, version

    CGCompositionModel* currentModel;

    static CGWorkspace* workspaceInstance;

};

class CGCompositionModel : public QObject
{
    Q_OBJECT

public:
    CGCompositionModel();
    CGCompositionModel(CGComposer* composer);
    CGCompositionModel(CGCompositionModel* parentComposition);

    void save();
    void saveAs();

    void setName(QString name);
    void setPath(QString path);

    QString getName();
    QString getPath();

    CGOperatorModel* addOperatorByName(QString name);

    void activate(QGraphicsItem* item);
    void unactivate(QGraphicsItem* item);

    CGCompositionModel* getParentCompositionModel();
    void setParentCompositionModel(CGCompositionModel* composition);

    CGOperatorModel* getOperatorById(int id);
    void setView(CGMulti* multiView);

    CGMulti* getOuterView();
    CGMulti* getInnerView();

    void setScene(CGScene* scene);
    CGScene* getScene();

    CGView* getSceneView();
    void setSceneView(CGView* view);

    void setId(int id);
    int getId();

    QList<CGOperatorModel*> getOperatorModelList();
    QList<CGCompositionModel*> getCompositionModelList();
    QList<CGShape*> getShapeModelList();
    QList<CGText*> getTextModelList();

    bool belongToThis(CGConnector* conn);
private:
    int id;
    QString name;
    QString path;
    CGCompositionModel* parentComposition; //if null, top compo
    CGMulti* multiView; //null if parent is null
    CGScene* scene;
    CGView* sceneview;
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
private:
    int id;
    QString name;
    QPointF scenePos;
    CGOperator* operatorView;
    QList<CGConnectorModel*> connectorsList;
    QMap<int, CGConnectorModel*> connectorMap;

};

class CGConnectorModel
{
public:
    CGConnectorModel(CGOperatorModel* parent);
    void setDataType(QString dataType);
    QString getDataType();
    void setSide(QString side);
    QString getSide();
    int getId();
    void setId(int id);
    void connect(CGConnectorModel* connectedConnector);
    void disconnect();
    bool getIsConnected();
    CGConnectorModel* getConnectedConnector();
    CGOperatorModel* getCGOperatorModel();
private:
    int id;
    QString dataType;
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
