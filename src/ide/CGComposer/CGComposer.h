/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "CGComposition.h"
#include <stdlib.h>

#include "CGCopyPaste.h"
#include "CGInstanceProperties.h"
#include "CGProjectProperties.h"
#include "CGThreadInProgressDialog.h"

class CGComposer : public QMainWindow
{
    Q_OBJECT

public:
    CGComposer(QWidget *parent = 0);
    ~CGComposer();

    void openCompositionByFile(QString fileName);
    void forceClear();
public slots:
    void sharePattern();
    void shareProject();
    void home();
    void forum();
    void videoTutorial();
    void populationDocumentation();
    void webDocumentation();
    void csdDocumentation();
    void enterSearch();
    void searchOperator();
    void openRecent(QAction * action);
    void openComposition();
    void newComposition();
    void newProject();
    void newLayout();
    void saveComposition();
    void saveAsComposition();
    void network();
    void newComposer();
    void newController();
    void build();
    void workspaceProperties();
    void compositionProperties();
    void showlog();
    void showProcessorStack();
    void showConsole();
    void showFactory();
    void showProject();
    void showInfo();
    void showPreview();
    void showMultiScaleTree();
    void reportABug();
    void documentation();
    void about();
    bool clear();
    void stopped();
    void paused();
    void timerEnd();
    void restart();
    void showControler();
    //operator's
    void deletecomponent();
    void copy();
    void cut();
    void paste();
    void goInto();
    void unmerge();
    void saveTmp();
    //connector's
    void disconnect();
    void disconnectControl();
    void disconnectComposerControl();
    void showControl();
    void addConnector();
    void rmvConnector();

    //multiselection's
    void up();
    void merge();
    void createMulti();
    void createPattern();
    void exportPattern();
    void multiProperties();

    //playerBar's
    void play();
    void pause();
    void stop();
    void nextStep();

    //commentBar's
    void text();
    void shape();
    void toFront();
    void sendBack();
    void currentFontChanged(const QFont &font);
    void fontSizeChanged(const QString &size);
    void colorChanged();
    void buttonTriggered();
    void handleFontChange();
    QMenu* getCommentShapeMenu();
    QMenu* getCommentTextMenu();
    QMenu* getOperatorMenu();
    QMenu* getConnectorMenu();
    QMenu* getControlConnectorMenu();
    QMenu* getMultiMenu();
    QMenu* getCompositionMenu();
    QMenu* getOperatorMultiMenu();
    QMenu* getControlComposerMenu();

    //compositionBar's
    void compositionMode();
    void markerMode();
    CGCopyPaste* getCopyPaste();
    void setCopyPaste(CGCopyPaste* copyPaste);
    void loadOpenRecent();
    QDockWidget* getGlossary();
    QDockWidget* getProject();
    QDockWidget* getMachine();

    void removePreviousActionList();
    void addActionList(QMenu* menu);
    void purgeTmp();
    QTabWidget* getMachineTab();
    bool getIsNext();
    void setIsNext(bool isNext);
    bool getIsPaused();
    void createFactory();
private:
    bool isNext;
    QTabWidget* tab;
    QMenu* controlComposerMenu;
    QAction* previousActionList;
    //composer construct method
    void init();
    void loadWorkspace();
    void createMenus();
    void createActions();
    void createPlayerBar();
    void createCommentBar();
    void createScene();
    void createMachineDock();
    void closeEvent(QCloseEvent *);
    bool alreadyClosed;
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(QColor color);
    QIcon createColorIcon(QColor color);

    //top's menus
    QMenu *fileMenu;
    QMenu *toolMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QMenu *documentationMenu;
    QMenu *openRecentMenu;

    //item's menus
    QMenu *commentShapeMenu;
    QMenu *commentTextMenu;
    QMenu *operatorMenu;
    QMenu *connectorMenu;
    QMenu *connectorControlMenu;
    QMenu *multiMenu;
    QMenu *compositionMenu;
    QMenu *operatorMultiMenu;

