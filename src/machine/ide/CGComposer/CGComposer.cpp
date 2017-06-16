/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include <CClient.h>

#include "CGComposer.h"
#include "CGCompositionFactory.h"
#include "CGCompositionManager.h"
#include "CGInstance.h"
#include "CGProject.h"
#include "CGProjectTree.h"
#include "CGChangeView.h"
#include "CGCreateOperator.h"
#include "CGDisconnectConnector.h"
#include "CGConnectConnector.h"
#include "CGDeleteOperator.h"
#include "CGClear.h"
#include "CGLayoutItem.h"
#include "CGLayoutManager.h"
#include "CLogger.h"
#include "CGGlossary.h"
#include "CGInstanceDelegate.h"
#include "CGCompositionProperties.h"
#include "CGDeleteControl.h"
#include "CGCreateControl.h"

CGComposer::CGComposer(QWidget *parent)
    : QMainWindow(parent)
{
    previousActionList = 0;
    isNext = false;
    this->setObjectName("composer");
    this->hide();
    hasBeenEdited = false;
    editLogger = new QTextEdit();
    this->createActions();
    this->createPlayerBar();
    this->createCommentBar();
    //    this->createFactory();
    this->createMachineDock();
    this->createMenus();
    this->createScene();
    compositionModeAction->setChecked(true);
    copyPast=NULL;
    this->loadOpenRecent();
    alreadyClosed = false;

    properties = new CGInstanceProperties();
    projectProperties = new CGProjectProperties();
    this->setContentsMargins(0,0,0,0);
    this->setWindowIcon(QIcon(":/icons/clogo.png"));

    showingInfos = false;
    currentColor = Qt::white;
    isstopped = true;

    dialogThread = new CGThreadInProgressDialog();
    //    CLogger::getInstance()->registerComponent("STOPPROCESS");
}

CGComposer::~CGComposer()
{

}

//composer construct method
void CGComposer::init(){

}

QTabWidget* CGComposer::getMachineTab(){
    return tab;
}

void CGComposer::createMachineDock(){
    tab = new QTabWidget();
    QFont serifFont("Courier", 9);
    editLogger->setFont(serifFont);
    editLogger->append("HELP - TO ACTIVATE OUTPUTS, SEE Properties>project>console outputs.");
    tab->addTab(editLogger,"console outputs");
    tab->addTab(CGCompositionManager::getInstance()->getErrorDial(),"Execution problems");

    if(!QObject::connect(CLogger::getInstance(), SIGNAL(log(QString)),editLogger, SLOT(append(QString)))){
        //        //qDebug << "Aie aie aie";
    }

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(tab);

    QWidget* dockWidget = new QWidget();
    dockWidget->setLayout(layout);

    layout->setSpacing(0);
    layout->setMargin(0);

    dockMachine = new QDockWidget(QObject::tr("Cameleon Outputs"));

    dockMachine->setWidget(dockWidget);
    dockWidget->setParent(dockMachine);

    addDockWidget(Qt::BottomDockWidgetArea, dockMachine);
    dockMachine->hide();
}

void CGComposer::enterSearch(){
    if(!hasBeenEdited && edit->toPlainText().compare("search") == 0){
        hasBeenEdited = true;
        edit->setText("");
    }
}

void CGComposer::searchOperator(){
    CGCompositionFactory::getInstance()->search(edit->toPlainText());
}

void CGComposer::createFactory(){
    dockFactory = CGGlossary::getInstance()->getWidgetCopy();
    dockFactory->setObjectName("composerfactory");
    dockProject = CGProject::getInstance()->getWidgetCopy();
    dockProject->setObjectName("composerproject");

    addDockWidget(Qt::LeftDockWidgetArea, dockFactory);
    addDockWidget(Qt::RightDockWidgetArea, dockProject);
    dockProject->hide();
    //    this->tabifyDockWidget(dockProject,dockFactory);
}

void CGComposer::restart(){
    this->saveComposition();
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void CGComposer::openRecent(QAction * action){
    QString fileName = action->text();
    if(fileName.compare("")!=0 && fileName.compare(CGProject::getInstance()->getPath()) != 0){
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Loader"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString(),QMessageBox::NoButton,QMessageBox::NoButton));
        }else{
            //call project close
            this->stop();
            if(this->clear()){
                CGProject::getInstance()->setPath(fileName);
                CGInstance::getInstance()->addRecentProject(fileName);
                CGProject::getInstance()->load();
                loadOpenRecent();
                CGCompositionManager::getInstance()->showSceneFromModel(CGCompositionManager::getInstance()->getCurrentCompositionModel());
                setWindowTitle(tr("Caméléon Composer (beta) %1").arg(fileName));
                CGLayoutManager::getInstance()->getController()->setWindowTitle(tr("Caméléon Controller (beta) %1").arg(fileName));
                if(CGProject::getInstance()->getShowController()){
                    CGLayoutManager::getInstance()->getController()->show();
                }else{
                    CGLayoutManager::getInstance()->getController()->hide();
                }
            }
        }
    }
}

void CGComposer::loadOpenRecent(){
    openRecentMenu->clear();
    QList<QString> list = CGInstance::getInstance()->getRecentProjects();
    foreach(QString s, list){
        QAction* newProjectAction= new QAction(QIcon(":/icons/page_save.png"),
                                               QObject::tr(s.toStdString().c_str()), this);
        newProjectAction->setStatusTip(QObject::tr(s.toStdString().c_str()));
        connect(openRecentMenu, SIGNAL(triggered(QAction*)),
                this, SLOT(openRecent(QAction*)));

        openRecentMenu->addAction(newProjectAction);
    }
}

void CGComposer::createMenus(){

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newProjectAction);

    openRecentMenu = new QMenu("open recent project");
    openRecentMenu->setIcon(QIcon(":/icons/page_save.png"));
    fileMenu->addMenu(openRecentMenu);
    fileMenu->addAction(openCompositionAction);
    fileMenu->addAction(saveCompositionAction);
    fileMenu->addAction(saveasCompositionAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);
    fileMenu->addAction(restartAction);

    windowMenu  = menuBar()->addMenu(tr("&Windows"));
    windowMenu->addAction(showController);
    windowMenu->addAction(showProjectAction);
    windowMenu->addAction(showLogAction);
    windowMenu->addAction(showFactoryAction);

    toolMenu = menuBar()->addMenu(tr("&Properties"));
    //    toolMenu->addAction(buildAction);
    //    toolMenu->addAction(showPreviewAction);
    //    toolMenu->addSeparator();
    //    toolMenu->addAction(installPatternAction);
    //    toolMenu->addAction(exportPatternAction);
    //    toolMenu->addSeparator();

    //    toolMenu->addAction(installLibAction);
    toolMenu->addAction(workspacePropertiesAction);
    toolMenu->addAction(compositionPropertiesAction);

    documentationMenu = new QMenu("documentation");
    documentationMenu->addAction(documentationAction);
    documentationMenu->addAction(videoAction);
    documentationMenu->addAction(populationAction);
    documentationMenu->addAction(csdAction);
    //    documentationMenu->addAction(webAction);
    documentationMenu->setIcon(QIcon(":/icons/book.png"));

    helpMenu = menuBar()->addMenu(tr("&Help"));

    QMenu *networkMenu = new QMenu("community");
    networkMenu->setIcon(QIcon(":/icons/world.png"));
    networkMenu->addAction(sharePatternAction);
    networkMenu->addAction(shareProjectAction);
    networkMenu->addAction(forumProjectAction);
    networkMenu->addAction(homeAction);

    helpMenu->addAction(documentationAction);
    helpMenu->addAction(sharePatternAction);
    helpMenu->addAction(reportABugAction);
    helpMenu->addAction(aboutAction);

    operatorMenu = new QMenu();
    operatorMenu->addAction(showInfos);
    //    operatorMenu->addAction(copyAction);
    //    operatorMenu->addAction(cutAction);
    //    operatorMenu->addAction(pasteAction);
    operatorMenu->addAction(deleteAction);

    operatorMultiMenu = new QMenu();
    operatorMultiMenu->addAction(showInfos);
    operatorMultiMenu->addAction(createPatternAction);
    operatorMultiMenu->addAction(multiPropertiesAction);
    operatorMultiMenu->addSeparator();
    //    operatorMultiMenu->addAction(copyAction);
    //    //    operatorMultiMenu->addAction(cutAction);
    //    operatorMultiMenu->addAction(pasteAction);
    operatorMultiMenu->addAction(deleteAction);
    operatorMultiMenu->addSeparator();
    operatorMultiMenu->addAction(unmergeAction);

    connectorMenu = new QMenu();
    connectorMenu->addAction(disconnectAction);
    connectorMenu->addAction(disconnectControlAction);
    connectorMenu->addAction(showControlAction);
    //    connectorMenu->addAction(addConnectorAction);
    //    connectorMenu->addAction(rmvConnectorAction);

    controlComposerMenu = new QMenu();
    controlComposerMenu->addAction(deleteAction);

    connectorControlMenu = new QMenu();
    connectorControlMenu->addAction(disconnectConnectorControlAction);

    multiMenu = new QMenu();
    multiMenu->addAction(showInfos);
    multiMenu->addAction(mergeAction);
    multiMenu->addSeparator();
    //    multiMenu->addAction(copyAction);
    //    multiMenu->addAction(cutAction);
    //    multiMenu->addAction(pasteAction);
    multiMenu->addAction(deleteAction);

    compositionMenu = new QMenu();
    compositionMenu->addAction(showInfos);
    compositionMenu->addAction(upAction);
    compositionMenu->addSeparator();
//    compositionMenu->addAction(pasteAction);
    compositionMenu->addAction(createMultiAction);
    //    compositionMenu->addAction(clearAction);
    compositionMenu->addSeparator();
    compositionMenu->addAction(compositionPropertiesAction);
    compositionMenu->addAction(workspacePropertiesAction);
}

void CGComposer::createPlayerBar(){
    bar = new QToolBar("Player bar", this);
    bar->setGeometry(bar->geometry().x(),bar->geometry().y(),bar->geometry().width(),bar->geometry().height()-10);
    bar->addAction(playAction);
    bar->addAction(pauseAction);
    bar->addAction(stopAction);
    bar->addAction(nextStepAction);
    bar->addSeparator();
    bar->addAction(compositionModeAction);
    //    bar->addAction(markerModeAction);
    bar->addAction(shapeAction);
    bar->addAction(textAction);
    bar->addSeparator();

    addToolBar(Qt::TopToolBarArea, bar);
}

