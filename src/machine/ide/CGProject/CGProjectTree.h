/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGPROJECTTREE_H
#define CGPROJECTTREE_H
#include <QtGui>

class CGProjectTree : public QTreeWidget
{
    Q_OBJECT

public slots:
    void deleteLayout();
    void rename();

    void addRessource(QString ressourcePath);

    void createRessource();
    void deleteRessource();
    void renameRessource();
    void createDirRessource();
    void deleteDirRessource();
    void renameDirRessource();

    QTreeWidgetItem* exists(QTreeWidgetItem* parent, QString toAdd);

    bool isRessource(QTreeWidgetItem* item);
    bool isDir(QTreeWidgetItem* item);
    bool isElement(QTreeWidgetItem* item);
public:
    bool cpDir(QString srcPath, QString dstPath);
    CGProjectTree(QWidget * parent = 0);
    QTreeWidgetItem* search(QString data, QTreeWidgetItem* topItem);
    void highlighItem(QString data);
    void setLock(bool locked);
    bool getLock();
    void setItemRessourceTop(QTreeWidgetItem* topitem);
    QTreeWidgetItem* getItemRessourceTop();
protected:
    QString getPathToParent(QTreeWidgetItem*item, QString from);
    virtual void contextMenuEvent ( QContextMenuEvent * event );

    bool locked;

    //project menu
    QMenu* projectMenu;
    QMenu* ressourceDirMenu;
    QMenu* ressourceMenu;
    QAction* renameAction;
    QAction* deleteAction;

    QAction* createRessourceAction;
    QAction* deleteRessourceAction;
    QAction* renameRessourceAction;

    QAction* createDirRessourceAction;
    QAction* deleteDirRessourceAction;
    QAction* renameDirRessourceAction;

    QTreeWidgetItem* itemRessourceTop;
};

#endif // CGPROJECTTREE_H
