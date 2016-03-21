/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGGLOSSARY_H
#define CGGLOSSARY_H
#include "CGGlossaryTree.h"
#include "CGlossary.h"
#include "../CGInstance.h"
#include "CGCompositionFactory.h"

class CGGlossary
{
public:
    static CGGlossary* getInstance();
    QDockWidget* getWidgetCopy();

    void addPattern(QString name);

    CGGlossaryTree* getComposerTree();
    CGGlossaryTree* getControllerTree();

private:
    void load(CGGlossaryTree* tree);
    CGGlossary();
    static CGGlossary* glossaryInstance;
    QList<CGGlossaryTree*> treeList;
    QLineEdit* edit;

};
void glossaryTreeGeneration(QTreeWidget * master, QTreeWidgetItem* parent,const Node<string> & node, map<string,string> key2name, int depth, QString type);

#endif // CGGLOSSARY_H