void CGComposer::createCommentBar(){

    fontCombo = new QFontComboBox();
    connect(fontCombo, SIGNAL(currentFontChanged(QFont)),
            this, SLOT(currentFontChanged(QFont)));

    fontSizeCombo = new QComboBox;
    fontSizeCombo->setEditable(true);
    for (int i = 8; i < 30; i = i + 2)
        fontSizeCombo->addItem(QString().setNum(i));
    QIntValidator *validator = new QIntValidator(2, 64, this);
    fontSizeCombo->setValidator(validator);
    connect(fontSizeCombo, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(fontSizeChanged(QString)));

    colorToolButton = new QToolButton;
    colorToolButton->setPopupMode(QToolButton::MenuButtonPopup);
    colorToolButton->setMenu(createColorMenu(SLOT(colorChanged()),
                                             Qt::white));
    colorToolButton->setIcon(createColorToolButtonIcon(Qt::white));
    connect(colorToolButton, SIGNAL(clicked()),
            this, SLOT(buttonTriggered()));


    textToolBar = addToolBar(tr("Font"));
    textToolBar->addAction(toFrontAction);
    textToolBar->addAction(sendBackAction);
    textToolBar->addSeparator();
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    textToolBar->addWidget(colorToolButton);
    //    textToolBar->addSeparator();
    //    textToolBar->addAction(buildAction);
    addToolBar(Qt::TopToolBarArea, textToolBar);


}

void CGComposer::closeEvent(QCloseEvent *)
{
    if(!alreadyClosed){
        CGLayoutManager::getInstance()->getController()->close();
        QMainWindow::close();
    }
    CGProject::getInstance()->emptyTmpFiles();
    alreadyClosed = true;
}

