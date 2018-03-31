/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGProjectProperties.h"
#include "CGProject.h"
#include "CLogger.h"
#include "../CGInstance.h"
#include "../CGInstanceDelegate.h"
#include "../CGComposer/CGCompositionManager.h"
CGProjectProperties::CGProjectProperties()
{
    QHBoxLayout* buttonLayout= new QHBoxLayout();
    QVBoxLayout* layout = new QVBoxLayout();

    this->setLayout(layout);

    tab = new QTabWidget();
    logs = new QWidget();
    libs = new QWidget();
    options = new QWidget();
    readme = new QWidget();

    tab->addTab(options,"options");
//    tab->addTab(libs,"project dependencies");
    tab->addTab(readme,"readme");
    tab->addTab(logs,"console outputs");

    QPushButton* applybutton = new QPushButton("apply");
    QPushButton* cancelbutton = new QPushButton("cancel");

    connect(applybutton, SIGNAL(clicked()),
            this, SLOT(save()));
    connect(cancelbutton, SIGNAL(clicked()),
            this, SLOT(cancel()));


    buttonLayout->addWidget(applybutton);
    buttonLayout->addWidget(cancelbutton);

    layout->addWidget(tab);
    layout->addLayout(buttonLayout);
    setWindowTitle("Composition Properties");

    this->resize(900,500);

    this->createLibsTab();
    this->createLogsTab();
    this->createOptsTab();
    this->setWindowIcon(QIcon(":/icons/clogo.png"));
}

void CGProjectProperties::save(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGProjectProperties::save");
    QString name = nameEdit->toPlainText();
    QString version = versionEdit->toPlainText();
    int r = logModel->rowCount();

    for(int i = 0;i<r;i++){
        QStandardItem*item = logModel->item(i,0);
        QString component = item->text();
        item = logModel->item(i,1);
        QString lvl = item->text();
        CLogger::getInstance()->setComponentLevel(component,CLogger::fromString(lvl));
    }

    CGProject::getInstance()->setNumberThread(boxthreads->currentText().toInt());
    CGProject::getInstance()->setName(name);
    CGProject::getInstance()->setVersion(version);
    CGProject::getInstance()->setConnectorOrderMode(boxChoice->currentText());
    CGCompositionManager::getInstance()->getComposer()->saveComposition();
    CGProject::getInstance()->setReadMe(readmeEdit->toPlainText());
    if(checkLog->isChecked()){
        CLogger::getInstance()->setActive(true);
    }else{
        CLogger::getInstance()->setActive(false);
    }
    CGProject::getInstance()->save();
}

void CGProjectProperties::cancel(){
    CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"CGProjectProperties::cancel");
    this->hide();
}

void CGProjectProperties::load(){
    this->populateLibsTab();
    this->populateLogsTab();
    this->populateOptsTab();
}

void CGProjectProperties::populateLibsTab(){
    QMap<QString, QString> installedLibsError = CGInstance::getInstance()->getInstalledLibsError();
    QMap<QString, QString> activatedLibs = CGInstance::getInstance()->getActivatedLibs();

    //TODO: uniquement les librairies dont est dépendant le projet.

    //column
    int i = 0;
    QMap<QString, QString>::iterator it = activatedLibs.begin();
    QStandardItemModel *model = new QStandardItemModel();
    model->setHorizontalHeaderItem( 0, new QStandardItem( QString("name" )) );
    model->setHorizontalHeaderItem( 1, new QStandardItem( QString("path" )) );
    model->setHorizontalHeaderItem( 2, new QStandardItem( QString("version" )) );
    model->setHorizontalHeaderItem( 3, new QStandardItem( QString("status" )) );
    while (it != activatedLibs.end()) {
        QFileInfo info(it.key());
        QStandardItem *item = new QStandardItem(info.baseName());
        model->setItem(i, 0, item);
        item->setEditable(false);
        QStandardItem *item2 = new QStandardItem(it.key());
        model->setItem(i, 1, item2);
        item2->setEditable(false);
        QStandardItem *item3 = new QStandardItem(it.value());
        model->setItem(i, 2, item3);
        item3->setEditable(false);
        QStandardItem *item4 = new QStandardItem("loaded");
        model->setItem(i, 3, item4);
        item3->setEditable(false);
        i++;
        it++;
    }
    i = 0;
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
    libtable->setModel(model);
    libtable->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    libtable->horizontalHeader()->resizeSections(QHeaderView::Stretch);
}

