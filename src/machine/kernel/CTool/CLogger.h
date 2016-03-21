/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CLOGGER_H
#define CLOGGER_H
#include <QtCore>
class CLoggerInstance
{
public:
  static CLoggerInstance* getInstance();
  void log(QString message);
private:
  CLoggerInstance();
  QFile instancelogger;
  static CLoggerInstance* loggerInstance;

};

class CLogger : public QObject
{
    Q_OBJECT

signals:
    void log(QString message);

public:
    enum LVL {ERROR,WARNING,INFO,DEBUG,SHUTDOWN};
    void log(QString name, LVL level, QString message);

    static CLogger* getInstance();

    void setPath(QString path);
    void start();

    void registerComponent(QString name);
    void setComponentLevel(QString name, LVL level);

    QMap<QString, CLogger::LVL> getMap();
    static QString toString(LVL level);
    static CLogger::LVL fromString(QString level);

    bool getActive();
    void setActive(bool a);
    void clear();

private:
    QFile logger;
    CLogger();
    QMap<QString, LVL> mapComponentLevel;
    static CLogger* loggerInstance;
    QString path;
    bool active;
    QMutex _mutex;
};

#endif // CLOGGER_H