void CGComposer::createActions(){
    showController= new QAction(QIcon(":/icons/application_form.png"),
                                "controller", this);
    showController->setStatusTip("controller");
    connect(showController, SIGNAL(triggered()),
            this, SLOT(showControler()));

    restartAction= new QAction(QIcon(":/icons/cog.png"),
                               "save and restart", this);
    restartAction->setStatusTip("save and restart");
    connect(restartAction, SIGNAL(triggered()),
            this, SLOT(restart()));

    multiPropertiesAction= new QAction(QIcon(":/icons/cog.png"),
                                       tr("properties ..."), this);
    multiPropertiesAction->setStatusTip(tr("properties ..."));
    connect(multiPropertiesAction, SIGNAL(triggered()),
            this, SLOT(multiProperties()));

    installLibAction= new QAction(QIcon(":/icons/bricks.png"),
                                  tr("install dicitonnary ..."), this);
    installLibAction->setStatusTip(tr("install dictionnary ..."));
    connect(installLibAction, SIGNAL(triggered()),
            this, SLOT(openComposition()));

    installPatternAction= new QAction(QIcon(":/icons/plugin.png"),
                                      tr("import patterns ..."), this);
    installPatternAction->setStatusTip(tr("import patterns ..."));
    connect(installPatternAction, SIGNAL(triggered()),
            this, SLOT(openComposition()));

    openCompositionAction= new QAction(QIcon(":/icons/page_save.png"),
                                       tr("open project ..."), this);
    openCompositionAction->setShortcut(tr("Ctrl+O"));
    openCompositionAction->setStatusTip(tr("open composition ..."));
    connect(openCompositionAction, SIGNAL(triggered()),
            this, SLOT(openComposition()));

    newProjectAction= new QAction(QIcon(":/icons/page_save.png"),
                                  tr("new project ..."), this);
    newProjectAction->setShortcut(tr("Ctrl+N"));
    newProjectAction->setStatusTip(tr("new project ..."));
    connect(newProjectAction, SIGNAL(triggered()),
            this, SLOT(newComposition()));

    newLayoutAction= new QAction(QIcon(":/icons/page_save.png"),
                                 tr("new layout ..."), this);
    newLayoutAction->setStatusTip(tr("new layout ..."));
    connect(newLayoutAction, SIGNAL(triggered()),
            this, SLOT(newLayout()));

    newCompositionAction= new QAction(QIcon(":/icons/page_save.png"),
                                      tr("new composition ..."), this);
    newCompositionAction->setStatusTip(tr("new composition ..."));
    connect(newCompositionAction, SIGNAL(triggered()),
            this, SLOT(newComposition()));

    saveCompositionAction= new QAction(QIcon(":/icons/page_save.png"),
                                       tr("save project"), this);
    saveCompositionAction->setShortcut(tr("Ctrl+S"));
    saveCompositionAction->setStatusTip(tr("save project"));
    connect(saveCompositionAction, SIGNAL(triggered()),
            this, SLOT(saveComposition()));

    saveasCompositionAction= new QAction(QIcon(":/icons/page_save.png"),
                                         tr("save project as ..."), this);
    saveasCompositionAction->setStatusTip(tr("save project as ..."));
    connect(saveasCompositionAction, SIGNAL(triggered()),
            this, SLOT(saveAsComposition()));

    exitAction= new QAction(QIcon(":/icons/cross_shield_2.png"),
                            tr("exit"), this);
    exitAction->setStatusTip(tr("exit"));
    connect(exitAction, SIGNAL(triggered()),
            this, SLOT(close()));

    //Tools menu's actions
    networkAction= new QAction(QIcon(":/icons/world.png"),
                               tr("network ..."), this);
    networkAction->setStatusTip(tr("network ..."));
    connect(networkAction, SIGNAL(triggered()),
            this, SLOT(network()));

    buildAction= new QAction(QIcon(":/icons/hammer.png"),
                             tr("export ..."), this);
    buildAction->setStatusTip(tr("export ..."));
    connect(buildAction, SIGNAL(triggered()),
            this, SLOT(build()));

    workspacePropertiesAction= new QAction(QIcon(":/icons/cog.png"),
                                           tr("cameleon"), this);
    workspacePropertiesAction->setStatusTip(tr("cameleon"));
    connect(workspacePropertiesAction, SIGNAL(triggered()),
            this, SLOT(workspaceProperties()));

    compositionPropertiesAction= new QAction(QIcon(":/icons/cog.png"),
                                             tr("project"), this);
    compositionPropertiesAction->setStatusTip(tr("project"));
    connect(compositionPropertiesAction, SIGNAL(triggered()),
            this, SLOT(compositionProperties()));

    showLogAction= new QAction(QIcon(":/icons/page_white_text.png"),
                               tr("console output"), this);
    showLogAction->setStatusTip(tr("console output"));
    connect(showLogAction, SIGNAL(triggered()),
            this, SLOT(showlog()));

    showProcessorStackAction= new QAction(QIcon(":/icons/database.png"),
                                          tr("stack"), this);
    showProcessorStackAction->setStatusTip(tr("stack"));
    connect(showProcessorStackAction, SIGNAL(triggered()),
            this, SLOT(showProcessorStack()));

    showConsoleAction= new QAction(QIcon(":/icons/monitor.png"),
                                   tr("console"), this);
    showConsoleAction->setStatusTip(tr("console"));
    connect(showConsoleAction, SIGNAL(triggered()),
            this, SLOT(showConsole()));

    showFactoryAction= new QAction(QIcon(":/icons/sitemap.png"),
                                   tr("factory tree"), this);
    showFactoryAction->setCheckable(true);
    showFactoryAction->setChecked(true);
    showFactoryAction->setStatusTip(tr("factory tree"));
    connect(showFactoryAction, SIGNAL(triggered()),
            this, SLOT(showFactory()));

    showProjectAction= new QAction(QIcon(":/icons/sitemap.png"),
                                   tr("project tree"), this);
    showProjectAction->setCheckable(true);
    showProjectAction->setChecked(false);
    showProjectAction->setStatusTip(tr("project tree"));
    connect(showProjectAction, SIGNAL(triggered()),
            this, SLOT(showProject()));

    showPreviewAction= new QAction(QIcon(":/icons/package.png"),
                                   tr("preview"), this);
    showPreviewAction->setStatusTip(tr("preview"));
    connect(showPreviewAction, SIGNAL(triggered()),
            this, SLOT(showPreview()));

    showMultiScaleTreeAction= new QAction(QIcon(":/icons/sitemap.png"),
                                          tr("project tree"), this);
    showMultiScaleTreeAction->setStatusTip(tr("project tree"));
    connect(showMultiScaleTreeAction, SIGNAL(triggered()),
            this, SLOT(showMultiScaleTree()));

    //Help menu's actions

    shareProjectAction= new QAction(QIcon(":/icons/application_add.png"),
                                    tr("share project ..."), this);
    shareProjectAction->setStatusTip(tr("share project ..."));
    connect(shareProjectAction, SIGNAL(triggered()),
            this, SLOT(shareProject()));

    sharePatternAction= new QAction(QIcon(":/icons/plugin.png"),
                                    tr("community ..."), this);
    sharePatternAction->setStatusTip(tr("community ..."));
    connect(sharePatternAction, SIGNAL(triggered()),
            this, SLOT(sharePattern()));

    forumProjectAction= new QAction(QIcon(":/icons/group.png"),
                                    tr("community forum ..."), this);
    forumProjectAction->setStatusTip(tr("community forum ..."));
    connect(forumProjectAction, SIGNAL(triggered()),
            this, SLOT(forum()));

    homeAction= new QAction(QIcon(":/icons/house.png"),
                            tr("community home ..."), this);
    homeAction->setStatusTip(tr("community home ..."));
    connect(homeAction, SIGNAL(triggered()),
            this, SLOT(home()));

    videoAction= new QAction(QIcon(":/icons/film.png"),
                             tr("video tutorial ..."), this);
    videoAction->setStatusTip(tr("video tutorial ..."));
    connect(videoAction, SIGNAL(triggered()),
            this, SLOT(videoTutorial()));

    populationAction= new QAction(QIcon(":/icons/book.png"),
                                  tr("population dictionary documentation ..."), this);
    populationAction->setStatusTip(tr("population dictionary documentation ..."));
    connect(populationAction, SIGNAL(triggered()),
            this, SLOT(populationDocumentation()));

    csdAction= new QAction(QIcon(":/icons/book.png"),
                           tr("csd dictionary documentation ..."), this);
    csdAction->setStatusTip(tr("csd dictionary documentation ..."));
    connect(csdAction, SIGNAL(triggered()),
            this, SLOT(csdDocumentation()));

    webAction= new QAction(QIcon(":/icons/book.png"),
                           tr("custom plot dictionary documentation ..."), this);
    webAction->setStatusTip(tr("custom plot dictionary documentation ..."));
    connect(webAction, SIGNAL(triggered()),
            this, SLOT(webDocumentation()));

    reportABugAction= new QAction(QIcon(":/icons/bug.png"),
                                  tr("report a bug ..."), this);
    reportABugAction->setStatusTip(tr("report a bug ..."));
    connect(reportABugAction, SIGNAL(triggered()),
            this, SLOT(reportABug()));

    documentationAction= new QAction(QIcon(":/icons/book.png"),
                                     tr("documentation ..."), this);
    documentationAction->setStatusTip(tr("documentation ..."));
    connect(documentationAction, SIGNAL(triggered()),
            this, SLOT(documentation()));

    aboutAction= new QAction(QIcon(":/icons/help.png"),
                             tr("about"), this);
    aboutAction->setStatusTip(tr("about"));
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));

    //composition's actions
    clearAction= new QAction(QIcon(":/icons/delete.png"),
                             tr("clear"), this);
    clearAction->setStatusTip(tr("clear"));
    connect(clearAction, SIGNAL(triggered()),
            this, SLOT(clear()));

    upAction = new QAction(QIcon(":/icons/eye.png"),
                           tr("up"), this);
    upAction->setStatusTip(tr("up"));
    connect(upAction, SIGNAL(triggered()),
            this, SLOT(up()));

    createMultiAction = new QAction(QIcon(":/icons/chart_line.png"),
                                    tr("new sub process"), this);
    createMultiAction->setStatusTip(tr("new sub process"));
    connect(createMultiAction, SIGNAL(triggered()),
            this, SLOT(createMulti()));

    //operator's actions
    deleteAction= new QAction(QIcon(":/icons/delete.png"),
                              tr("&delete"), this);
    deleteAction->setShortcut(tr("Delete"));
    deleteAction->setStatusTip(tr("delete"));
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(deletecomponent()));

    copyAction= new QAction(QIcon(":/icons/cut.png"),
                            tr("copy"), this);
    copyAction->setStatusTip(tr("copy"));
    connect(copyAction, SIGNAL(triggered()),
            this, SLOT(copy()));


    cutAction= new QAction(QIcon(":/icons/cut.png"),
                           tr("cut"), this);
    cutAction->setStatusTip(tr("cut"));
    connect(cutAction, SIGNAL(triggered()),
            this, SLOT(cut()));

    pasteAction= new QAction(QIcon(":/icons/cut.png"),
                             tr("paste"), this);
    pasteAction->setStatusTip(tr("paste"));
    connect(pasteAction, SIGNAL(triggered()),
            this, SLOT(paste()));

    createPatternAction= new QAction(QIcon(":/icons/plugin.png"),
                                     tr("save as a pattern"), this);
    createPatternAction->setStatusTip(tr("save as a pattern"));
    connect(createPatternAction, SIGNAL(triggered()),
            this, SLOT(createPattern()));

    exportPatternAction= new QAction(QIcon(":/icons/plugin.png"),
                                     tr("export patterns ..."), this);
    exportPatternAction->setStatusTip(tr("export patterns ..."));
    connect(exportPatternAction, SIGNAL(triggered()),
            this, SLOT(exportPattern()));

    unmergeAction= new QAction(QIcon(":/icons/arrow_out.png"),
                               tr("unmerge"), this);
    unmergeAction->setStatusTip(tr("unmerge"));
    connect(unmergeAction, SIGNAL(triggered()),
            this, SLOT(unmerge()));

    goIntoAction= new QAction(QIcon(":/icons/bringtofront.png"),
                              tr("down"), this);
    goIntoAction->setStatusTip(tr("down"));
    connect(goIntoAction, SIGNAL(triggered()),
            this, SLOT(goInto()));

    showInfos = new QAction(QIcon(":/icons/help.png"),
                            tr("show infos"), this);
    showInfos->setShortcut(tr("W"));
    showInfos->setStatusTip(tr("show infos"));
    connect(showInfos, SIGNAL(triggered()),
            this, SLOT(showInfo()));


    //connector's actions
    disconnectAction= new QAction(QIcon(":/icons/disconnect.png"),
                                  tr("disconnect"), this);
    disconnectAction->setStatusTip(tr("disconnect"));
    connect(disconnectAction, SIGNAL(triggered()),
            this, SLOT(disconnect()));

    disconnectConnectorControlAction = new QAction(QIcon(":/icons/disconnect.png"),
                                                   tr("disconnect"), this);
    disconnectConnectorControlAction->setStatusTip(tr("disconnect"));
    connect(disconnectConnectorControlAction, SIGNAL(triggered()),
            this, SLOT(disconnectComposerControl()));

    disconnectControlAction= new QAction(QIcon(":/icons/disconnect.png"),
                                         tr("disconnect control"), this);
    disconnectControlAction->setStatusTip(tr("disconnect control"));
    connect(disconnectControlAction, SIGNAL(triggered()),
            this, SLOT(disconnectControl()));

    showControlAction= new QAction(QIcon(":/icons/eye.png"),
                                   tr("show control"), this);
    showControlAction->setStatusTip(tr("show control"));
    connect(showControlAction, SIGNAL(triggered()),
            this, SLOT(showControl()));

    disconnectAction= new QAction(QIcon(":/icons/disconnect.png"),
                                  tr("disconnect"), this);
    disconnectAction->setStatusTip(tr("disconnect"));
    connect(disconnectAction, SIGNAL(triggered()),
            this, SLOT(disconnect()));

    addConnectorAction= new QAction(QIcon(":/icons/add.png"),
                                    tr("add connector"), this);
    addConnectorAction->setStatusTip(tr("add connector"));
    connect(addConnectorAction, SIGNAL(triggered()),
            this, SLOT(addConnector()));
    addConnectorAction->setDisabled(true);

    rmvConnectorAction= new QAction(QIcon(":/icons/delete.png"),
                                    tr("remove connector"), this);
    rmvConnectorAction->setStatusTip(tr("remove connector"));
    connect(rmvConnectorAction, SIGNAL(triggered()),
            this, SLOT(rmvConnector()));
    rmvConnectorAction->setDisabled(true);

    //multiselection's actions
    mergeAction= new QAction(QIcon(":/icons/arrow_in.png"),
                             tr("merge into sub process"), this);
    mergeAction->setStatusTip(tr("merge into sub process"));
    connect(mergeAction, SIGNAL(triggered()),
            this, SLOT(merge()));

    //playerBar's actions
    playAction= new QAction(QIcon(":/icons/control_play.png"),
                            tr("play"), this);
    playAction->setStatusTip(tr("play"));
    connect(playAction, SIGNAL(triggered()),
            this, SLOT(play()));

    pauseAction= new QAction(QIcon(":/icons/control_pause.png"),
                             tr("pause"), this);
    pauseAction->setStatusTip(tr("pause"));
    connect(pauseAction, SIGNAL(triggered()),
            this, SLOT(pause()));

    stopAction= new QAction(QIcon(":/icons/control_stop.png"),
                            tr("stop"), this);
    stopAction->setStatusTip(tr("stop"));
    connect(stopAction, SIGNAL(triggered()),
            this, SLOT(stop()));

    nextStepAction= new QAction(QIcon(":/icons/control_end.png"),
                                tr("next step"), this);
    nextStepAction->setStatusTip(tr("next step"));
    connect(nextStepAction, SIGNAL(triggered()),
            this, SLOT(nextStep()));

    //commentBar's actions
    textAction= new QAction(QIcon(":/icons/style.png"),
                            tr("text"), this);
    textAction->setCheckable(true);
    textAction->setChecked(false);
    textAction->setStatusTip(tr("text"));
    connect(textAction, SIGNAL(triggered()),
            this, SLOT(text()));

    shapeAction= new QAction(QIcon(":/icons/shape_square.png"),
                             tr("shape"), this);
    shapeAction->setCheckable(true);
    shapeAction->setStatusTip(tr("shape"));
    connect(shapeAction, SIGNAL(triggered()),
            this, SLOT(shape()));

    boldAction= new QAction(QIcon(":/icons/bold.png"),
                            tr("bold"), this);
    boldAction->setCheckable(true);
    boldAction->setStatusTip(tr("bold"));
    connect(boldAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));

    underlineAction= new QAction(QIcon(":/icons/underline.png"),
                                 tr("underline"), this);
    underlineAction->setCheckable(true);

    underlineAction->setStatusTip(tr("underline"));
    connect(underlineAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));

    italicAction= new QAction(QIcon(":/icons/italic.png"),
                              tr("italic"), this);
    italicAction->setCheckable(true);

    italicAction->setStatusTip(tr("italic"));
    connect(italicAction, SIGNAL(triggered()),
            this, SLOT(handleFontChange()));

    toFrontAction= new QAction(QIcon(":/icons/shape_move_forwards.png"),
                               tr("bring to front"), this);
    toFrontAction->setStatusTip(tr("bring item to front"));
    connect(toFrontAction, SIGNAL(triggered()),
            this, SLOT(toFront()));

    sendBackAction= new QAction(QIcon(":/icons/shape_move_backwards.png"),
                                tr("send to back"), this);
    sendBackAction->setStatusTip(tr("send item to back"));
    connect(sendBackAction, SIGNAL(triggered()),
            this, SLOT(sendBack()));

    //compositionBar's actions
    compositionModeAction= new QAction(QIcon(":/icons/cursor.png"),
                                       tr("composition mode"), this);
    compositionModeAction->setCheckable(true);
    compositionModeAction->setStatusTip(tr("composition mode"));
    connect(compositionModeAction, SIGNAL(triggered()),
            this, SLOT(compositionMode()));

    markerModeAction= new QAction(QIcon(":/icons/target.png"),
                                  tr("mark mode"), this);
    markerModeAction->setCheckable(true);
    markerModeAction->setStatusTip(tr("mark mode"));
    connect(markerModeAction, SIGNAL(triggered()),
            this, SLOT(markerMode()));

    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(false);
    nextStepAction->setEnabled(true);

}

