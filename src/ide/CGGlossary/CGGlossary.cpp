/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGGlossary.h"

CGGlossary* CGGlossary::glossaryInstance = NULL;

void glossaryTreeGeneration(QTreeWidget * master, QTreeWidgetItem* parent,const Node<string> & node, map<string,string> key2name, int depth, QString type)
{
    //    itemRessourceTop->setText(0, text);
    //    itemRessourceTop->setText(1, "dir");
    //    itemRessourceTop->setText(1, "path");
    if(depth!=0)
    {
        QTreeWidgetItem* item1;
        if(master==NULL)
            item1 = new QTreeWidgetItem(parent);
        else
            item1 = new QTreeWidgetItem(master);
        QString str((node.data())->c_str());
        item1->setData(0,Qt::UserRole,type);

        if((int)node.getNbrChild() == 0){
            item1->setText(0, key2name[str.toStdString()].c_str());
            item1->setText(1, "file");
            item1->setText(2, str);
            item1->setIcon(0,QIcon(":/icons/clogo.png"));
        }else{
            item1->setText(0, str);
            item1->setText(1, "dir");
            item1->setText(2, str);
            item1->setIcon(0,QIcon(":/icons/ui_tab_content.png"));
        }

        for(int i =0;i<(int)node.getNbrChild();i++)
            glossaryTreeGeneration(NULL,item1,*(node[i]),key2name,depth+1,type);

    }
    else
    {
        for(int i =0;i<(int)node.getNbrChild();i++)
            glossaryTreeGeneration(master,parent,*(node[i]),key2name,depth+1,type);
    }

}

CGGlossary::CGGlossary()
{
    //    CLogger::getInstance()->registerComponent("GLOSSARYEXPLORER");

}

CGGlossary* CGGlossary::getInstance(){
    if(NULL != glossaryInstance){
        return glossaryInstance;
    }else{
        glossaryInstance = new CGGlossary();
        return glossaryInstance;
    }
}

QDockWidget* CGGlossary::getWidgetCopy(){

    CGGlossaryTree* tree = new CGGlossaryTree();
    this->load(tree);
    QVBoxLayout* layout = new QVBoxLayout();
    treeList.append(tree);
    layout->addWidget(tree);

    //search features
    edit = new QLineEdit();
    edit->setText("search");
    edit->setMinimumHeight(30);
    edit->setMaximumHeight(30);
    tree->setEdit(edit);
    layout->addWidget(edit);
    layout->setMargin(0);
    layout->setSpacing(0);
    QObject::connect(edit, SIGNAL(textChanged(QString)),
                     tree, SLOT(searchOperator()));

    QObject::connect(edit, SIGNAL(cursorPositionChanged(int,int)),
                     tree, SLOT(enterSearch()));


    QWidget* dockWidget = new QWidget();
    dockWidget->setLayout(layout);
    QString name = "";
    if(treeList.size() == 1){
        name = "Glossary (composer)";
    }else{
        name = "Glossary (controller)";
    }

    QDockWidget* dockFactory = new QDockWidget(QObject::tr(name.toStdString().c_str()));
    dockFactory->setWidget(dockWidget);
    dockWidget->setParent(dockFactory);

    dockFactory->adjustSize();

    return dockFactory;
}

