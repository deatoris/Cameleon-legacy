/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGInstanceProperties.h"
#include "CGInstance.h"
#include "CGInstanceDelegate.h"
#include "CGCommandManager.h"
CGInstanceProperties::CGInstanceProperties()
{
    QHBoxLayout* buttonLayout= new QHBoxLayout();
    QVBoxLayout* layout = new QVBoxLayout();

    this->setLayout(layout);

    tab = new QTabWidget();
    //    patterns = new QWidget();
    logs = new QWidget();
    libs = new QWidget();
    options = new QWidget();

    tab->addTab(options,"options");
    //    tab->addTab(patterns,"patterns");
    tab->addTab(libs,"installed dictionaries");
    //    tab->addTab(logs,"logs");

    QPushButton* applybutton = new QPushButton("apply");
    QPushButton* cancelbutton = new QPushButton("cancel");

    connect(applybutton, SIGNAL(clicked()),
            this, SLOT(save()));
    connect(cancelbutton, SIGNAL(clicked()),
            this, SLOT(close()));

    buttonLayout->addWidget(applybutton);
    buttonLayout->addWidget(cancelbutton);

    layout->addWidget(tab);
    layout->addLayout(buttonLayout);
    setWindowTitle("Cameleon Properties");

    this->createLibsTab();
    //    this->createLogsTab();
    this->createOptsTab();
    //    this->createPatternTab();

    this->resize(900,400);
    this->setWindowIcon(QIcon(":/icons/clogo.png"));
}

void CGInstanceProperties::load(){
    this->populateLibsTab();
    //    this->populateLogsTab();
    this->populateOptsTab();
    //    this->populatePatternTab();
}

//void CGInstanceProperties::createPatternTab(){
//    patterntable = new QTableView();
//    QVBoxLayout* layout = new QVBoxLayout();
//    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    QPushButton* installbutton = new QPushButton("install");
//    QPushButton* uninstallbutton = new QPushButton("uninstall selected");
//    QPushButton* exportbutton = new QPushButton("export selected");

//    buttonLayout->addWidget(installbutton);
//    buttonLayout->addWidget(uninstallbutton);
//    buttonLayout->addWidget(exportbutton);

//    layout->addWidget(patterntable);
//    layout->addLayout(buttonLayout);
//    patterntable->horizontalHeader()->resizeSections(QHeaderView::Stretch);
//    patterns->setLayout(layout);
//}

void CGInstanceProperties::createLibsTab(){
    libtable = new QTableView();
    QVBoxLayout* layout = new QVBoxLayout();
//    QHBoxLayout* buttonLayout = new QHBoxLayout();
//    QPushButton* installbutton = new QPushButton("install an external lib");


//    buttonLayout->addWidget(installbutton);

//    connect(installbutton, SIGNAL(clicked()),
//            this, SLOT(installLib()));

    layout->addWidget(libtable);
//    layout->addLayout(buttonLayout);
    libtable->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    libs->setLayout(layout);
}

//void CGInstanceProperties::createLogsTab(){
//    QVBoxLayout* layout = new QVBoxLayout();

//    logModel = new QStandardItemModel();
//    logTable = new QTableView();
//    logTable->setModel(logModel);
//    logModel->setHorizontalHeaderItem( 0, new QStandardItem( QString("component name" )) );
//    logModel->setHorizontalHeaderItem( 1, new QStandardItem( QString("log level" )) );

//    ComboBoxDelegate* delegate = new ComboBoxDelegate();
//    logTable->setItemDelegateForColumn(1,delegate);

//    logTable->horizontalHeader()->resizeSections(QHeaderView::Stretch);


//    logTable->setWindowTitle(QObject::tr("Spin Box Delegate"));

//    layout->addWidget(logTable);
//    logs->setLayout(layout);
//    logTable->show();
//}