void CGProjectProperties::populateLogsTab(){
    QMap<QString, CLogger::LVL> log = CLogger::getInstance()->getMap();
    logModel->clear();

    logModel->setHorizontalHeaderItem( 0, new QStandardItem( QString("component name" )) );
    logModel->setHorizontalHeaderItem( 1, new QStandardItem( QString("log level" )) );

    int i = 0;
    QMap<QString, CLogger::LVL>::iterator it = log.begin();
    while (it != log.end()) {
        CLogger::getInstance()->log("INSTANCE",CLogger::INFO,"add pattern to table "+it.key());
        QStandardItem *item = new QStandardItem(it.key());
        logModel->setItem(i, 0, item);
        item->setEditable(false);

        QModelIndex index = logModel->index(i, 1, QModelIndex());
        logModel->setData(index, CLogger::getInstance()->toString(it.value()));
        i++;
        it++;
    }
    logTable->horizontalHeader()->resizeSections(QHeaderView::Stretch);
    if(CLogger::getInstance()->getActive()){
        checkLog->setChecked(true);
    }else{
        checkLog->setChecked(false);
    }
}

void CGProjectProperties::populateOptsTab(){
    QFileInfo infos(CGProject::getInstance()->getPath());
    nameEdit->setText(CGProject::getInstance()->getName());
    versionEdit->setText(CGProject::getInstance()->getVersion());
    tmpEdit->setText(CGProject::getInstance()->getTmpPath());
    logEdit->setText(CGProject::getInstance()->getLogPath());
    targetEdit->setText(CGProject::getInstance()->getTargetPath());
    projectEdit->setText(infos.absolutePath());
    //THREAD BLOC
    int i = 0;//boxthreads->findText(QString::number(CGProject::getInstance()->getNumberThread()));
    boxthreads->setCurrentIndex(i);
    boxthreads->setEnabled(false);
    ressourceEdit->setText(CGProject::getInstance()->getRessourcePath());
    if(CGProject::getInstance()->getConnectorOrderMode().compare("FREE")==0){
        boxChoice->setCurrentIndex(0);
    }else if(CGProject::getInstance()->getConnectorOrderMode().compare("MEDIUM")==0){
        boxChoice->setCurrentIndex(1);
    }else if(CGProject::getInstance()->getConnectorOrderMode().compare("HARD")==0){
        boxChoice->setCurrentIndex(2);
    }
    readmeEdit->setText(CGProject::getInstance()->getReadMe());
}

void CGProjectProperties::createLibsTab(){
    libtable = new QTableView();
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(libtable);
    libs->setLayout(layout);
}

void CGProjectProperties::createLogsTab(){
    QVBoxLayout* layout = new QVBoxLayout();

    logModel = new QStandardItemModel();
    logTable = new QTableView();
    logTable->setModel(logModel);
    logModel->setHorizontalHeaderItem( 0, new QStandardItem( QString("component name" )) );
    logModel->setHorizontalHeaderItem( 1, new QStandardItem( QString("log level" )) );

    ComboBoxDelegate* delegate = new ComboBoxDelegate();
    logTable->setItemDelegateForColumn(1,delegate);

    logTable->setWindowTitle(QObject::tr("Spin Box Delegate"));

    checkLog = new QCheckBox("log enabled?");

    layout->addWidget(checkLog);
    layout->addWidget(logTable);
    logs->setLayout(layout);
    logTable->show();
}

