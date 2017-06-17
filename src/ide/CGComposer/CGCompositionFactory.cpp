/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCompositionFactory.h"
#include "CGCompositionManager.h"
#include "CGlossary.h"
#include "../CGInstance.h"
#include "CClient.h"
#include "CGProject.h"

CGCompositionFactory* CGCompositionFactory::factoryInstance = NULL;

void treeGeneration(QTreeWidget * master, QTreeWidgetItem* parent,const Node<string> & node, map<string,string> key2name, int depth)
{
    if(depth!=0)
    {
        QTreeWidgetItem* item1;
        if(master==NULL)
            item1 = new QTreeWidgetItem(parent);
        else
            item1 = new QTreeWidgetItem(master);
        QString str((node.data())->c_str());
        item1->setData(0,Qt::UserRole,str);

        if((int)node.getNbrChild() == 0){
            item1->setText(0, key2name[str.toStdString()].c_str());
            item1->setIcon(0,QIcon(":/icons/clogo.png"));
        }else{
            item1->setText(0, str);
            item1->setIcon(0,QIcon(":/icons/ui_tab_content.png"));
        }

        for(int i =0;i<(int)node.getNbrChild();i++)
            treeGeneration(NULL,item1,*(node[i]),key2name,depth+1);

    }
    else
    {
        for(int i =0;i<(int)node.getNbrChild();i++)
            treeGeneration(master,parent,*(node[i]),key2name,depth+1);
    }

}

void CGCompositionFactory::addPattern(QString name){
    QTreeWidgetItem* item1 = new QTreeWidgetItem(patterns);
    item1->setText(0, name);
    item1->setData(0,Qt::UserRole,"pattern:"+name);
}


void CGCompositionFactory::refresh(){

}

CGCompositionModel* CGCompositionFactory::getCompositionByPatternName(QString name, CGCompositionModel* parentModel){
    //read composition & layout from XML file
    return CGProject::getInstance()->compositionFromFile(name, parentModel);
}

void CGCompositionFactory::loadPatterns(){
    patterns= new QTreeWidgetItem(tree);
    patterns->setText(0, "patterns");
    patterns->setData(0,Qt::UserRole,"patterns");
    patterns->setIcon(0,QIcon(":/icons/ui_tab_content.png"));

    QMap<QString, QString> mapNamePath = CGInstance::getInstance()->getPatterns();
    QMapIterator<QString, QString> i(mapNamePath);
    while (i.hasNext()) {
        i.next();
        QTreeWidgetItem* item1 = new QTreeWidgetItem(patterns);
        item1->setText(0, i.key());
        item1->setData(0,Qt::UserRole,"pattern:"+i.key());
        item1->setIcon(0,QIcon(":/icons/clogo.png"));
    }
}

CGCompositionFactory::CGCompositionFactory()
{
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

    tree = new QTreeWidget();
    treeGeneration(tree,NULL,node,key2name,0);

    tree->setHeaderHidden(true);
    tree->setDragEnabled(true);
    tree->setDropIndicatorShown(true);

    this->loadPatterns();

}

void CGCompositionFactory::expandToItem(QTreeWidget* tree, QTreeWidgetItem* item){
    if(item->parent() != 0){
        item->parent()->setExpanded(true);
        this->expandToItem(tree, item->parent());
    }
}

void CGCompositionFactory::search(QString text){
    QTreeWidgetItem *item;
    QList<QTreeWidgetItem *> found = tree->findItems(
                text, Qt::MatchContains | Qt::MatchRecursive);
    QList<QTreeWidgetItem *> found2 = tree->findItems(
                "", Qt::MatchContains | Qt::MatchRecursive);

    foreach(item, found2){
        item->setExpanded(false);
        item->setSelected(false);
    }

    if(text.compare("") != 0){
        foreach(item, found){
            if(item->childCount() == 0){
                this->expandToItem(tree, item);
                item->setSelected(true);
            }
        }
    }
}


CGCompositionFactory* CGCompositionFactory::getInstance(){
    if(NULL != factoryInstance){
        return factoryInstance;
    }else{
        factoryInstance = new CGCompositionFactory();
        return factoryInstance;
    }
}

QTreeWidget* CGCompositionFactory::getTreeWidget(){
    return tree;
}

CGOperatorModel* CGCompositionFactory::getCGOperatorByStruct(COperatorStructure opstruc, int opid){
        CGOperatorModel* model = new CGOperatorModel(); //TODO machine factory call
        model->setId(opid);
        model->setName(opstruc.getName().c_str());
        model->setKey(opstruc.getKey().c_str());
        model->setInformation(opstruc.getInformation().c_str());
        model->setNumPlugIn((int)opstruc.plugIn().size());
        model->setActionList(opstruc.getActionKeys());
        for(int i =0;i<(int)opstruc.plugIn().size();i++){
            CGConnectorModel* connector = new CGConnectorModel(model);
            connector->setSide("IN");
            connector->setDataType(opstruc.plugIn()[i].first);
            connector->setName(QString::fromStdString(opstruc.plugIn()[i].second));
            connector->setId(i);
            model->addConnector(connector);
        }
        for(int i =0;i<(int)opstruc.plugOut().size();i++){
            CGConnectorModel* connector = new CGConnectorModel(model);
            connector->setSide("OUT");
            connector->setDataType(opstruc.plugOut()[i].first);
            connector->setName(QString::fromStdString(opstruc.plugOut()[i].second));
            connector->setId(opstruc.plugIn().size()+i);
            model->addConnector(connector);
        }
        return model;
}


CGOperatorModel* CGCompositionFactory::getCGOperatorByName(QString name){
    int opid;
    COperatorStructure opstruc;
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->createOperatorSend(name.toStdString(),opid,opstruc);
    if(opid != -1){
        CGOperatorModel* model = new CGOperatorModel(); //TODO machine factory call
        model->setId(opid);
        model->setName(opstruc.getName().c_str());
        model->setKey(opstruc.getKey().c_str());
        model->setInformation(opstruc.getInformation().c_str());
        model->setNumPlugIn((int)opstruc.plugIn().size());
        model->setActionList(opstruc.getActionKeys());
        for(int i =0;i<(int)opstruc.plugIn().size();i++){
            CGConnectorModel* connector = new CGConnectorModel(model);
            connector->setSide("IN");
            connector->setDataType(opstruc.plugIn()[i].first);
            connector->setName(QString::fromStdString(opstruc.plugIn()[i].second));
            connector->setId(i);
            model->addConnector(connector);
        }
        for(int i =0;i<(int)opstruc.plugOut().size();i++){
            CGConnectorModel* connector = new CGConnectorModel(model);
            connector->setSide("OUT");
            connector->setDataType(opstruc.plugOut()[i].first);
            connector->setName(QString::fromStdString(opstruc.plugOut()[i].second));
            connector->setId(opstruc.plugIn().size()+i);
            model->addConnector(connector);
        }
//        CClientSingleton::getInstance()->getCInterfaceClient2Server()->EndCreateOperatorSend(opid);
        return model;
    }else{
        return NULL;
    }
}

CGCompositionModel* CGCompositionFactory::getModelFromXML(){
    return NULL;
}
