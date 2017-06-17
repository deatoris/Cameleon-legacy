/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGDOC_H
#define CGDOC_H
#include <QtCore>
class DocNode;
class CGDoc
{
public:
    CGDoc();
    void generateDoc();
private:
    void analyse();
    QString operatorDoc(QString key);
    QString controlDoc(QString key);
    QString index(DocNode* node);
    QString generateDoc(DocNode* node);
    QString generateControlDoc(DocNode* node);
    void printIndex(DocNode* node);
    void printGlossary(DocNode* node);
    void printWPGlossary(DocNode* node);
    DocNode*parentNode;
    DocNode*parentControlNode;
    DocNode* getControlNodeByKey(QString key);
    QString indexControl(DocNode* node);
};

class DocNode{
public:
    DocNode();
    void setParent(DocNode*parent);
    DocNode* getParent();
    DocNode* addChild(QString key);
    QList<DocNode*> getChildren();

    QString getKey();
    void setKey(QString key);
private:
    QList<DocNode*> children;
    DocNode* parent;
    QString key;
};

#endif // CGDOC_H
