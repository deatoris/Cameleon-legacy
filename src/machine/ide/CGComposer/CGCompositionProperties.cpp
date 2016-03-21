/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCompositionProperties.h"
#include "CGCompositionManager.h"
#include "CGCompositionItem.h"
#include "CLogger.h"
CGCompositionProperties::CGCompositionProperties()
{
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* buttonLayout= new QHBoxLayout();
    QPushButton* applybutton = new QPushButton("apply");
    QPushButton* cancelbutton = new QPushButton("cancel");

    buttonLayout->addWidget(applybutton);
    buttonLayout->addWidget(cancelbutton);
    connect(applybutton, SIGNAL(clicked()),
            this, SLOT(save()));
    connect(cancelbutton, SIGNAL(clicked()),
            this, SLOT(close()));
    this->setLayout(layout);
    this->create();
    nameEdit = new QLineEdit();
    edit = new QTextEdit();
    layout->addWidget(nameEdit);
    layout->addWidget(edit);
    layout->addWidget(table);
    layout->addLayout(buttonLayout);
    this->resize(750,500);
    this->setWindowIcon(QIcon(":/icons/clogo.png"));
}

void CGCompositionProperties::save(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGCompositionProperties::save");
    int rcount = model->rowCount();
    compositionModel->setInformation(edit->toPlainText());
    compositionModel->getOuterView()->setToolTip(compositionModel->getInformation());
    compositionModel->setName(nameEdit->text());
    for(int i=0;i<rcount;i++){
        QStandardItem* iditem = model->item(i,0);
        QString sid = iditem->text();
        int id = sid.toInt();

        QStandardItem* nameitem = model->item(i,1);

        CGMultiConnector *cm =
                qgraphicsitem_cast<CGMultiConnector *>(this->getConnectorById(id));;
        cm->setName(nameitem->text());
        cm->toolT();
    }
}

void CGCompositionProperties::close(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGCompositionProperties::close");
    this->hide();
}

CGConnector* CGCompositionProperties::getConnectorById(int id){
    QList<CGConnector*> cL = compositionModel->getOuterView()->getConnectorsList();
    foreach(CGConnector*c,cL){
        if(c->getId() == id){
            return c;
        }
    }
    return NULL;
}

void CGCompositionProperties::load(CGCompositionModel* cmodel){
    //column
    int i = 0;
    this->compositionModel = cmodel;
    nameEdit->setText(compositionModel->getName());
    edit->setText(compositionModel->getInformation());
    QList<CGConnector*> cL = cmodel->getOuterView()->getConnectorsList();
    foreach(CGConnector* c, cL){
        CGMultiConnector *cm =
                qgraphicsitem_cast<CGMultiConnector *>(c);
        QStandardItem *item = new QStandardItem(QString::number(c->getId()));
        model->setItem(i, 0, item);
        item->setEditable(false);
        QStandardItem *item3 = new QStandardItem(c->getSide());
        model->setItem(i, 2, item3);
        item3->setEditable(false);

        if(cm->getName().compare("")!=0
                && cm->getName().compare("Other side not connected")!=0
                && cm->getName().compare("Can't find name")!=0){
            QStandardItem *item2 = new QStandardItem(cm->getName());
            model->setItem(i, 1, item2);
        }else{
            if(cm->getOtherSide()->isConnected()){
                if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){

                    QStandardItem *item2 = new QStandardItem(cm->getOtherSide()->getConnectedConnector()->getModel()->getName());
                    model->setItem(i, 1, item2);
                }else{
                    QStandardItem *item2 = new QStandardItem("Can't find name");
                    model->setItem(i, 1, item2);
                }
            }else{
                QStandardItem *item2 = new QStandardItem("Other side not connected");
                model->setItem(i, 1, item2);
            }
        }

        if(cm->getOtherSide()->isConnected()){
            if(cm->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){

                QStandardItem *item4 = new QStandardItem(cm->getOtherSide()->getConnectedConnector()->getModel()->getDataType());
                model->setItem(i, 3, item4);
                item4->setEditable(false);
            }else{
                QStandardItem *item4 = new QStandardItem("UNKNOWN");
                model->setItem(i, 3, item4);
                item4->setEditable(false);
            }
        }else{
            QStandardItem *item4 = new QStandardItem("Other side not connected");
            model->setItem(i, 3, item4);
            item4->setEditable(false);
        }
        i++;
    }
    table->setModel(model);
    table->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void CGCompositionProperties::create(){
    model = new QStandardItemModel();
    table = new QTableView();
    table->setModel(model);
    model->setHorizontalHeaderItem( 0, new QStandardItem( QString("connector id" )) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( QString("connector name" )) );
    model->setHorizontalHeaderItem( 2, new QStandardItem( QString("connector side" )) );
    model->setHorizontalHeaderItem( 3, new QStandardItem( QString("connector type" )) );
}
