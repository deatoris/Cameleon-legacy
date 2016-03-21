/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGDoc.h"
#include<CSingleton.h>
#include<CInterfaceClient.h>
#include <CGlossary.h>
#include <CProcessor.h>
#include <DataNumber.h>
#include <CClient.h>
#include <CMachine.h>
#include "CGInstance.h"
#include <memory>
#include <tr1/memory>
#include <tr1/shared_ptr.h>
#include <DataMap.h>
#include <DataString.h>

#include <CUtilitySTL.h>
#include "CGLicence.h"
#include "CGProjectSerialization.h"

CGDoc::CGDoc()
{
    parentNode = new DocNode();
    parentControlNode = new DocNode();
    parentNode->setKey("topParent");
}

#include "CGGlossary.h"
void CGDoc::generateDoc(){

    analyse();
    //    QString indexHtml = index(parentNode);
    //    QString docHtml = generateDoc(parentNode);
    //    indexHtml = indexHtml+docHtml;
    printIndex(parentNode);
    printGlossary(parentNode);
    printWPGlossary(parentNode);

}

void CGDoc::printIndex(DocNode* node){
    QString s = "../../../doc/glossary/index.html";
    QList<DocNode*> children = node->getChildren();
    QString indexHtml = "<!DOCTYPE html><html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"dic.css\"></head><body>";
    if(!children.isEmpty()){
        indexHtml = indexHtml+"<OL>\n";
        foreach(DocNode*nc, children){
            indexHtml = indexHtml+"<LI><a href=\""+nc->getKey()+".html\">"+nc->getKey()+"</a>\n";
        }
        indexHtml = indexHtml+"</OL>\n";
    }
    indexHtml = indexHtml + "</body></html>";
    ofstream  out(s.toStdString().c_str());
    if(out.is_open()){
        out<<indexHtml.toStdString().c_str();
    }
}

DocNode* CGDoc::getControlNodeByKey(QString key){
    QList<DocNode*> children = parentControlNode->getChildren();
    foreach(DocNode*nc, children){
        if(nc->getKey().compare(key)==0)return nc;
    }
    return NULL;
}

void CGDoc::printWPGlossary(DocNode* node){
    QList<DocNode*> children = node->getChildren();
    if(!children.isEmpty()){
        foreach(DocNode*nc, children){
            QString indexHtml = "";
            indexHtml = indexHtml+"<b>Operators</b>"+index(nc);
            if(getControlNodeByKey(nc->getKey())!=NULL) indexHtml = indexHtml+"<b>Controls</b>"+indexControl(getControlNodeByKey(nc->getKey()));
            indexHtml = indexHtml+generateDoc(nc);
            if(getControlNodeByKey(nc->getKey())!=NULL) indexHtml = indexHtml+generateControlDoc(getControlNodeByKey(nc->getKey()));
            QString s = "../../../doc/glossary/wp_"+nc->getKey()+".html";
            indexHtml.replace("<LI>","");
            indexHtml.replace("<OL>","");
            indexHtml.replace("</OL>","");
            indexHtml.replace("\n\n\n","\n");
            indexHtml.replace("img/","http://www.shinoe.org/glossary/img/");
            ofstream  out(s.toStdString().c_str());
            if(out.is_open()){
                out<<indexHtml.toStdString().c_str();
            }
        }
    }
}

void CGDoc::printGlossary(DocNode* node){
    QList<DocNode*> children = node->getChildren();
    if(!children.isEmpty()){
        foreach(DocNode*nc, children){
            QString indexHtml = "<!DOCTYPE html><html><head><link rel=\"stylesheet\" type=\"text/css\" href=\"dic.css\"></head><body>";
            indexHtml = indexHtml+"<h2>"+nc->getKey()+"</h2>";
            indexHtml = indexHtml+"<b>Operators</b>"+index(nc);
            if(getControlNodeByKey(nc->getKey())!=NULL) indexHtml = indexHtml+"<b>Controls</b>"+indexControl(getControlNodeByKey(nc->getKey()));
            indexHtml = indexHtml+generateDoc(nc);
            if(getControlNodeByKey(nc->getKey())!=NULL) indexHtml = indexHtml+generateControlDoc(getControlNodeByKey(nc->getKey()));
            QString s = "../../../doc/glossary/"+nc->getKey()+".html";
            indexHtml = indexHtml + "</body></html>";
            ofstream  out(s.toStdString().c_str());
            if(out.is_open()){
                out<<indexHtml.toStdString().c_str();
            }
        }
    }
}