void CGComposer::removePreviousActionList(){
    if(previousActionList != 0){
        operatorMenu->removeAction(previousActionList);
        previousActionList = 0;
    }
}

QMenu* CGComposer::getControlComposerMenu(){
    return controlComposerMenu;
}

void CGComposer::addActionList(QMenu* menu){
    previousActionList = operatorMenu->addMenu(menu);
}


void CGComposer::createScene(){
    CGCompositionManager::getInstance()->init(this);
    setWindowTitle(tr("Caméléon Composer (beta)"));
}

void CGComposer::newComposition(){
    //call project open
    if(this->clear()){
        CGProject::getInstance()->setPath("");
        CGLayout* layout0 = CGLayoutManager::getInstance()->getLayout();
        layout0->setName("pattern");
        layout0->setType("interface");
        layout0->setResolution("1920 x 1080");
        layout0->setIsPopup(false);

        CGLayout* lay = CGLayoutManager::getInstance()->getLayout();
        lay->setName("interface");
        lay->setType("interface");
        lay->setResolution("1920 x 1080");
        lay->setIsPopup(false);
        CGProject::getInstance()->addLayout(lay);
        CGLayoutManager::getInstance()->showLayout(lay->getId());

        saveAsComposition();
    }
}

void CGComposer::saveTmp(){
    //call project save
    QString h = QDateTime::currentDateTime().toString("ddMMyyyyhhmmsszzz");
    QString fileName = CGInstance::getInstance()->getCurrentProjectPath()+"."+h+".bck";
    QString currentp = CGInstance::getInstance()->getCurrentProjectPath();
    if(fileName.compare("")!=0){
        ///SAVE PROCESS
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Loader"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        file.close();
        CGProject::getInstance()->saveAs(fileName, CGCompositionManager::getInstance()->getParentCompositionModel(), CGLayoutManager::getInstance()->getLayoutL());
        CGInstance::getInstance()->setCurrentProjectPath(currentp);
    }
}

void CGComposer::purgeTmp(){
    QFileInfo info(CGInstance::getInstance()->getCurrentProjectPath());
    QDir d = info.absoluteDir();
    QStringList listFilter;
    listFilter << "*.bck";
    listFilter << "*.bck.tmp";

    QDirIterator fileIterator(d.absolutePath(), listFilter ,QDir::AllEntries | QDir::NoDotAndDotDot);

    while(fileIterator.hasNext())
    {
        QFile::remove(fileIterator.next());
    }
}

void CGComposer::saveAsComposition(){
    //call project save
    QString compath = QFileDialog::getSaveFileName(this, tr("Save project"), CGInstance::getInstance()->getProjectPath(), tr("Saved Files (*.cm)"));
    if(compath.compare("")!=0){
        QFile file(compath);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Loader"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(compath)
                                 .arg(file.errorString()));
            return;
        }else{
            CGProject::getInstance()->saveAs(compath, CGCompositionManager::getInstance()->getParentCompositionModel(), CGLayoutManager::getInstance()->getLayoutL());
            CGInstance::getInstance()->addRecentProject(compath);
            loadOpenRecent();
            CGInstance::getInstance()->save();
            CGLayoutManager::getInstance()->getController()->setWindowTitle(tr("Caméléon Controller (beta) %1").arg(compath));;
            setWindowTitle(tr("Caméléon Composer (beta) %1").arg(compath));
        }
    }
    this->purgeTmp();
}

void CGComposer::saveComposition(){
    //call project save
    QString fileName = CGInstance::getInstance()->getCurrentProjectPath();
    if(fileName.compare("")!=0){
        ///SAVE PROCESS
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Loader"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
            return;
        }
        file.close();
        CGProject::getInstance()->saveAs(fileName, CGCompositionManager::getInstance()->getParentCompositionModel(), CGLayoutManager::getInstance()->getLayoutL());
        CGInstance::getInstance()->addRecentProject(fileName);
        loadOpenRecent();
        CGInstance::getInstance()->save();
        setWindowTitle(tr("Caméléon Composer (beta) %1").arg(fileName));
        CGLayoutManager::getInstance()->getController()->setWindowTitle(tr("Caméléon Controller (beta) %1").arg(fileName));;
    }else{
        this->saveAsComposition();
    }
    this->purgeTmp();
}

#include "CPerf.h"
void CGComposer::openComposition(){
    //call project open
    CPerf::getInstance()->probeStart("OPEN COMPOSITION");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open project"), CGInstance::getInstance()->getProjectPath(), tr("Saved Files (*.cm)"));
    if(fileName.compare("")!=0 && fileName.compare(CGProject::getInstance()->getPath()) != 0){
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Loader"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        }else{
            //call project close
            this->stop();
            if(this->clear()){
                CGProject::getInstance()->setPath(fileName);
                if(CGInstance::getInstance()->getMode() == CGInstance::MNG)CGInstance::getInstance()->addRecentProject(fileName);
                CGProject::getInstance()->load();
                loadOpenRecent();
                CGCompositionManager::getInstance()->showSceneFromModel(CGCompositionManager::getInstance()->getCurrentCompositionModel());
                setWindowTitle(tr("Caméléon Composer (beta) %1").arg(fileName));
                CGLayoutManager::getInstance()->getController()->setWindowTitle(tr("Caméléon Controller (beta) %1").arg(fileName));;
                CGInstance::getInstance()->save();
                if(CGProject::getInstance()->getShowController()){
                    CGLayoutManager::getInstance()->getController()->show();
                }else{
                    CGLayoutManager::getInstance()->getController()->hide();
                }
            }
        }
    }
    CPerf::getInstance()->probeEnd("OPEN COMPOSITION");
}

void CGComposer::openCompositionByFile(QString fileName){
    //call project open
    if(fileName.compare("")!=0 && fileName.compare(CGProject::getInstance()->getPath()) != 0){
        QFile file(fileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Loader"),
                                 tr("Cannot read file %1:\n%2.")
                                 .arg(fileName)
                                 .arg(file.errorString()));
        }else{
            //call project close
            this->stop();
            this->forceClear();
            CGProject::getInstance()->setPath(fileName);
            if(CGInstance::getInstance()->getMode() == CGInstance::MNG)CGInstance::getInstance()->addRecentProject(fileName);
            CGProject::getInstance()->load();

            loadOpenRecent();
            CGCompositionManager::getInstance()->showSceneFromModel(CGCompositionManager::getInstance()->getCurrentCompositionModel());
            setWindowTitle(tr("Caméléon Composer (beta) %1").arg(fileName));
            CGLayoutManager::getInstance()->getController()->setWindowTitle(tr("Caméléon Controller (beta) %1").arg(fileName));;
            CGInstance::getInstance()->save();
            CGLayoutManager::getInstance()->getController()->hide();
            CGCompositionManager::getInstance()->getComposer()->hide();
        }
    }
}

void CGComposer::network(){

}

void CGComposer::newComposer(){
}

void CGComposer::newController(){
}

void CGComposer::newProject(){
}

void CGComposer::newLayout(){
}

void CGComposer::build(){
    //call project build
    this->saveComposition();
    QStringList listArg;
    listArg << "-mode=GUI";
    //    QDir::absoluteFilePath("../project/devkit/exporter/exporter.cm");
    listArg << "-cm=../project/devkit/exporter/exporter.cm";
    QProcess::startDetached(qApp->arguments()[0], listArg);
}

void CGComposer::workspaceProperties(){
    properties->load();
    properties->setVisible(true);
}

void CGComposer::compositionProperties(){
    projectProperties->load();
    projectProperties->setVisible(true);
}

void CGComposer::showlog(){
    if(dockMachine->isHidden()){
        dockMachine->show();
    }else{
        dockMachine->hide();
    }
}

void CGComposer::showProcessorStack(){
}

void CGComposer::reportABug(){
    QDesktopServices::openUrl ( QUrl("https://git.framasoft.org/ocds/Cameleon/issues/"));
}

void CGComposer::videoTutorial(){
    QDesktopServices::openUrl ( QUrl("http://www.shinoe.org/cameleon/videos/"));
}

void CGComposer::populationDocumentation(){
    QDesktopServices::openUrl ( QUrl("http://www.population-image.fr"));
}

void CGComposer::documentation(){
    QDesktopServices::openUrl ( QUrl("http://www.shinoe.org/cameleon/documentation.html"));
}

void CGComposer::sharePattern(){
    QDesktopServices::openUrl ( QUrl("https://git.framasoft.org/shinoe/Cameleon"));
}
void CGComposer::shareProject(){
    QDesktopServices::openUrl ( QUrl("http://www.shinoe.org/forum"));
}

void CGComposer::home(){
    QDesktopServices::openUrl ( QUrl("http://www.shinoe.org/cameleon"));
}

void CGComposer::forum(){
    QDesktopServices::openUrl ( QUrl("http://www.shinoe.org/forum"));
}

void CGComposer::webDocumentation(){
    QDesktopServices::openUrl ( QUrl("http://cameleon.shinoe.org"));
}

void CGComposer::csdDocumentation(){
    QDesktopServices::openUrl ( QUrl("http://www.shinoe.org/cameleon"));
}

void CGComposer::about(){
    QFile aboutHTML("../ressource/about.html");

    QString htmlContent = "";
    if (!aboutHTML.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&aboutHTML);
    QString line = in.readLine();
    htmlContent += line;
    while (!line.isNull()) {
        line = in.readLine();
        htmlContent += line;
    }

    QMessageBox::about(this, tr("About Cameleon"),htmlContent);
}


void CGComposer::forceClear(){
    CGCommandManager::getInstance()->startAction("CLEAR");
    this->stop();
    CGClear* clear = new CGClear();
    clear->doCommand();
    CGCommandManager::getInstance()->endAction("CLEAR");
}

