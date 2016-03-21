/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGCommandManager_H
#define CGCommandManager_H
#include <QtGui>
class CGCommand;

class CGCommandManager
{
public:
    void startAction(QString name);
    void endAction(QString name);
    void add(CGCommand* command);

    void undo(); //CTRL+Z
    void redo(); //CTRL+Y

    static CGCommandManager* getInstance();

    void clearUndo();
    void clearDoUndo();
    void setStep(int s);
    int getStep();
private:
    int step;
    int count;
    CGCommandManager();
    QList<CGCommand*> commandList;
    QList<CGCommand*> undocommandList;
    static CGCommandManager* commandManagerInstance;
};

class CGCommand
{
public:
    CGCommand();

    void doCommand();
    void undoCommand();

    virtual void exec(){}
    virtual void unexec(){}

    QString getType();
    bool getIsReversable();

protected:
    void setIsReversable(bool reversable);
    QString type;
    bool isReversable;

};

#endif // CGCommandManager_H