void CGGlossary::load(CGGlossaryTree* tree){
    if(treeList.size() == 0){
        //        tree->getItemControlTop()->setDisabled(true);
        //        tree->getItemRessourceTop()->setDisabled(true);
        //load operators
        vector<pair<vector<string>,string> > v = CGlossarySingletonClient::getInstance()->getOperatorsByName();



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

        glossaryTreeGeneration(NULL,tree->getItemOperatorTop(),node,key2name,0,"operator");

        //load controls
        vector<pair<vector<string>,string> > v2 = CGlossarySingletonClient::getInstance()->getControlsByName();
//        vector<pair<vector<string>,string> >::iterator it;
//        for(it =v2.begin()  ; it!= v2.end() ;it++) {
//            if(it->second=="MarkerImageGrid"){
//                v2.erase(it);
//            }
//            if( it->second=="MarkerImageGrid3d"){
//                v2.erase(it);
//            }
//        }

        map<string,string> key2name2;
        for(int i =0;i<(int)v2.size();i++)
        {
            key2name2[*(v2[i].first.rbegin())]=v2[i].second;
        }
        string * c2 =  new string("head");
        Node<string> node2(c2);
        for(int i =0;i<(int)v2.size();i++)
        {
            addBranchMerge(v2[i].first,node2);
        }

        glossaryTreeGeneration(NULL,tree->getItemControlTop(),node2,key2name2,0,"control");
        //load ressources
        QMap<QString,QString> ressourceMap = CGInstance::getInstance()->getRessources();
        QMap<QString, QString>::iterator k = ressourceMap.begin();
        while (k != ressourceMap.end()) {
            CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::INFO,"Load ressource "+k.value()+" "+k.key()+" ...");
            tree->addRessource(k.value());
            k++;
        }
        tree->getItemRessourceTop()->sortChildren(1,Qt::AscendingOrder);
        //load patterns
        QMap<QString,QString> patternMap = CGInstance::getInstance()->getPatterns();
        QMap<QString, QString>::iterator j = patternMap.begin();
        while (j != patternMap.end()) {
            CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::INFO,"Load pattern "+j.value()+" "+j.key()+" ...");
            tree->addPattern(j.value());
            j++;
        }
        tree->getItemPatternTop()->sortChildren(1,Qt::AscendingOrder);

    }else{
        tree->getItemOperatorTop()->setDisabled(true);
        //        tree->getItemPatternTop()->setDisabled(true);
        //load controls
        vector<pair<vector<string>,string> > v2 = CGlossarySingletonClient::getInstance()->getControlsByName();
        map<string,string> key2name2;
        for(int i =0;i<(int)v2.size();i++)
        {
            key2name2[*(v2[i].first.rbegin())]=v2[i].second;
        }
        string * c2 =  new string("head");
        Node<string> node2(c2);
        for(int i =0;i<(int)v2.size();i++)
        {
            addBranchMerge(v2[i].first,node2);
        }

        glossaryTreeGeneration(NULL,tree->getItemControlTop(),node2,key2name2,0,"control");

        //load ressources
        QMap<QString,QString> ressourceMap = CGInstance::getInstance()->getRessources();
        QMap<QString, QString>::iterator k = ressourceMap.begin();
        while (k != ressourceMap.end()) {
            CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::INFO,"Load ressource "+k.value()+" "+k.key()+" ...");
            tree->addRessource(k.value());
            k++;
        }
        tree->getItemRessourceTop()->sortChildren(1,Qt::AscendingOrder);

        QMap<QString,QString> patternMap = CGInstance::getInstance()->getPatterns();
        QMap<QString, QString>::iterator j = patternMap.begin();
        while (j != patternMap.end()) {
            CLogger::getInstance()->log("GLOSSARYEXPLORER",CLogger::INFO,"Load pattern "+j.value()+" "+j.key()+" ...");
            tree->addPattern(j.value());
            j++;
        }
        tree->getItemPatternTop()->sortChildren(1,Qt::AscendingOrder);
    }

}

void CGGlossary::addPattern(QString name){
    QTreeWidgetItem* item = new QTreeWidgetItem(treeList[0]->getItemPatternTop());
    item->setText(0, name);
    item->setText(1, "file");
    item->setText(2, CGInstance::getInstance()->getPatternPath()+"/"+name);
    item->setData(0,Qt::UserRole,"pattern");
    item->setIcon(0,QIcon(":/icons/clogo.png"));

    item = new QTreeWidgetItem(treeList[1]->getItemPatternTop());
    item->setText(0, name);
    item->setText(1, "file");
    item->setText(2, CGInstance::getInstance()->getPatternPath()+"/"+name);
    item->setData(0,Qt::UserRole,"pattern");
    item->setIcon(0,QIcon(":/icons/clogo.png"));
}


CGGlossaryTree* CGGlossary::getComposerTree(){
    return treeList[0];
}

CGGlossaryTree* CGGlossary::getControllerTree(){
    return treeList[1];
}