//    <OL>
//    <LI>élément 1 :
//        <OL>
//        <LI>sous-élément A,
//        <LI>sous-élément B.
//        </OL>
//    <LI>élément 2,
//    <LI>élément 3.
//    </OL>
QString CGDoc::index(DocNode* node){
    QString indexHtml = "";

    QList<DocNode*> children = node->getChildren();
    if(!children.isEmpty()){
        indexHtml = indexHtml+"<OL>\n";
        foreach(DocNode*nc, children){
            if(!nc->getChildren().isEmpty()){
                indexHtml = indexHtml+"<a href=\"#operator"+nc->getKey()+"\">"+nc->getKey()+"</a>\n"+index(nc);
            }
        }
        indexHtml = indexHtml+"</OL>\n";
    }
    return indexHtml;
}

QString CGDoc::indexControl(DocNode* node){
    QString indexHtml = "";

    QList<DocNode*> children = node->getChildren();
    if(!children.isEmpty()){
        indexHtml = indexHtml+"<OL>\n";
        foreach(DocNode*nc, children){
            if(!nc->getChildren().isEmpty()){
                indexHtml = indexHtml+"<a href=\"#control"+nc->getKey()+"\">"+nc->getKey()+"</a>\n"+index(nc);
            }
        }
        indexHtml = indexHtml+"</OL>\n";
    }
    return indexHtml;
}

QString CGDoc::generateDoc(DocNode* node){
    QString indexHtml = "";

    QList<DocNode*> children = node->getChildren();
    if(!children.isEmpty()){
        foreach(DocNode*nc, children){
            if(!nc->getChildren().isEmpty()){
                indexHtml  = indexHtml  + "<a name=\"operator"+nc->getKey()+"\"></a>\n";
                indexHtml = indexHtml+"<b>"+nc->getKey()+" (<a href=\"#\" >Back to top</a>)</b><br>\n"+generateDoc(nc);
            }else{
                indexHtml = indexHtml+operatorDoc(nc->getKey())+"\n"+generateDoc(nc);
            }
        }
    }
    return indexHtml;
}

QString CGDoc::generateControlDoc(DocNode* node){
    QString indexHtml = "";

    QList<DocNode*> children = node->getChildren();
    if(!children.isEmpty()){
        foreach(DocNode*nc, children){
            if(!nc->getChildren().isEmpty()){
                indexHtml  = indexHtml  + "<a name=\"control"+nc->getKey()+"\"></a>\n";
                indexHtml = indexHtml+"<b>"+nc->getKey()+" (<a href=\"#\" >Back to top</a>)</b><br>\n"+generateControlDoc(nc);
            }else{
                indexHtml = indexHtml+controlDoc(nc->getKey())+"\n"+generateControlDoc(nc);
            }
        }
    }
    return indexHtml;
}

QString CGDoc::operatorDoc(QString key){
    CGOperatorModel* model = CGCompositionFactory::getInstance()->getCGOperatorByName(key);

//    CGOperator* ope = new CGOperator(0,0,0,0);
//    ope->setModel(model);
//    ope->renderModel();
//    model->setView(ope);
//    QImage image(model->getView()->boundingRect().size().toSize(), QImage::Format_RGB32);
//    image.fill(QColor(0, 0, 0).rgb());
//    QPainter painter(&image);
//    QStyleOptionGraphicsItem styleOption;
//    model->getView()->paint(&painter, &styleOption);
//    painter.end();
//    image.save("../../../doc/glossary/img/"+model->getName()+".png");

    QString s = "";
    QString inputs="";
    QString outputs="";

    foreach(CGConnectorModel* cmodel, model->getConnectorsList()){
        if(cmodel->getSide().compare("OUT") == 0) outputs=outputs+" "+cmodel->getName()+" ("+cmodel->getDataType()+")";
        if(cmodel->getSide().compare("IN") == 0) inputs=inputs+" "+cmodel->getName()+" ("+cmodel->getDataType()+")";
    }

    QString imageName=QString::number(model->getNumPlugIn())+QString::number(model->getNumPlugOut())+".jpg";

    //content
    s = s + "<a name=\""+model->getName()+"\"></a>\n";
    s = s + "<div class=\"operator\" >\n";
    s = s + "<div class=\"operator-image\" ><img src=\"img/"+imageName+"\" height=\"150\" width=\"150\" ></div>\n";
    s = s + "<div class=\"operator-content\">\n";
    s = s + "<b>"+ model->getName()+"</b>\n";
    s = s + "<p><div class=\"operator-inputs\">Inputs: "+ inputs+"</div>\n";
    s = s + "<div class=\"operator-outputs\">Outputs: "+outputs+"</div>\n";
    s = s + "<p><b><br>Description</b></p>\n";
    QString inf = model->getInformation().replace("<","\"<\"");
    if(inf.compare("")==0)inf="<strong>[WARN]</strong> No <br>Description available.";
    inf = inf.replace(">","\">\"");
    s = s + inf +"\n";
    s = s + "</div></div>\n";
    return s;
}

