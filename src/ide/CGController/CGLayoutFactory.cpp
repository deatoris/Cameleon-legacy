/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGLayoutFactory.h"
#include "CGInNumber.h"
#include "CGLayoutManager.h"
#include<CGlossary.h>
#include <CTree.h>
#include "CGCompositionFactory.h"
CGLayoutFactory* CGLayoutFactory::layoutFactoryInstance = NULL;


CGLayoutFactory::CGLayoutFactory()
{
    vector<pair<vector<string>,string> > v = CGlossarySingletonClient::getInstance()->getControlsByName();
    map<string,string> key2name;
    for(int i =0;i<(int)v.size();i++)
    {
        key2name[*(v[i].first.rbegin())]=v[i].second;
    }
    string * c =  new string("head");
    Node<string> node(c);
    for(int i =0;i<(int)v.size();i++)
    {
        addBranchMerge(v[i].first,node);
    }

    tree = new QTreeWidget();
    treeGeneration(tree,NULL,node,key2name,0);


    tree->setHeaderHidden(true);
    tree->setDragEnabled(true);
    tree->setDropIndicatorShown(true);
}

CGLayoutFactory* CGLayoutFactory::getInstance(){
    if(NULL != layoutFactoryInstance){
        return layoutFactoryInstance;
    }else{
        layoutFactoryInstance = new CGLayoutFactory();
        return layoutFactoryInstance;
    }
}

QTreeWidget* CGLayoutFactory::getTreeWidget(){
    return tree;
}

CGLayoutItem* CGLayoutFactory::getLayoutItemByName(QString name, QGraphicsScene* scene){
    CGLayoutItem* layout = new CGLayoutItem(0,0,scene);
    //CGInNumber * control = new CGInNumber();
    try{
        CControl * control =  CGlossarySingletonClient::getInstance()->createControl(name.toStdString());

        layout->setControl(control);
        layout->render();
        return layout;
    }catch(exception e)
    {
        scene->removeItem(layout);
        return NULL;
    }
}