bool CGComposer::clear(){
    QMessageBox msgBox;
    msgBox.setText("Your unsaved work will be lost!");
    msgBox.setInformativeText("Do you want to continue?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::Save);
    int ret = msgBox.exec();

    CGProject::getInstance()->emptyTmpFiles();

    bool bret = false;
    if(ret == QMessageBox::Yes){
        bret = true;
    }
    if(bret){
        CGCommandManager::getInstance()->startAction("CLEAR");
        this->stop();
        CGClear* clear = new CGClear();
        clear->doCommand();
        CGProject::getInstance()->getControllerTree()->getItemRessourceTop()->takeChildren();
        CGProject::getInstance()->getComposerTree()->getItemRessourceTop()->takeChildren();
        CGCommandManager::getInstance()->endAction("CLEAR");
    }
    return bret;
}


QDockWidget* CGComposer::getGlossary(){
    return dockFactory;
}

QDockWidget* CGComposer::getProject(){
    return dockProject;
}

QDockWidget* CGComposer::getMachine(){
    return dockMachine;
}

//operator's
void CGComposer::deletecomponent(){
    if(CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().count() > 0){
        this->stop();
        CGCommandManager::getInstance()->startAction("DELETE COMPONENT");
        foreach (QGraphicsItem *item, CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems()) {
            if (item->type() == CGMulti::Type) {
                CGMulti* multi = qgraphicsitem_cast<CGMulti *>(item);
                multi->disconnect();

                CGDeleteOperator* del= new CGDeleteOperator();
                del->setItem(item);
                del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                del->doCommand();
            }else if (item->type() == CGOperator::Type) {
                CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);
                op->disconnect();

                CGDeleteOperator* del= new CGDeleteOperator();
                del->setItem(item);
                del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                del->doCommand();
            }else if(item->type() == CGText::Type || item->type() == CGShape::Type){
                CGDeleteOperator* del= new CGDeleteOperator();
                del->setItem(item);
                del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                del->doCommand();
            }else if(item->type() == CGLayoutItem::Type){
                CGLayoutItem* layout = qgraphicsitem_cast<CGLayoutItem *>(item);
                foreach(CGLayoutItemConnector* connector, layout->getConnectorsL()){
                    if(connector->isConnected()){
                        CGDisconnectConnector* disconnect = new CGDisconnectConnector();
                        disconnect->setControl(layout->getControl());
                        disconnect->setControlConnector(connector);
                        disconnect->setConnector(connector->getConnectedConnector());
                        disconnect->doCommand();
                    }
                }
                CGDeleteControl* del= new CGDeleteControl();
                del->setItem(item);
                del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                del->doCommand();
            }else{
                if(item->type()!=CGConnector::Type
                        &&item->type()!=CGMultiConnector::Type
                        &&item->type()!=CGLayoutItemConnector::Type) CGCompositionManager::getInstance()->getCurrentCompositionModel()->unactivate(item);
            }
        }
        CGCommandManager::getInstance()->endAction("DELETE COMPONENT");
    }
}

//connector's

void CGComposer::disconnectControl(){
    QGraphicsItem* item = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().first();
    CGConnector* connector = 0;
    if(item->type() == CGConnector::Type){
        connector = qgraphicsitem_cast<CGConnector *>(item);
    }else if(item->type() == CGMultiConnector::Type){
        CGMultiConnector* c = qgraphicsitem_cast<CGMultiConnector *>(item);
        if(c->getOtherSide()->isConnected() && c->getOtherSide()->getConnectedConnector()->type() == CGConnector::Type){
            connector = c->getOtherSide()->getConnectedConnector();
        }
    }
    if(connector!=0 && connector->getConnectedControlConnector()!=0){
        CGCommandManager::getInstance()->startAction("DISCONNECT CONTROL");
        CGDisconnectConnector* disconnect = new CGDisconnectConnector();
        CGLayoutItem* layout = qgraphicsitem_cast<CGLayoutItem *>(connector->getConnectedControlConnector()->parentItem());
        disconnect->setControl(layout->getControl());
        disconnect->setControlConnector(connector->getConnectedControlConnector());
        disconnect->setConnector(connector);
        disconnect->doCommand();
        CGCommandManager::getInstance()->endAction("DISCONNECT CONTROL");
    }
}

void CGComposer::showControler(){
    if(CGLayoutManager::getInstance()->getController()->isHidden()){
        CGLayoutManager::getInstance()->getController()->show();
    }else{
        CGLayoutManager::getInstance()->getController()->hide();
    }
}


void CGComposer::showControl(){
    QGraphicsItem* item = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().first();
    CGConnector* connector = qgraphicsitem_cast<CGConnector *>(item);
    if(connector->getIsControlConnected())CGLayoutManager::getInstance()->showConnected(connector->getConnectedControlConnector());
}

void CGComposer::disconnectComposerControl(){
    QList<QGraphicsItem*> items = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems();
    foreach(QGraphicsItem* item, items){
        if(item->type() == CGLayoutItemConnector::Type){
            CGCommandManager::getInstance()->startAction("DISCONNECT");
            CGDisconnectConnector* disconnect = new CGDisconnectConnector();
            CGLayoutItemConnector* connector = qgraphicsitem_cast<CGLayoutItemConnector *>(item);
            if(connector->isConnected()){
                CGLayoutItem* layout = qgraphicsitem_cast<CGLayoutItem *>(connector->parentItem());
                disconnect->setControl(layout->getControl());
                disconnect->setControlConnector(connector);
                disconnect->setConnector(connector->getConnectedConnector());
                disconnect->doCommand();
                CGCommandManager::getInstance()->endAction("DISCONNECT");
            }
        }
    }
}

void CGComposer::disconnect(){
    if(CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().count() > 0)
        CGCommandManager::getInstance()->startAction("DISCONNECT");
    foreach (QGraphicsItem *item, CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems()) {
        CGDisconnectConnector* disconnect = new CGDisconnectConnector();
        if (item->type() == CGConnector::Type) {
            CGConnector* connector = qgraphicsitem_cast<CGConnector *>(item);
            if(connector->isConnected()){
                disconnect->setConnector(connector);
                disconnect->doCommand();
            }
        }
        if (item->type() == CGMultiConnector::Type) {
            CGMultiConnector* connector = qgraphicsitem_cast<CGMultiConnector *>(item);
            if(connector->isConnected()){
                disconnect->setConnector(connector);
                disconnect->doCommand();
            }
        }
    }
    CGCommandManager::getInstance()->endAction("DISCONNECT");
}
void CGComposer::addConnector(){
}

void CGComposer::rmvConnector(){
}

