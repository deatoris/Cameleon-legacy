/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGCONTROLLER_H
#define CGCONTROLLER_H
#include <QtGui>
class CGController;
class CGConnector;
using namespace std;
class CGController : public QMainWindow
{
    Q_OBJECT

public:
    CGController(QWidget *parent = 0);
    ~CGController();
    void setStandaloneMode(bool standalone);
    bool getStandaloneMode();

    QMenu* getConnectorMenu();
    QMenu* getControlMenu();
    QMenu* getControllerMenu();

signals:
    void connectConnector();

public slots:
    void initview();
    void controllerError(int id, string message);
    void disconnectAll();
    void disconnect();
    void deleteItem();
    void showConnector();
    void clear();
    void newLayout();
    void moveToLayout();
    void makePersistent();
    void group();
    void changeResolution(QString s);
    void setResolution(QString s);
    void popupClick();
    void interactiveClick();
    void setIsPopup(bool pop);

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

    QDockWidget* getGlossary();
    QDockWidget* getProject();

    void compositionMode();

    void attachFactory();
private:
    bool isStandalone;
    void closeEvent(QCloseEvent *event);
    bool alreadyClosed;
    //composer construct method
    void init();
    void attachProject();
    void attachLayout();
    void attachMenu();
    void attachAction();
    void attachBar();

    //control menu
    QMenu* controlMenu;
    QAction* disconnectAllAction;
    QAction* deleteAction;

    //connector menu
    QMenu* connectorMenu;
    QAction* disconnectAction;
    QAction* showAction;

    //controller menu
    QMenu* controllerMenu;
    QAction* clearAction;
    QAction* newLayoutAction;

    //bars
    QToolBar *bar;
    QToolBar *textToolBar;
    QToolBar *editToolBar;
    QToolBar *colorToolBar;
    QToolBar *pointerToolbar;

    QComboBox *itemColorCombo;
    QComboBox *textColorCombo;
    QComboBox *fontSizeCombo;
    QFontComboBox *fontCombo;

    //playerBar's actions
    QAction *playAction;
    QAction *pauseAction;
    QAction *stopAction;
    QAction *nextStepAction;
    QAction *moveToLayoutAction;
    QAction *makePersistentAction;
    QAction *makeInteractiveAction;
    QAction *groupAction;

    //commentBar's actions
    QAction *colorAction;
    QAction *compositionAction;
    QAction *textAction;
    QAction *shapeAction;
    QAction *boldAction;
    QAction *underlineAction;
    QAction *italicAction;
    QAction *toFrontAction;
    QAction *sendBackAction;
    QAction *installLibAction;
    QAction *installPatternAction;
    QToolButton *colorToolButton;
    QDockWidget* dockProject;
    QDockWidget* dock;
    QMenu *createColorMenu(const char *slot, QColor defaultColor);
    QIcon createColorToolButtonIcon(QColor color);
    QIcon createColorIcon(QColor color);
    QComboBox* resolution;
    QCheckBox* popupBox;
    QCheckBox* interactiveBox;
};

#include <QDialog>

class QCheckBox;
class QDialogButtonBox;
class QGroupBox;
class QLabel;
class QLineEdit;
class QPushButton;

class layoutdialog : public QDialog
{
    Q_OBJECT

public:
    layoutdialog(QWidget *parent = 0);

    QString getType();
    QString getName();
    bool ret();
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QComboBox *typeBox;
    QDialogButtonBox *buttonBox;
    QPushButton *okButton;
    QPushButton *cancelButton;
    QWidget *extension;
};
#endif // CGCONTROLLER_H