QString CGDoc::controlDoc(QString key){
    CGLayoutItem* item = CGLayoutFactory::getInstance()->getLayoutItemByName(key,CGCompositionManager::getInstance()->getCurrentComposerScene());
    QPixmap pix = QPixmap::grabWidget(item->getControl());
    pix.save("../../../doc/glossary/img/"+key+".png");

    QString inputs="";
    QString outputs="";

    foreach(CGConnector* cmodel, item->getConnectorsL()){
        if(cmodel->getModel()->getSide().compare("OUT") == 0) outputs=outputs+" "+cmodel->getModel()->getName()+" ("+cmodel->getModel()->getDataType()+")";
        if(cmodel->getModel()->getSide().compare("IN") == 0) inputs=inputs+" "+cmodel->getModel()->getName()+" ("+cmodel->getModel()->getDataType()+")";
    }

    QString img = "<div class=\"operator\" >\n";
    img = img + "<div class=\"operator-content\">";
    img = img+"<div class=\"control-image\" ><img src=\"img/"+key+".png\"></div>";
    img = img + "<b>"+ QString(item->getControl()->getName().c_str())+"</b>\n";
    img = img + "<p><div class=\"operator-inputs\">Inputs: "+ inputs+"</div>\n";
    img = img + "<div class=\"operator-outputs\">Outputs: "+outputs+"</div>\n";
    img = img + "<p><b><br>Description</b></p>\n";
    QString inf = QString(item->getControl()->getInformation().c_str()).replace("<","\"<\"");
    if(inf.compare("")==0)inf="<strong>[WARN]</strong> No <br>Description available.";
    inf = inf.replace(">","\">\"");
    img = img + inf +"\n";
    img = img + "</div></div>";
    return img;
}


void CGDoc::analyse(){
    CLoggerInstance::getInstance()->log("Analyse doc data sources...");
    CGGlossaryTree* t = CGGlossary::getInstance()->getComposerTree();

    QTreeWidgetItemIterator it(t);
    while (*it) {
        if(t->isControl(*it) && t->isElement(*it)){
            CLoggerInstance::getInstance()->log("Process index for Control"+ t->getPathToParent(*it,""));

            //process path without dico name & element name
            QString path = t->getPathToParent(*it,"");
            QStringList listP = path.split("/",QString::SkipEmptyParts);
            path = "";
            DocNode* c = parentControlNode;
            foreach(QString l, listP){
                if(listP.last()!=l)c = c->addChild(l);
            }
            c->addChild((*it)->text(2));
        }
        if(t->isOperator(*it) && t->isElement(*it)){
            CLoggerInstance::getInstance()->log("Process index for Operator "+ t->getPathToParent(*it,""));

            //process path without dico name & element name
            QString path = t->getPathToParent(*it,"");
            QStringList listP = path.split("/",QString::SkipEmptyParts);
            path = "";
            DocNode* c = parentNode;
            foreach(QString l, listP){
                if(listP.last()!=l)c = c->addChild(l);
            }
            c->addChild((*it)->text(2));
        }
        ++it;
    }
    QList<DocNode*> children = parentControlNode->getChildren();
    if(!children.isEmpty()){
        foreach(DocNode*nc, children){
            parentNode->addChild(nc->getKey());
        }
    }
}

DocNode::DocNode(){

}

void DocNode::setParent(DocNode*parent){
    this->parent = parent;
}

DocNode* DocNode::getParent(){
    return parent;
}

DocNode* DocNode::addChild(QString key){
    foreach(DocNode*c,children){
        if(c->getKey().compare(key)==0)return c;
    }
    DocNode* child = new DocNode();
    child->setKey(key);
    child->setParent(this);
    children.push_back(child);
    return child;
}

QList<DocNode*> DocNode::getChildren(){
    return children;
}

QString DocNode::getKey(){
    return key;
}

void DocNode::setKey(QString name){
    this->key = name;
}