void CGComposer::unmerge(){
    QMap<CGConnector*, CGConnector*> recordConnectedConnector;
    QMap<CGConnector*, CGConnector*> recordControlConnectedConnector;

    if(CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().count() > 0){
        CGCommandManager::getInstance()->startAction("UNMERGE");

        QGraphicsItem* item = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().first();
        QPointF sp = item->scenePos();
        if (item->type() == CGMulti::Type) {
            CGMulti* multiToUnmerge = qgraphicsitem_cast<CGMulti *>(item);
            CGCompositionModel* model = multiToUnmerge->getModel();

            CGCompositionManager::getInstance()->getCurrentComposerScene();

            //record connections

            QList<CGCompositionModel*> modelsList = model->getCompositionModelList();
            QList <CGOperatorModel*> operatorsList = model->getOperatorModelList();
            foreach(CGOperatorModel*ope, operatorsList){
                foreach(CGConnectorModel* connector,ope->getConnectorsList()){
                    CGConnector* vc = ope->getView()->getConnectorById(connector->getId());
                    if(vc->isConnected()
                            && !model->belongToThis(vc->getConnectedConnector())
                            && vc->getConnectedConnector()->type() != CGLayoutItemConnector::Type){
                        CGMultiConnector* mc = qgraphicsitem_cast<CGMultiConnector *>(vc->getConnectedConnector());;
                        CGConnector* toConnect = mc->getParentConnector()->getOuterConnector()->getConnectedConnector();
                        if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
                    }else if(vc->isConnected() && !vc->getIsControlConnected()){
                        CGConnector* toConnect = vc->getConnectedConnector();
                        if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
                    }

                    if(vc->getIsControlConnected()){
                        CGConnector* toConnect = vc->getConnectedControlConnector();
                        //if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
                        if(toConnect!=NULL){
                            recordControlConnectedConnector.insert(vc, toConnect);
                        }
                    }
                }
            }
            foreach(CGCompositionModel*compo, modelsList){
                foreach(CGConnector* vc,compo->getOuterView()->getConnectorsList()){
                    if(vc->isConnected() && !model->belongToThis(vc->getConnectedConnector())){
                        CGMultiConnector* mc = qgraphicsitem_cast<CGMultiConnector *>(vc->getConnectedConnector());;
                        CGConnector* toConnect = mc->getParentConnector()->getOuterConnector()->getConnectedConnector();
                        if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
                    }else if(vc->isConnected()){
                        CGConnector* toConnect = vc->getConnectedConnector();
                        if(toConnect!=NULL) recordConnectedConnector.insert(vc, toConnect);
                    }
                }
            }

            //disconnect
            foreach(CGOperatorModel*ope, operatorsList){
                ope->getView()->disconnect();
            }
            foreach(CGCompositionModel*compo, modelsList){
                compo->getOuterView()->disconnect();
            }
            model->getOuterView()->disconnect();

            //delete
            QList<CGShape*> shapesList = model->getShapeModelList();
            QList <CGText*> textsList = model->getTextModelList();
            QList<QGraphicsItem*> items = model->getScene()->items();
            foreach(QGraphicsItem*item,items){
                if(item->type() == CGLayoutItem::Type){
                    CGDeleteControl* del= new CGDeleteControl();
                    del->setScene(model->getScene());
                    del->setItem(item);
                    del->doCommand();
                }
            }
            foreach(CGOperatorModel*ope, operatorsList){
                CGOperator* op = ope->getView();
                CGDeleteOperator* del= new CGDeleteOperator();
                del->setScene(model->getScene());
                del->setItem(op);
                del->doCommand();
            }
            foreach(CGCompositionModel*compo, modelsList){
                CGMulti* multi = compo->getOuterView();
                CGDeleteOperator* del= new CGDeleteOperator();
                del->setScene(model->getScene());
                del->setMove(true);
                del->setItem(multi);
                del->doCommand();
            }
            foreach(CGShape* shape, shapesList) {
                CGDeleteOperator* del= new CGDeleteOperator();
                del->setScene(model->getScene());
                del->setMove(true);
                del->setItem(shape);
                del->doCommand();
            }

            foreach(CGText* text, textsList){
                CGDeleteOperator* del= new CGDeleteOperator();
                del->setScene(model->getScene());
                del->setMove(true);
                del->setItem(text);
                del->doCommand();

            }

            //create
            QGraphicsItemGroup* group = new QGraphicsItemGroup(0,model->getParentCompositionModel()->getScene());
            foreach(QGraphicsItem*item,items){
                if(item->type() == CGLayoutItem::Type){
                    CGCreateControl* create = new CGCreateControl();
                    create->setType(CGLayoutItem::Type);
                    create->setItem(item);
                    create->setScene(model->getParentCompositionModel()->getScene());
                    create->doCommand();
                }
            }
            foreach(CGOperatorModel*ope, operatorsList){
                CGCreateOperator* create = new CGCreateOperator();
                create->setScene(model->getParentCompositionModel()->getScene());
                create->setType(CGOperator::Type);
                create->setItem(ope->getView());
                create->doCommand();
            }
            foreach(CGCompositionModel*compo, modelsList){
                CGCreateOperator* create = new CGCreateOperator();
                create->setScene(model->getParentCompositionModel()->getScene());
                create->setType(CGMulti::Type);
                create->setItem(compo->getOuterView());
                create->doCommand();
            }
            foreach(CGShape* shape, shapesList) {
                CGCreateOperator* create = new CGCreateOperator();
                create->setScene(model->getParentCompositionModel()->getScene());
                create->setType(CGShape::Type);
                create->setItem(shape);
                create->doCommand();
            }
            foreach(CGText* text, textsList){
                CGCreateOperator* create = new CGCreateOperator();
                create->setScene(model->getParentCompositionModel()->getScene());
                create->setType(CGText::Type);
                create->setItem(text);
                create->doCommand();
            }

            //delete father
            CGDeleteOperator* del= new CGDeleteOperator();
            del->setScene(model->getParentCompositionModel()->getScene());
            del->setItem(model->getOuterView());
            del->doCommand();

            foreach (QGraphicsItem *item, items) {
                if(CGText::Type==item->type()){
                    group->addToGroup(item);
                }
                if(CGShape::Type==item->type()){
                    group->addToGroup(item);
                }
                if(CGMulti::Type==item->type()){
                    if(model->getInnerView()!=item)group->addToGroup(item);
                }
                if(CGOperator::Type==item->type()){
                    group->addToGroup(item);
                }
                if(CGLayoutItem::Type==item->type()){
                    group->addToGroup(item);
                }
            }

            //translation
            group->setPos(sp);
            model->getParentCompositionModel()->getScene()->destroyItemGroup(group);

            //reconnect
            foreach(CGOperatorModel*ope, operatorsList){
                foreach(CGConnector* connector,ope->getView()->getConnectorsList()){
                    if(connector->getConnectedConnector()==NULL && recordConnectedConnector.contains(connector)){
                        if(recordConnectedConnector.value(connector)->type() != CGLayoutItemConnector::Type){
                            CGConnectConnector* connect = new CGConnectConnector();
                            connect->setStartItem(recordConnectedConnector.value(connector));
                            connect->setEndItem(connector);
                            connect->doCommand();
                        }else{
                            CGConnector* connectortoconnect = recordConnectedConnector.value(connector);
                            CGLayoutItemConnector* lconnectortoconnect = qgraphicsitem_cast<CGLayoutItemConnector *>(connectortoconnect);
                            CGLayoutItem* layitem = qgraphicsitem_cast<CGLayoutItem *>(lconnectortoconnect->parentItem());
                            CControl* control = layitem->getControl();

                            CGConnectConnector* connect = new CGConnectConnector();
                            connect->setControl(control);
                            connect->setControlConnector(connectortoconnect);
                            connect->setConnector(connector);
                            connect->doCommand();
                            connectortoconnect->parentItem()->setSelected(false);
                            CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
                        }
                    }
                    if(recordControlConnectedConnector.contains(connector)) {
                        CGConnector* connectortoconnect = recordControlConnectedConnector.value(connector);
                        CGLayoutItemConnector* lconnectortoconnect = qgraphicsitem_cast<CGLayoutItemConnector *>(connectortoconnect);
                        CGLayoutItem* layitem = qgraphicsitem_cast<CGLayoutItem *>(lconnectortoconnect->parentItem());
                        CControl* control = layitem->getControl();

                        CGConnectConnector* connect = new CGConnectConnector();
                        connect->setControl(control);
                        connect->setControlConnector(connectortoconnect);
                        connect->setConnector(connector);
                        connect->doCommand();
                        connectortoconnect->parentItem()->setSelected(false);
                        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
                    }
                }
            }
            foreach(CGCompositionModel*compo, modelsList){
                foreach(CGConnector* connector,compo->getOuterView()->getConnectorsList()){
                    if(connector->getConnectedConnector()==NULL && recordConnectedConnector.contains(connector)){
                        CGConnectConnector* connect = new CGConnectConnector();
                        connect->setStartItem(recordConnectedConnector.value(connector));
                        connect->setEndItem(connector);
                        connect->doCommand();
                    }
                }
            }
        }
        CGCommandManager::getInstance()->endAction("UNMERGE");
    }
    if(CGLayoutManager::getInstance()->getCurrentLayout())CGLayoutManager::getInstance()->getCurrentLayout()->update();
    if(CGCompositionManager::getInstance()->getCurrentComposerScene())CGCompositionManager::getInstance()->getCurrentComposerScene()->update();

}

void CGComposer::merge(){
    //TODO: add dialog with name
    this->stop();
    QMap<CGConnector*, CGConnector*> recordConnectedConnector;
    QMap<CGConnector*, CGConnector*> recordConnectedControlConnector;
    bool ok;
    QString text = QInputDialog::getText(0, QObject::tr("Sub process creation"),
                                         QObject::tr("Sub process name:"), QLineEdit::Normal,
                                         QObject::tr(""), &ok);
    if (ok && !text.isEmpty()){
        CGCommandManager::getInstance()->startAction("MERGE");
        if(CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().count() > 0){

            QList<QGraphicsItem*> items = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems();


            CGCreateOperator* create = new CGCreateOperator();
            create->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
            create->setType(CGMulti::Type);
            create->setOperatorName(text);
            create->setEventPos(CGCompositionManager::getInstance()->getCurrentComposerScene()->getLastSceneRightPressedPos());
            create->doCommand();

            QGraphicsItem* i = create->getItem();
            CGMulti* createdMulti = qgraphicsitem_cast<CGMulti *>(i);

            //disconnect & record connection
            foreach (QGraphicsItem *item, items) {
                if (item->type() == CGMulti::Type) {
                    CGMulti* multi = qgraphicsitem_cast<CGMulti *>(item);
                    foreach(CGConnector* con, multi->getConnectorsList()){
                        if(con->getConnectedConnector() != NULL){
                            recordConnectedConnector.insert(con, con->getConnectedConnector());
                        }
                    }
                    multi->disconnect();

                }else if (item->type() == CGOperator::Type) {
                    CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);
                    foreach(CGConnector* con, op->getConnectorsList()){
                        if(con->getIsControlConnected() && con->getConnectedControlConnector()!=NULL){
                            recordConnectedControlConnector.insert(con, con->getConnectedControlConnector());
                        }
                        if(con->getConnectedConnector() != NULL && con->getConnectedConnector()->type() != CGLayoutItemConnector::Type){
                            recordConnectedConnector.insert(con, con->getConnectedConnector());
                        }
                    }
                    op->disconnect();
                }
            }
            CGCompositionManager::getInstance()->getCurrentComposerScene()->unselectAll();
            foreach (QGraphicsItem *item, items) {
                //delete
                if (item->type() == CGMulti::Type) {
                    CGDeleteOperator* del= new CGDeleteOperator();
                    del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                    del->setItem(item);
                    del->setMove(true);
                    del->doCommand();
                }else if (item->type() == CGOperator::Type) {
                    CGDeleteOperator* del= new CGDeleteOperator();
                    del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                    del->setItem(item);
                    del->doCommand();
                }else if(item->type() == CGText::Type){
                    CGDeleteOperator* del= new CGDeleteOperator();
                    del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                    del->setItem(item);
                    del->doCommand();
                }else if(item->type() == CGShape::Type){
                    CGDeleteOperator* del= new CGDeleteOperator();
                    del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                    del->setItem(item);
                    del->doCommand();
                }else if(item->type() == CGLayoutItem::Type){
                    CGDeleteControl* del= new CGDeleteControl();
                    del->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
                    del->setItem(item);
                    del->doCommand();
                }
            }

            //create
            QGraphicsItemGroup* group = new QGraphicsItemGroup(0,createdMulti->getModel()->getScene());
            foreach (QGraphicsItem *item, items) {
                if (item->type() == CGMulti::Type) {
                    CGMulti* multi = qgraphicsitem_cast<CGMulti *>(item);
                    CGCreateOperator* create2 = new CGCreateOperator();
                    create2->setScene(createdMulti->getModel()->getScene());
                    create2->setType(CGMulti::Type);
                    create2->setItem(multi);
                    create2->doCommand();
                }else if (item->type() == CGOperator::Type) {
                    CGOperator* op = qgraphicsitem_cast<CGOperator *>(item);
                    CGCreateOperator* create2 = new CGCreateOperator();
                    create2->setScene(createdMulti->getModel()->getScene());
                    create2->setType(CGOperator::Type);
                    create2->setItem(op);
                    create2->doCommand();
                }else if(item->type() == CGText::Type){
                    CGCreateOperator* create2 = new CGCreateOperator();
                    create2->setScene(createdMulti->getModel()->getScene());
                    create2->setType(CGText::Type);
                    create2->setItem(item);
                    create2->doCommand();
                }else if(item->type() == CGShape::Type){
                    CGCreateOperator* create2 = new CGCreateOperator();
                    create2->setScene(createdMulti->getModel()->getScene());
                    create2->setType(CGShape::Type);
                    create2->setItem(item);
                    create2->doCommand();
                }else if(item->type() == CGLayoutItem::Type){
                    CGCreateControl* create = new CGCreateControl();
                    create->setType(CGLayoutItem::Type);
                    create->setItem(item);
                    create->setScene(createdMulti->getModel()->getScene());
                    create->doCommand();
                }
            }

            foreach (QGraphicsItem *item, items) {
                if(item->type()!=CGMultiConnector::Type &&item->type()!=CGConnector::Type && item->type()!=CGLayoutItemConnector::Type)group->addToGroup(item);
            }

            QRectF re = group->boundingRect();

            //translation
            group->translate(-re.x()-re.width()/2,-re.y()-re.height()/2);
            createdMulti->getModel()->getScene()->destroyItemGroup(group);
            //reconnect
            foreach(CGOperatorModel* ope, createdMulti->getModel()->getOperatorModelList()){
                foreach(CGConnector* con, ope->getView()->getConnectorsList()){
                    if(recordConnectedConnector.contains(con)){
                        if(!createdMulti->belongToThis(recordConnectedConnector.value(con)) && !con->isConnected()){
                            createdMulti->getModel()->getInnerView()->addConnection(con, recordConnectedConnector.value(con));
                        }else if(!con->isConnected()){
                            CGConnectConnector* connect = new CGConnectConnector();
                            connect->setStartItem(recordConnectedConnector.value(con));
                            connect->setEndItem(con);
                            connect->doCommand();
                        }
                    }
                    if(recordConnectedControlConnector.contains(con)){
                        if(recordConnectedControlConnector.value(con)->type() == CGLayoutItemConnector::Type && !recordConnectedControlConnector.value(con)->isConnected()){
                            CGLayoutItem* layoutConnector = qgraphicsitem_cast<CGLayoutItem*>(recordConnectedControlConnector.value(con)->parentItem());
                            CControl*control = layoutConnector->getControl();
                            CGConnector* connectortoconnect = con;
                            con->setIsControlConnected(true);
                            CGConnectConnector* connect = new CGConnectConnector();
                            connect->setControl(control);
                            connect->setControlConnector(connectortoconnect);
                            connect->setConnector(recordConnectedControlConnector.value(con));
                            connect->doCommand();
                        }
                    }
                }
            }
            foreach(CGCompositionModel* composition, createdMulti->getModel()->getCompositionModelList()){
                foreach(CGConnector* con, composition->getOuterView()->getConnectorsList()){
                    if(recordConnectedConnector.contains(con) && !recordConnectedConnector.value(con)->getIsControlConnected()){
                        if(!createdMulti->belongToThis(recordConnectedConnector.value(con)) && !con->isConnected()){
                            createdMulti->getModel()->getInnerView()->addConnection(con, recordConnectedConnector.value(con));
                        }else if(!con->isConnected()){
                            CGConnectConnector* connect = new CGConnectConnector();
                            connect->setStartItem(recordConnectedConnector.value(con));
                            connect->setEndItem(con);
                            connect->doCommand();
                        }
                    }
                }
            }

            createdMulti->getModel()->getInnerView()->processInnerRay(true);
            createdMulti->getModel()->getSceneView()->centerOn(createdMulti->getParentView()->getInnerView());
        }

        CGCommandManager::getInstance()->endAction("MERGE");
    }
    if(CGLayoutManager::getInstance()->getCurrentLayout())CGLayoutManager::getInstance()->getCurrentLayout()->update();
    if(CGCompositionManager::getInstance()->getCurrentComposerScene())CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
}

