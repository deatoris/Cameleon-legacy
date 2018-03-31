/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGProject_H
#define CGProject_H
#include <QtGui>
class CGOperatorModel;
class CGCompositionModel;
class CGComposition;
class CGLayout;
class CGProjectTree;

class CGProject : QObject
{
    Q_OBJECT

public slots:
    void setReadme(QString readme);
    void setName(QString name);
    void setVersion(QString version);
    void setPath(QString path);
    void setTmpPath(QString tmpPath);
    void setLogPath(QString logPath);
    void setRessourcePath(QString libPath);
    void setTargetPath(QString targetPath);
    void setConnectorOrderMode(QString connectorOrderMode);
    void setNumberThread(int nb);

    QString getReadme();
    QString getName();
    QString getVersion();
    QString getPath();
    QString getLogPath();
    QString getTmpPath();
    QString getTargetPath();
    QString getRessourcePath();
    QString getConnectorOrderMode();

    int getNumberThread();
public:


    static CGProject* getInstance();

    void createProject(QString path);
private:
    bool showCler;
    QString readme;
    QString name;
    QString version;
    QString path;
    QString tmpPath;
    QString logPath;
    QString ressourcePath;
    QString targetPath;
    QString connectorOrderMode;
    int nbThread;
    CGProject();

    static CGProject* projectInstance;

};

#endif // CGProject_H