void CGInstanceProperties::createOptsTab(){
    QLabel* instanceLabel = new QLabel("instance path");
    instanceEdit = new QTextEdit();
    instanceEdit->setMinimumHeight(30);
    instanceEdit->setMaximumHeight(30);
    instanceEdit->setEnabled(false);

    QLabel* patternLabel = new QLabel("pattern path");
    patternEdit = new QTextEdit();
    patternEdit->setMinimumHeight(30);
    patternEdit->setMaximumHeight(30);
    patternEdit->setFixedHeight(30);
    patternEdit->setEnabled(false);

    QLabel* logLabel = new QLabel("plugins path");
    pluginEdit = new QTextEdit();
    pluginEdit->setMinimumHeight(30);
    pluginEdit->setMaximumHeight(30);
    pluginEdit->setEnabled(false);

    QLabel* libLabel = new QLabel("dictionaries path");
    libEdit = new QTextEdit();
    libEdit->setMinimumHeight(30);
    libEdit->setMaximumHeight(30);
    libEdit->setEnabled(false);

    QLabel* projectLabel = new QLabel("projects path");
    projectEdit = new QTextEdit();
    projectEdit->setMinimumHeight(30);
    projectEdit->setMaximumHeight(30);
    projectEdit->setEnabled(false);

    QLabel* pressourceLabel = new QLabel("ressources path");
    ressourceEdit = new QTextEdit();
    ressourceEdit->setMinimumHeight(30);
    ressourceEdit->setMaximumHeight(30);
    ressourceEdit->setEnabled(false);

    QLabel* savecomp = new QLabel("auto-save step number");
    saveCompoEdit = new QComboBox();
    saveCompoEdit->addItem("1");
    saveCompoEdit->addItem("5");
    saveCompoEdit->addItem("10");
    saveCompoEdit->addItem("20");
    saveCompoEdit->addItem("30");
    saveCompoEdit->addItem("40");
    saveCompoEdit->setEnabled(true);

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(instanceLabel, 0, 0);
    gridLayout->addWidget(instanceEdit, 0, 1);
    gridLayout->addWidget(patternLabel, 1, 0);
    gridLayout->addWidget(patternEdit, 1, 1);
    gridLayout->addWidget(logLabel, 2, 0);
    gridLayout->addWidget(pluginEdit, 2, 1);
    gridLayout->addWidget(libLabel, 3, 0);
    gridLayout->addWidget(libEdit, 3, 1);
    gridLayout->addWidget(projectLabel, 4, 0);
    gridLayout->addWidget(projectEdit, 4, 1);
    gridLayout->addWidget(pressourceLabel, 5, 0);
    gridLayout->addWidget(ressourceEdit, 5, 1);
    gridLayout->addWidget(savecomp, 6, 0);
    gridLayout->addWidget(saveCompoEdit, 6, 1);

    options->setLayout(gridLayout);
}

void CGInstanceProperties::fillPatternsTable(QTableView* ){
    //    QMap<QString, QString> patterns = CGInstance::getInstance()->getPatterns();

    //    //column
    //    int i = 0;
    //    QMap<QString, QString>::iterator it = patterns.begin();
    //    QStandardItemModel *model = new QStandardItemModel();
    //    model->setHorizontalHeaderItem( 0, new QStandardItem( QString("name" )) );
    //    model->setHorizontalHeaderItem( 1, new QStandardItem( QString("path" )) );
    //    model->setHorizontalHeaderItem( 2, new QStandardItem( QString("selected" )) );
    //    while (it != patterns.end()) {
    //        CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"add pattern to table "+it.key());
    //        QStandardItem *item = new QStandardItem(it.key());
    //        model->setItem(i, 0, item);
    //        QStandardItem *item2 = new QStandardItem(it.value());
    //        model->setItem(i, 1, item2);
    //        item2->setEditable(false);
    //        QStandardItem* item0 = new QStandardItem(true);
    //        item0->setCheckable(true);
    //        item0->setCheckState(Qt::Checked);
    //        model->setItem(i, 2, item0);
    //        i++;
    //        it++;
    //    }
    //    table->setModel(model);
    //    table->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);

}