    //File menu's actions
    QAction *openCompositionAction;
    QAction *newProjectAction;
    QAction *newLayoutAction;
    QAction *newCompositionAction;
    QAction *saveCompositionAction;
    QAction *saveasCompositionAction;
    QAction *newWorkspaceAction;
    QAction *switchWorkspaceAction;
    QAction *exitAction;

    //Tools menu's actions
    QAction *networkAction;
    QAction *newComposerAction;
    QAction *newControllerAction;
    QAction *buildAction;
    QAction *workspacePropertiesAction;
    QAction *compositionPropertiesAction;
    QAction *showLogAction;
    QAction *showProcessorStackAction;
    QAction *showConsoleAction;
    QAction *showFactoryAction;
    QAction *showProjectAction;
    QAction *showPreviewAction;
    QAction *showMultiScaleTreeAction;
    QAction* restartAction;
    QAction* showController;

    //Help menu's actions
    QAction *reportABugAction;
    QAction *documentationAction;
    QAction *aboutAction;

    //composition's actions
    QAction *upAction;
    QAction* showInfos;
    QAction *clearAction;
    QAction *createMultiAction;

    //operator's actions
    QAction *deleteAction;
    QAction *copyAction;
    QAction *cutAction;
    QAction *pasteAction;

    //operatormulti's actions
    QAction *unmergeAction;
    QAction *goIntoAction;
    QAction *createPatternAction;
    QAction *exportPatternAction;

    //connector's actions
    QAction *disconnectAction;
    QAction *disconnectControlAction;
    QAction *showControlAction;
    QAction *addConnectorAction;
    QAction *rmvConnectorAction;
    QAction *disconnectConnectorControlAction;
    //multiselection's actions
    QAction *mergeAction;
    QAction *multiPropertiesAction;
    //playerBar's actions
    QAction *playAction;
    QAction *pauseAction;
    QAction *stopAction;
    QAction *nextStepAction;

    //commentBar's actions
    QAction *textAction;
    QAction *colorAction;
    QAction *shapeAction;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *installLibAction;
    QAction *installPatternAction;
    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;

    QComboBox *sceneScaleCombo;
    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    QToolBox *toolBox;
    QButtonGroup *buttonGroup;
    QButtonGroup *pointerTypeGroup;
    QButtonGroup *backgroundButtonGroup;
    QToolButton *colorToolButton;

    //compositionBar's actions
    QAction* compositionModeAction;
    QAction* markerModeAction;

    QDockWidget* dockFactory;
    QDockWidget* dockMachine;
    QDockWidget* dockProject;

    //multiscale's actions
    QToolBar *bar;

    CGCopyPaste* copyPast;

    QTextEdit* editLogger;

    CGInstanceProperties* properties;
    CGProjectProperties* projectProperties;

    QTextEdit* edit;
    bool hasBeenEdited;

    QAction *sharePatternAction;
    QAction *shareProjectAction;
    QAction *forumProjectAction;
    QAction *homeAction;
    QAction *videoAction;
    QAction *populationAction;
    QAction *csdAction;
    QAction *webAction;
    QColor currentColor;

    bool showingInfos;
    bool isstopped;

    CGThreadInProgressDialog* dialogThread;

};

#include <QDialog>

 class QCheckBox;
 class QDialogButtonBox;
 class QGroupBox;
 class QLabel;
 class QLineEdit;
 class QPushButton;

 class patterndialog : public QDialog
 {
     Q_OBJECT

 public:
     patterndialog(QWidget *parent = 0);

     bool isInterface();
     QString getName();
     bool ret();
 private:
     QLabel *label;
     QLineEdit *lineEdit;
     QCheckBox *interfaceBox;
     QDialogButtonBox *buttonBox;
     QPushButton *okButton;
     QPushButton *cancelButton;
     QWidget *extension;
 };

#endif // MAINWINDOW_H