void CGProjectProperties::createOptsTab(){
    QLabel* nameLabel = new QLabel("composition name");
    nameEdit = new QTextEdit();
    nameEdit->setMinimumHeight(30);
    nameEdit->setMaximumHeight(30);

    QLabel* versionLabel = new QLabel("composition version");
    versionEdit = new QTextEdit();
    versionEdit->setMinimumHeight(30);
    versionEdit->setMaximumHeight(30);
    versionEdit->setFixedHeight(30);

    QLabel* tmpLabel = new QLabel("tmp path");
    tmpEdit = new QTextEdit();
    tmpEdit->setMinimumHeight(30);
    tmpEdit->setMaximumHeight(30);
    tmpEdit->setFixedHeight(30);
    tmpEdit->setEnabled(false);

    QLabel* logLabel = new QLabel("log path");
    logEdit = new QTextEdit();
    logEdit->setMinimumHeight(30);
    logEdit->setMaximumHeight(30);
    logEdit->setEnabled(false);

    QLabel* projectLabel = new QLabel("compositions path");
    projectEdit = new QTextEdit();
    projectEdit->setMinimumHeight(30);
    projectEdit->setMaximumHeight(30);
    projectEdit->setEnabled(false);

    QLabel* ressourceLabel = new QLabel("ressources path");
    ressourceEdit = new QTextEdit();
    ressourceEdit->setMinimumHeight(30);
    ressourceEdit->setMaximumHeight(30);
    ressourceEdit->setEnabled(false);

    QLabel* targetLabel = new QLabel("target directory");
    targetEdit = new QTextEdit();
    targetEdit->setMinimumHeight(30);
    targetEdit->setMaximumHeight(30);
    targetEdit->setEnabled(false);

    QLabel* choiceLabel = new QLabel("connectors behavior");
    boxChoice = new QComboBox();
    boxChoice->addItem("FREE");
    boxChoice->addItem("MEDIUM");
    boxChoice->addItem("HARD");
    boxChoice->setEnabled(false);

    QLabel* boxthreadsLabel = new QLabel("max threads");
    boxthreads = new QComboBox();
    boxthreads->addItem("1");
    boxthreads->addItem("2");
    boxthreads->addItem("4");
    boxthreads->addItem("8");
    boxthreads->addItem("16");
    boxthreads->addItem("32");
    boxthreads->addItem("64");
    boxthreads->addItem("128");
    boxthreads->addItem("256");
    boxthreads->addItem("512");

    QGridLayout *gridLayout = new QGridLayout;
    gridLayout->addWidget(nameLabel, 0, 0);
    gridLayout->addWidget(nameEdit, 0, 1);
    gridLayout->addWidget(versionLabel, 1, 0);
    gridLayout->addWidget(versionEdit, 1, 1);
    gridLayout->addWidget(tmpLabel, 2, 0);
    gridLayout->addWidget(tmpEdit, 2, 1);
    gridLayout->addWidget(logLabel, 3, 0);
    gridLayout->addWidget(logEdit, 3, 1);
    gridLayout->addWidget(projectLabel, 4, 0);
    gridLayout->addWidget(projectEdit, 4, 1);
    gridLayout->addWidget(ressourceLabel, 5, 0);
    gridLayout->addWidget(ressourceEdit, 5, 1);
    gridLayout->addWidget(targetLabel, 6, 0);
    gridLayout->addWidget(targetEdit, 6, 1);
    gridLayout->addWidget(choiceLabel, 7, 0);
    gridLayout->addWidget(boxChoice, 7, 1);
    gridLayout->addWidget(boxthreadsLabel, 8, 0);
    gridLayout->addWidget(boxthreads, 8, 1);


    options->setLayout(gridLayout);

    //readme part
    readmeEdit = new QTextEdit();
    QHBoxLayout* lay = new QHBoxLayout();
    lay->addWidget(readmeEdit);
    readme->setLayout(lay);
}