void CGComposer::showInfo(){
    QList<QGraphicsItem *> selectedItems;
    if(showingInfos){
        showingInfos = false;
    }else{
        showingInfos = true;
    }

    if(showingInfos) selectedItems= CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems();
    if(!showingInfos) selectedItems= CGCompositionManager::getInstance()->getCurrentComposerScene()->items();
    foreach(QGraphicsItem *item, selectedItems){
        if(item->type() == CGMulti::Type){
            CGMulti* multi = qgraphicsitem_cast<CGMulti *>(item);
            multi->setDoc(showingInfos);
            multi->drawDocs();
        }
        if(item->type() == CGOperator::Type){
            CGOperator* ope = qgraphicsitem_cast<CGOperator *>(item);
            ope->setDoc(showingInfos);
            ope->drawDocs();
        }
    }
}

//playerBar's
void CGComposer::play(){
    isNext = false;
    /*foreach(CGCompositionModel* comp, CGCompositionManager::getInstance()->getCompositionList()){
        comp->getScene()->unselectAll();
        foreach(CGOperatorModel* model, comp->getOperatorModelList()){
            model->getView()->setError("");
            model->getView()->setIsHighlight(false);
            model->getView()->update();
        }
        foreach(QGraphicsItem* item, comp->getScene()->items()){
            if(item->type() == CGLayoutItem::Type){
                CGLayoutItem* ilayout = qgraphicsitem_cast<CGLayoutItem *>(item);
                ilayout->setError("");
                ilayout->update();
            }
        }
        comp->getScene()->update();
    }
    foreach(CGLayout* l, CGLayoutManager::getInstance()->getLayoutL()){
        l->unselectAll();
        foreach(QGraphicsItem* item, l->items()){
            if(item->type() == CGLayoutItem::Type){
                CGLayoutItem* ilayout = qgraphicsitem_cast<CGLayoutItem *>(item);
                ilayout->setError("");
                ilayout->update();
            }
        }
    }
    CGCompositionManager::getInstance()->clearError();*/
    playAction->setEnabled(false);
    pauseAction->setEnabled(true);
    stopAction->setEnabled(true);
    nextStepAction->setEnabled(true);
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->startSend();
}

void CGComposer::pause(){
    isNext = false;
    CLogger::getInstance()->log("STOPPROCESS",CLogger::DEBUG,"CGComposer::stop");

    QTimer::singleShot(200, this, SLOT(timerEnd()));
    isstopped = false;
    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(true);
    nextStepAction->setEnabled(true);
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->pauseSend();
}

void CGComposer::stop(){
    isNext = false;
    CLogger::getInstance()->log("STOPPROCESS",CLogger::DEBUG,"CGComposer::stop");

    QTimer::singleShot(200, this, SLOT(timerEnd()));
    isstopped = false;
    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(false);
    nextStepAction->setEnabled(true);
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->stopSend();
}

void CGComposer::nextStep(){
    isNext = true;
    CLogger::getInstance()->log("STOPPROCESS",CLogger::DEBUG,"CGComposer::stop");
    foreach(CGCompositionModel* comp, CGCompositionManager::getInstance()->getCompositionList()){
        comp->getScene()->unselectAll();
        foreach(CGOperatorModel* model, comp->getOperatorModelList()){
            model->getView()->setError("");
            model->getView()->update();
        }
        comp->getScene()->update();
    }
    foreach(CGLayout* l, CGLayoutManager::getInstance()->getLayoutL()){
        l->unselectAll();
        foreach(QGraphicsItem* item, l->items()){
            if(item->type() == CGLayoutItem::Type){
                CGLayoutItem* ilayout = qgraphicsitem_cast<CGLayoutItem *>(item);
                ilayout->setError("");
                ilayout->update();
            }
        }
    }
    QTimer::singleShot(200, this, SLOT(timerEnd()));
    isstopped = false;
    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(true);
    nextStepAction->setEnabled(true);
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->nextSend();

}

void CGComposer::setIsNext(bool isNext){
    this->isNext = isNext;
}

bool CGComposer::getIsNext(){
    return isNext;
}

bool CGComposer::getIsPaused(){
    return isstopped;
}

void CGComposer::paused(){
    CLogger::getInstance()->log("STOPPROCESS",CLogger::DEBUG,"CGComposer::paused receive");
    isstopped = true;
    dialogThread->hide();
    this->setEnabled(true);
    CGLayoutManager::getInstance()->getController()->setEnabled(true);
    CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
    CGLayoutManager::getInstance()->getCurrentLayout()->update();
}

void CGComposer::stopped(){
    CLogger::getInstance()->log("STOPPROCESS",CLogger::DEBUG,"CGComposer::stopped receive");
    isstopped = true;
    dialogThread->hide();
    this->setEnabled(true);
    CGLayoutManager::getInstance()->getController()->setEnabled(true);
    CGCompositionManager::getInstance()->getCurrentComposerScene()->update();
    CGLayoutManager::getInstance()->getCurrentLayout()->update();
}

void CGComposer::timerEnd(){
    CLogger::getInstance()->log("STOPPROCESS",CLogger::DEBUG,"CGComposer::timerEnd receive");
    if(!isstopped){
        CLogger::getInstance()->log("STOPPROCESS",CLogger::DEBUG,"CGComposer::timerEnd && not stopped receive");
        this->setEnabled(false);
        CGLayoutManager::getInstance()->getController()->setEnabled(false);
        dialogThread->show();
    }
}

//commentBar's
void CGComposer::toFront(){
    if (CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() >= zValue)
            zValue = item->zValue() + 1;
    }
    selectedItem->setZValue(zValue);
}

void CGComposer::sendBack(){
    if (CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->zValue()!=-2000)
            zValue = item->zValue() - 1;
    }
    selectedItem->setZValue(zValue);
}

void CGComposer::currentFontChanged(const QFont &)
{
    handleFontChange();
}

void CGComposer::fontSizeChanged(const QString &)
{
    handleFontChange();
}

void CGComposer::colorChanged(){
    colorAction = qobject_cast<QAction *>(sender());
    colorToolButton->setIcon(createColorToolButtonIcon(qVariantValue<QColor>(colorAction->data())));
    currentColor = qVariantValue<QColor>(colorAction->data());
    buttonTriggered();
}

void CGComposer::buttonTriggered()
{
    CGCompositionManager::getInstance()->getCurrentComposerScene()->setTextColor(currentColor);
    CGCompositionManager::getInstance()->getCurrentComposerScene()->setItemColor(currentColor);
}

void CGComposer::handleFontChange(){
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());

    CGCompositionManager::getInstance()->getCurrentComposerScene()->setFont(font);
}

//compositionBar's
void CGComposer::text(){
    if(textAction->isChecked()){
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::InsertText);
        markerModeAction->setChecked(false);
        shapeAction->setChecked(false);
        compositionModeAction->setChecked(false);
    }else{
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
        compositionModeAction->setChecked(true);
    }
}

void CGComposer::shape(){
    if(shapeAction->isChecked()){
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::InsertShape);
        markerModeAction->setChecked(false);
        textAction->setChecked(false);
        compositionModeAction->setChecked(false);
    }else{
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
        compositionModeAction->setChecked(true);
    }
}