void CGInstanceProperties::fillLibsTable(QTableView* table){
    QMap<QString, QString> installedLibs = CGInstance::getInstance()->getInstalledLibs();
    QMap<QString, QString> activatedLibs = CGInstance::getInstance()->getActivatedLibs();
    QMap<QString, QString> installedLibsError = CGInstance::getInstance()->getInstalledLibsError();

    //column
    int i = 0;
    QMap<QString, QString>::iterator it = installedLibs.begin();
    model = new QStandardItemModel();
    model->setHorizontalHeaderItem( 0, new QStandardItem( QString("name" )) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( QString("path" )) );
    model->setHorizontalHeaderItem( 2, new QStandardItem( QString("version" )) );
    model->setHorizontalHeaderItem( 3, new QStandardItem( QString("activated" )) );
    while (it != installedLibs.end()) {
        QFileInfo info(it.key());
        QStandardItem *item = new QStandardItem(info.baseName());
        item->setEditable(false);
        model->setItem(i, 0, item);
        QStandardItem *item2 = new QStandardItem(CGInstance::getInstance()->makeRelative(it.key()));
        model->setItem(i, 1, item2);
        item2->setEditable(false);
        QStandardItem *item3 = new QStandardItem(it.value());
        model->setItem(i, 2, item3);
        item3->setEditable(false);
        QStandardItem* item0 = new QStandardItem(true);
        item0->setCheckable(true);
        item0->setCheckState(Qt::Unchecked);
        if(activatedLibs.contains(CGInstance::getInstance()->makeRelative(it.key()))){
                item0->setCheckState(Qt::Checked);
        }
        model->setItem(i, 3, item0);
        i++;
        it++;
    }
    it = installedLibsError.begin();
    while (it != installedLibsError.end()) {
        QFileInfo info(it.key());
        QStandardItem *item = new QStandardItem(info.baseName());
        model->setItem(i, 0, item);
        item->setEditable(false);
        QStandardItem *item2 = new QStandardItem(it.key());
        model->setItem(i, 1, item2);
        item2->setEditable(false);
        QStandardItem *item3 = new QStandardItem("ERROR");
        model->setItem(i, 2, item3);
        item3->setEditable(false);
        QStandardItem *item4 = new QStandardItem(it.value());
        model->setItem(i, 3, item4);
        item3->setEditable(false);
        i++;
        it++;
    }
    table->setModel(model);
    table->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);

    table->horizontalHeader()->resizeSections(QHeaderView::Stretch);
}

//void CGInstanceProperties::fillLogsTable(QTableView* table){
//    QMap<QString, CLogger::LVL> log = CLogger::getInstance()->getMap();

//    int i = 0;
//    QMap<QString, CLogger::LVL>::iterator it = log.begin();
//    while (it != log.end()) {
//        QStandardItem *item = new QStandardItem(it.key());
//        logModel->setItem(i, 0, item);
//        item->setEditable(false);

//        QModelIndex index = logModel->index(i, 1, QModelIndex());
//        logModel->setData(index, CLogger::getInstance()->toString(it.value()));
//        i++;
//        it++;
//    }
//}

void CGInstanceProperties::populateLibsTab(){
    fillLibsTable(libtable);
}

//void CGInstanceProperties::populateLogsTab(){
//    fillLogsTable(logTable);
//}

void CGInstanceProperties::populateOptsTab(){
    QFileInfo infos(CGInstance::getInstance()->getPath());
    instanceEdit->setText(infos.absolutePath());
    patternEdit->setText(CGInstance::getInstance()->getPatternPath());
    pluginEdit->setText(CGInstance::getInstance()->getPluginPath());
    libEdit->setText(CGInstance::getInstance()->getLibPath());
    projectEdit->setText(CGInstance::getInstance()->getProjectPath());
    ressourceEdit->setText(CGInstance::getInstance()->getRessourcePath());
    int i = saveCompoEdit->findText(QString::number(CGCommandManager::getInstance()->getStep()));
    saveCompoEdit->setCurrentIndex(i);
}

//void CGInstanceProperties::populatePatternTab(){
//    fillPatternsTable(patterntable);
//}

void CGInstanceProperties::save(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGInstanceProperties::save");

    int r = model->rowCount();
//    QMap<QString, QString> activatedLibs = CGInstance::getInstance()->getActivatedLibs();
    CGInstance::getInstance()->clearActivated();
    for(int i = 0;i<r;i++){
        QStandardItem*item = model->item(i,1);
        QString lib = item->text();
        item = model->item(i,2);
        QString version = item->text();
        item = model->item(i,3);
        Qt::CheckState state = item->checkState();

        if(state == Qt::Checked){
            CGInstance::getInstance()->activeLib(lib,version,true);
        }
    }

    CGCommandManager::getInstance()->setStep(saveCompoEdit->currentText().toInt());

    CGInstance::getInstance()->save();
}

void CGInstanceProperties::close(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGInstanceProperties::close");
    this->hide();
}

void CGInstanceProperties::installLib(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGInstanceProperties::installLib");

}
