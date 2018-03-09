/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CARGUMENTS_H
#define CARGUMENTS_H
#include <QtCore>
class argument;
class CArguments
{
public:
    static QString getCompositionFromAppArgs(QList<argument> commandArgs);

    static QString getReadmeFromComposition(QString cmPath);

    static QList<argument> getArgumentsFromComposition(QString cmPath);

    static QList<argument> getArgumentsFromApplication();

    static argument getArgument(QString key, QList<argument> fromArguments);

    static bool checkSurface(QList<argument> commandArgs);

    static bool isCameleonHelp(QList<argument> commandArgs);

    static bool isCompositionHelp(QList<argument> commandArgs);

    static bool isCompositionForced(QList<argument> commandArgs);

    static bool isFileExist(QString file);

    static bool isDouble(QString key, QList<argument> commandArgs);

    static QString mode(QList<argument> commandArgs);

    static bool checkArguments(QList<argument>  commandArgs, QList<argument> compositionArgs ); // QString : "-h", "-mode=$MODE", "-cm=$cmPath", "-key"

    static void printCameleonHelp();

    static void printCompositionHelp(QString compositionPath, QList<argument> compositionArgs);
};

class argument
{
public:
    argument(){
        key="";
        value="";
        datatype="";
        description="";
        optional=false;
        present=false;
    }

    QString key;
    QString datatype;
    QString value;
    QString description;
    bool optional;
    bool present;
};

#endif // CARGUMENTS_H