void CGComposer::markerMode(){
    if(markerModeAction->isChecked()){
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Target);
        shapeAction->setChecked(false);
        textAction->setChecked(false);
        compositionModeAction->setChecked(false);
    }else{
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
        compositionModeAction->setChecked(true);
    }
}

void CGComposer::compositionMode(){
    if(compositionModeAction->isChecked()){
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
        markerModeAction->setChecked(false);
        textAction->setChecked(false);
        shapeAction->setChecked(false);
    }else{
        CGCompositionManager::getInstance()->getCurrentComposerScene()->setMode(CGComposition::Composition);
        compositionModeAction->setChecked(true);
    }
}

QMenu *CGComposer::createColorMenu(const char *slot, QColor defaultColor)
{
    QList<QColor> colors;
    colors << Qt::black
           << Qt::white
           << Qt::red
           << Qt::darkRed
           << Qt::green
           << Qt::darkGreen
           << Qt::blue
           << Qt::darkBlue
           << Qt::cyan
           << Qt::darkCyan
           << Qt::magenta
           << Qt::darkMagenta
           << Qt::yellow
           << Qt::darkYellow
           << Qt::gray
           << Qt::darkGray
           << Qt::lightGray;
    QStringList names;

    names <<tr("black")
         <<tr("white")
        <<tr("red")
       <<tr("darkRed")
      <<tr("green")
     <<tr("darkGreen")
    <<tr("blue")
    <<tr("darkBlue")
    <<tr("cyan")
    <<tr("darkCyan")
    <<tr("magenta")
    <<tr("darkMagenta")
    <<tr("yellow")
    <<tr("darkYellow")
    <<tr("gray")
    <<tr("darkGray")
    <<tr("lightGray");

    QMenu *colorMenu = new QMenu(this);
    for (int i = 0; i < colors.count(); ++i) {
        QAction *action = new QAction(names.at(i), this);
        action->setData(colors.at(i));
        action->setIcon(createColorIcon(colors.at(i)));
        connect(action, SIGNAL(triggered()),
                this, slot);
        colorMenu->addAction(action);
        if (colors.at(i) == defaultColor) {
            colorMenu->setDefaultAction(action);
        }
    }
    return colorMenu;
}

QIcon CGComposer::createColorToolButtonIcon(QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, 50, 80), color);
    return QIcon(pixmap);
}

QIcon CGComposer::createColorIcon(QColor color)
{
    QPixmap pixmap(40, 40);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 40, 40), color);

    return QIcon(pixmap);
}


QMenu* CGComposer::getCommentShapeMenu(){
    return commentShapeMenu;
}

QMenu* CGComposer::getCommentTextMenu(){
    return commentTextMenu;
}

QMenu* CGComposer::getOperatorMenu(){
    return operatorMenu;
}

QMenu* CGComposer::getControlConnectorMenu(){
    return connectorControlMenu;
}

QMenu* CGComposer::getConnectorMenu(){
    return connectorMenu;
}

QMenu* CGComposer::getMultiMenu(){
    return multiMenu;
}
QMenu* CGComposer::getCompositionMenu(){
    return compositionMenu;
}

QMenu* CGComposer::getOperatorMultiMenu(){
    return this->operatorMultiMenu;
}

void CGComposer::goInto(){

}

CGCopyPaste* CGComposer::getCopyPaste(){
    return this->copyPast;
}

void CGComposer::setCopyPaste(CGCopyPaste* copyPaste){
    this->copyPast = copyPaste;
}

void CGComposer::copy(){
    copyPast = new CGCopyPaste();
    copyPast->copy(CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems());
}

void CGComposer::cut(){
    copyPast = new CGCopyPaste();
    copyPast->copy(CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems());
    this->deletecomponent();
}

void CGComposer::paste(){
    if(copyPast!=NULL) copyPast->paste(CGCompositionManager::getInstance()->getCurrentComposerScene(), CGCompositionManager::getInstance()->getCurrentComposerScene()->getLastSceneLeftPressedPos());
}

void CGComposer::showConsole(){

}

void CGComposer::showFactory(){
    if(!this->dockFactory->isHidden()){
        this->dockFactory->hide();
        CGLayoutManager::getInstance()->getController()->getGlossary()->hide();
    }else{
        this->dockFactory->show();
        CGLayoutManager::getInstance()->getController()->getGlossary()->show();
    }
}

void CGComposer::showProject(){
    if(this->dockProject->isHidden()){
        this->dockProject->show();
        CGLayoutManager::getInstance()->getController()->getProject()->show();
    }else{
        this->dockProject->hide();
        CGLayoutManager::getInstance()->getController()->getProject()->hide();
    }
}


void CGComposer::up(){
    CGCompositionManager::getInstance()->up();
}


void CGComposer::multiProperties(){
    if (CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().first();
    CGMulti* multi = qgraphicsitem_cast<CGMulti *>(selectedItem);
    CGCompositionProperties* compProperties = new CGCompositionProperties();

    compProperties->load(multi->getModel());
    compProperties->setVisible(true);
}

void CGComposer::createPattern(){
    //call compositionModel createPattern

    if (CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = CGCompositionManager::getInstance()->getCurrentComposerScene()->selectedItems().first();
    CGMulti* multi = qgraphicsitem_cast<CGMulti *>(selectedItem);


    //    bool ok;
    //    QString text = QInputDialog::getText(0, QObject::tr("QInputDialog::getText()"),
    //                                         QObject::tr("pattern name:"), QLineEdit::Normal,
    //                                         QObject::tr(""), &ok);

    patterndialog dialog;
    dialog.exec();
    QString text = dialog.getName();
    bool ok = dialog.ret();
    bool isInterface = false;//dialog.isInterface();
    //pattern check
    bool check = true;
    foreach(CGOperatorModel* ope, CGCompositionManager::getInstance()->getOperatorList(multi->getModel())){
        foreach(CGConnector* connector, ope->getView()->getConnectorsList()){
            if(check == true){
                if(connector->getIsControlConnected()){
                    QGraphicsItem* item = connector->getConnectedControlConnector()->parentItem();
                    CGLayoutItem *ctrl = qgraphicsitem_cast<CGLayoutItem *>(item);
                    if(ctrl->getLayout()!=0 && ctrl->getLayout()->getId() != 0){
                        check = false;
                        QMessageBox msgBox;
                        msgBox.setText("Pattern creation error!");
                        msgBox.setInformativeText("Pattern's controls must be in patterns interface!");
                        msgBox.setStandardButtons(QMessageBox::Ok);
                        msgBox.exec();
                    }else if(ctrl->getLayout()==0){
                        CGComposition* compo = dynamic_cast<CGComposition*>(ctrl->scene());
                        check = false;
                        if(multi->getModel()->getScene() == compo){
                            check = true;
                        }
                        QList<CGCompositionModel*> l = CGCompositionManager::getInstance()->getCompositionList(multi->getModel());
                        foreach(CGCompositionModel* c, l){
                            if(compo == c->getScene()){
                                check = true;
                            }
                        }
                        if(!check){
                            QMessageBox msgBox;
                            msgBox.setText("Pattern creation error!");
                            msgBox.setInformativeText("Pattern's controls must be in pattern's compositions !");
                            msgBox.setStandardButtons(QMessageBox::Ok);
                            msgBox.exec();
                        }

                    }
                }
            }
        }
    }
    if (check && ok && !text.isEmpty()){

    }

    if (check && ok && !text.isEmpty()){
        QString compath = CGInstance::getInstance()->getPatternPath()+"/"+text+".pa";
        QFile file(compath);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, tr("Loader"),
                                 tr("Cannot write file %1:\n%2.")
                                 .arg(compath)
                                 .arg(file.errorString()));
            return;
        }else{
            QList<CGLayout*> ll;
            CGProject::getInstance()->saveAs(compath, multi->getModel(), ll,true, isInterface);
            CGInstance::getInstance()->addPattern(text);
            CGInstance::getInstance()->save();
        }
    }
}

void CGComposer::exportPattern(){
    //call pattern save
}

void CGComposer::createMulti(){
    bool ok;
    QString text = QInputDialog::getText(0, QObject::tr("Sub process creation"),

                                         QObject::tr("Sub process name:"), QLineEdit::Normal,
                                         QObject::tr(""), &ok);
    if (ok && !text.isEmpty()){
        CGCommandManager::getInstance()->startAction("EMPTY MERGE");
        CGCreateOperator* create = new CGCreateOperator();
        create->setScene(CGCompositionManager::getInstance()->getCurrentComposerScene());
        create->setType(CGMulti::Type);
        create->setOperatorName(text);
        create->setEventPos(CGCompositionManager::getInstance()->getCurrentComposerScene()->getLastSceneRightPressedPos());
        create->doCommand();
        CGCommandManager::getInstance()->endAction("EMPTY MERGE");
    }
}

void CGComposer::showPreview(){
    this->saveComposition();
    QStringList listArg;
    listArg << "-mode=GUI";
    listArg << "-cm="+CGProject::getInstance()->getPath();
    QProcess::startDetached(qApp->arguments()[0], listArg);
}

void CGComposer::showMultiScaleTree(){

}

patterndialog::patterndialog(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel(tr("Pattern name"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    interfaceBox = new QCheckBox(tr("New interface layout at pattern creation?"));

    okButton = new QPushButton(tr("&Ok"));
    okButton->setCheckable(true);
    okButton->setChecked(false);

    cancelButton = new QPushButton(tr("&Cancel"));

    buttonBox = new QDialogButtonBox(Qt::Horizontal);
    buttonBox->addButton(okButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(cancelButton, QDialogButtonBox::ActionRole);

    connect(okButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *topLeftLayout = new QHBoxLayout;
    topLeftLayout->addWidget(label);
    topLeftLayout->addWidget(lineEdit);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addLayout(topLeftLayout);
    //     leftLayout->addWidget(interfaceBox);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->addWidget(buttonBox, 1, 0);
    mainLayout->setRowStretch(2, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("Pattern creation"));
}

bool patterndialog::isInterface(){
    return interfaceBox->isChecked();
}

QString patterndialog::getName(){
    return lineEdit->text();
}

bool patterndialog::ret(){
    return okButton->isChecked();
}

