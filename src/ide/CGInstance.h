/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGINSTANCE_H
#define CGINSTANCE_H
#include <QtGui>
class CGCompositionModel;
class CGComposer;
#include <QXmlStreamReader>
#include "CLogger.h"
class CGProjectSerialization;

class CGInstance
{
public:
    enum Mode {
        MNG, GUI, CMD
    };
    void save();
    void load();
    void start();
    void init();
    void close();

    void setName(QString name);
    void setPath(QString path);
    void setPluginPath(QString pluginPath);
    void setProjectPath(QString projectPath);
    void setPatternPath(QString patternPath);
    void setLibPath(QString libPath);
    void setRessourcePath(QString ressourcePath);

    QString getName();
    QString getPath();
    QString getPluginPath();
    QString getRessourcePath();
    QString getPatternPath();
    QString getLibPath();
    QString getProjectPath();

    void addInstalledLib(QString name, QString version);
    void removeInstalledLib(QString name, QString version);
    void activeLib(QString name, QString version, bool activated);
    void addPattern(QString name);

    QMap<QString, QString> getInstalledLibsError();
    QMap<QString, QString> getInstalledLibs();
    QMap<QString, QString> getActivatedLibs();
    void loadInstalledLibInfos();
    void loadPatterns();
    void loadRessources();

    void setCurrentProjectPath(QString currentPath);
    void addRecentProject(QString projectPath);
    QString getCurrentProjectPath();
    QList<QString> getRecentProjects();
    QMap<QString, QString> getPatterns();
    QMap<QString, QString> getRessources();

    QString getInstanceVersion();

    static CGInstance* getInstance();
    void setFirstLaunch(bool firstLaunch);
    bool getFirstLaunch();
    CGProjectSerialization* getSerial();
    Qt::DockWidgetArea stringToArea(QString s);
    QString areaToString(Qt::DockWidgetArea area);

    void setMode(CGInstance::Mode mode);
    CGInstance::Mode getMode();

    QString makeRelative(QString it);
    QString makeAbsolute(QString it);

    void clearActivated();
private:
    QStringList getSharedLibrariesFromDir(QString dir);
    QStringList getPatternsFromDir(QString dir);
    QStringList getRessourcesFromDir(QString dir);

    Qt::DockWidgetArea controllerproject;
    Qt::DockWidgetArea controllerglossary;
    Qt::DockWidgetArea composerglossary;
    Qt::DockWidgetArea composerproject;
    Qt::DockWidgetArea machine;

    bool isProjectControllerOpen;
    bool isGlossaryControllerOpen;
    bool isProjectComposerOpen;
    bool isGlossaryComposerOpen;
    bool isMachinOpen;
    bool firstLaunch;

    CGInstance();
    QString instanceVersion;
    QString name;
    QString path;
    QString pluginPath;
    QString libPath;
    QString projectPath;
    QString patternPath;
    QString ressourcePath;

    QString currentProjectPath;
    QList<QString> recentProjects;

    QMap<QString, QString> installedLibs; //name, version
    QMap<QString, QString> installedLibsErrors; //name, version
    QMap<QString, QString> activatedLibs; //name, version
    QMap<QString, QString> patterns; //name, path
    QMap<QString, QString> ressources; //name, path

    QXmlStreamWriter xmlWriter;
    QXmlStreamReader xmlReader;

    static CGInstance* workspaceInstance;
    CGProjectSerialization* serial;
    CGInstance::Mode mode;
};

#endif // CGINSTANCE_H
