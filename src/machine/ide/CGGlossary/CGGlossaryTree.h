/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGGlossaryTree_H
#define CGGlossaryTree_H
#include <QtGui>

class CGGlossaryTree : public QTreeWidget
{
    Q_OBJECT

public slots:
    void deletePattern();
    void renamePattern();
    void createDirPattern();
    void deleteDirPattern();
    void renameDirPattern();

    void createRessource();
    void deleteRessource();
    void renameRessource();
    void createDirRessource();
    void deleteDirRessource();
    void renameDirRessource();

    void searchOperator();
    void enterSearch();

public:
    CGGlossaryTree(QWidget * parent = 0);
    bool isChild(QTreeWidgetItem* parent, QTreeWidgetItem* child);

    QTreeWidgetItem* exists(QTreeWidgetItem* parent, QString toAdd);

    bool isControl(QTreeWidgetItem* item);
    bool isPattern(QTreeWidgetItem* item);
    bool isOperator(QTreeWidgetItem* item);
    bool isRessource(QTreeWidgetItem* item);
    bool isDir(QTreeWidgetItem* item);
    bool isElement(QTreeWidgetItem* item);
    QString getPathToParent(QTreeWidgetItem*item, QString from);

    bool reviewPath(QTreeWidgetItem* item);

    QTreeWidgetItem* getItemOperatorTop();
    QTreeWidgetItem* getItemControlTop();
    QTreeWidgetItem* getItemPatternTop();
    QTreeWidgetItem* getItemRessourceTop();

    void addPattern(QString patternPath);
    void addRessource(QString ressourcePath);

    void newPattern(QString name);
    void setEdit(QLineEdit* edit);

protected:
    bool move(QString srcPath, QString dstPath);
    bool rmDir( QString dirPath);
    void expandToItem(QTreeWidget* tree, QTreeWidgetItem* item);

    void createActions();
    void createMenu();
    void createDummyTree();

    virtual void contextMenuEvent ( QContextMenuEvent * event );
    virtual void dropEvent ( QDropEvent * event );
    //project menu
    QMenu* patternMenu;
    QMenu* patternDirMenu;

    QMenu* ressourceMenu;
    QMenu* ressourceDirMenu;

    QAction* renamePatternAction;
    QAction* deletePatternAction;
    QAction* createRessourceAction;
    QAction* deleteRessourceAction;
    QAction* renameRessourceAction;
    QAction* createDirPatternAction;
    QAction* deleteDirPatternAction;
    QAction* renameDirPatternAction;
    QAction* createDirRessourceAction;
    QAction* deleteDirRessourceAction;
    QAction* renameDirRessourceAction;

    QTreeWidgetItem* itemOperatorTop;
    QTreeWidgetItem* itemControlTop;
    QTreeWidgetItem* itemPatternTop;
    QTreeWidgetItem* itemRessourceTop;

    bool hasBeenEdited;
    QLineEdit* edit;
};
#endif // CGGlossaryTree_H
