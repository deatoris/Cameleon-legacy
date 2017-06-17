/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGErrorDialog.h"

#include "CGCompositionManager.h"
#include "CGLayoutManager.h"
#include "CGCompositionItem.h"
#include "CLogger.h"
CGErrorDialog::CGErrorDialog()
{
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* buttonLayout= new QHBoxLayout();
    prevbutton = new QPushButton("previous");
    nextbutton = new QPushButton("next");
    QPushButton* cancelbutton = new QPushButton("cancel");

    buttonLayout->addWidget(prevbutton);
    buttonLayout->addWidget(nextbutton);
    //    buttonLayout->addWidget(cancelbutton);
    connect(prevbutton, SIGNAL(clicked()),
            this, SLOT(previous()));
    connect(nextbutton, SIGNAL(clicked()),
            this, SLOT(next()));
    connect(cancelbutton, SIGNAL(clicked()),
            this, SLOT(close()));

    this->setLayout(layout);
    this->create();

    //    layout->addLayout(buttonLayout);
    layout->addWidget(table);

    this->resize(750,500);
    this->setWindowIcon(QIcon(":/icons/clogo.png"));
    current = -1;
    max = 0;
    prevbutton->setEnabled(false);
}

void CGErrorDialog::close(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGErrorDialog::close");
    this->hide();
}

void CGErrorDialog::previous(){
    current = current-1;
    this->initButton();
    if(current != -1 && current != max+1) this->showCurrent(current);
}

void CGErrorDialog::initButton(){
    if(current == max){
        nextbutton->setEnabled(false);
    }else{
        nextbutton->setEnabled(true);
    }

    if(current == 0){
        prevbutton->setEnabled(false);
    }else{
        prevbutton->setEnabled(true);
    }
}


void CGErrorDialog::next(){
    current = current+1;
    this->initButton();
    if(current != -1 && current != max+1) this->showCurrent(current);
}

void CGErrorDialog::rowSelection(int i){
    this->showCurrent(i);
}

void CGErrorDialog::showCurrent(int current){
    table->selectRow(current);
    QStandardItem *itemType = model->item(current,0);
    QString type = itemType->text();
    QStandardItem *itemId = model->item(current,2);
    QString sid = itemId->text();
    int id = sid.toInt();

    if(type.compare("operator") == 0){
        CGOperatorModel* ope = CGCompositionManager::getInstance()->searchOperatorById(id);
        CGComposition*composition = (CGComposition*)ope->getView()->scene();
        ope->getView()->setSelected(true);
        CGCompositionManager::getInstance()->showSceneFromModel(composition->getModel());
        composition->unselectAll();
        composition->getView()->centerOn(ope->getView()->scenePos());
        ope->getView()->setSelected(true);
        ope->getView()->setIsHighlight(true);
        ope->getView()->update();
    }else if(type.compare("control") == 0){
        CGLayoutItem* lay = CGLayoutManager::getInstance()->getItemById(id);
        if(lay!=0){
            CGLayout* layout = lay->getLayout();
            if(layout!=0){
                CGLayoutManager::getInstance()->showLayout(layout->getId());
                layout->unselectAll();
                layout->getView()->centerOn(lay->scenePos());
            }else{
                QGraphicsScene* scene = lay->scene();
                if(CGComposition* compo = dynamic_cast<CGComposition*>(scene)){
                    compo->unselectAll();
                    CGCompositionManager::getInstance()->showSceneFromModel(compo->getModel());
                    compo->getView()->centerOn(lay->scenePos());
                }
            }
            lay->setSelected(true);
        }
    }
}

void CGErrorDialog::clear(){
    model->clear();

    model->setHorizontalHeaderItem( 0, new QStandardItem( QString("component type" )) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( QString("component name" )) );
    model->setHorizontalHeaderItem( 2, new QStandardItem( QString("component id" )) );
    model->setHorizontalHeaderItem( 3, new QStandardItem( QString("error message" )) );

}

void CGErrorDialog::load(){
    //column
    int i = 0;
    current = 0;
    max = 0;
    model->clear();

    model->setHorizontalHeaderItem( 0, new QStandardItem( QString("component type" )) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( QString("component name" )) );
    model->setHorizontalHeaderItem( 2, new QStandardItem( QString("component id" )) );
    model->setHorizontalHeaderItem( 3, new QStandardItem( QString("error message" )) );

    QMap<int,QString> opeError = CGCompositionManager::getInstance()->getOpeErrorMap();
    QMap<int,QString> ctlError = CGCompositionManager::getInstance()->getCtlErrorMap();

    QMapIterator<int, QString> it(opeError);
    while (it.hasNext()) {
        it.next();

        QStandardItem *item1 = new QStandardItem("operator");
        item1->setEditable(false);
        model->setItem(i, 0, item1);

        CGOperatorModel* o = CGCompositionManager::getInstance()->searchOperatorById(it.key());
        QStandardItem *item2 = new QStandardItem(o->getName());
        item2->setEditable(false);
        model->setItem(i, 1, item2);

        QStandardItem *item3 = new QStandardItem(QString::number(it.key()));
        item3->setEditable(false);
        model->setItem(i, 2, item3);

        QStandardItem *item4 = new QStandardItem(it.value());
        item4->setEditable(false);
        model->setItem(i, 3, item4);

        i++;
    }

    QMapIterator<int, QString> it2(ctlError);
    while (it2.hasNext()) {
        it2.next();

        QStandardItem *item1 = new QStandardItem("control");
        item1->setEditable(false);
        model->setItem(i, 0, item1);

        CGLayoutItem* o = CGLayoutManager::getInstance()->getItemById(it2.key());
        QStandardItem *item2 = new QStandardItem(o->getControl()->getName().c_str());
        item2->setEditable(false);
        model->setItem(i, 1, item2);

        QStandardItem *item3 = new QStandardItem(QString::number(it2.key()));
        item3->setEditable(false);
        model->setItem(i, 2, item3);

        QStandardItem *item4 = new QStandardItem(it2.value());
        item4->setEditable(false);
        model->setItem(i, 3, item4);

        i++;
    }

    table->setModel(model);
    table->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    table->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    max = i-1;

    this->initButton();
    if(current != -1 && current != max+1) this->showCurrent(current);
}

void CGErrorDialog::create(){
    model = new QStandardItemModel();
    table = new QTableView();
    table->setModel(model);
    model->setHorizontalHeaderItem( 0, new QStandardItem( QString("component type" )) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( QString("component name" )) );
    model->setHorizontalHeaderItem( 2, new QStandardItem( QString("component id" )) );
    model->setHorizontalHeaderItem( 3, new QStandardItem( QString("error message" )) );
    table->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    connect( (QObject*) table->verticalHeader(), SIGNAL( sectionClicked(int) ), this, SLOT( rowSelection( int ) ) );

}
