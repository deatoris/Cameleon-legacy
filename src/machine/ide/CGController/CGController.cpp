/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGController.h"
#include "../CGGlossary/CGGlossary.h"
#include "CGLayoutManager.h"
#include "CGCompositionManager.h"
#include "CGLayoutFactory.h"
#include "../CGCommandManager.h"
#include "../CGCommand/CGDeleteControl.h"
#include "../CGCommand/CGDisconnectConnector.h"
#include "../CGCommand/CGCreateLayout.h"
#include "CGProject.h"
#include<CClient.h>
#include "BackgroundControl.h"
CGController::CGController(QWidget *parent)
    : QMainWindow(parent)
{
//    this->attachFactory();
    this->attachProject();
    this->attachLayout();
    this->attachAction();
    this->attachMenu();
    this->attachBar();
    alreadyClosed = false;
    this->setWindowIcon(QIcon(":/icons/clogo.png"));
    this->isStandalone = false;
}

bool CGController::getStandaloneMode(){
    return isStandalone;
}

void CGController::setStandaloneMode(bool standalone){
    this->isStandalone = standalone;
    if(standalone){
        this->dockProject->hide();
        this->dock->hide();
        bar->hide();
        textToolBar->hide();

        CGLayoutItem* bg = 0;

        //windows Size from background if exist (getFirstToCom)

        QList<CGLayoutItem*> list = CGCompositionManager::getInstance()->searchLayoutItemByType(QString::fromStdString(BackgroundControl::KEY));
        if(list.size()>0) bg = list[0];

        //center on backgound center
        if(bg!=0){
            QSize size  = qApp->desktop()->screenGeometry().size();
            this->setGeometry(size.width()/2-bg->getRect().width()/2,size.height()/2-bg->getRect().height()/2,bg->getRect().width()-40,bg->getRect().height()-40);
            this->setFixedSize(bg->getRect().width()-40,bg->getRect().height()-40);
            QList<CGLayout*> lL = CGLayoutManager::getInstance()->getLayoutL();
            foreach(CGLayout* l, lL){
                l->getView()->centerOn(bg);
            }
        }

        QList<CGLayout*> lL = CGLayoutManager::getInstance()->getLayoutL();
        foreach(CGLayout* l, lL){
            foreach(QGraphicsItem* item, l->items()){
                item->setFlag(QGraphicsItem::ItemIsMovable, false);
                item->setFlag(QGraphicsItem::ItemIsSelectable, false);
                item->setFlag(QGraphicsItem::ItemSendsGeometryChanges, false);

                if(item->type() == CGText::Type){
                    CGText* t = qgraphicsitem_cast<CGText *>(item);
                    t->setEnabled(false);
                }
            }
        }
    }
}

CGController::~CGController(){

}

void CGController::init(){

}
QMenu *CGController::createColorMenu(const char *slot, QColor defaultColor)
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

QIcon CGController::createColorToolButtonIcon(QColor color)
{
    QPixmap pixmap(50, 80);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.fillRect(QRect(0, 0, 50, 80), color);
    return QIcon(pixmap);
}

void CGController::initview(){
    QList<CGLayout*> layL = CGLayoutManager::getInstance()->getLayoutL();
    foreach(CGLayout*lay,layL){
        lay->getView()->renderScale(-lay->getView()->getScaleNum());
    }
}

void CGController::controllerError(int id, string message){
    CGLayoutItem* i = CGLayoutManager::getInstance()->getItemById(id);
    i->setError(message.c_str());
    CGCompositionManager::getInstance()->addCtlError(id, message.c_str());
}

QIcon CGController::createColorIcon(QColor color)
{
    QPixmap pixmap(40, 40);
    QPainter painter(&pixmap);
    painter.setPen(Qt::NoPen);
    painter.fillRect(QRect(0, 0, 40, 40), color);

    return QIcon(pixmap);
}

void CGController::attachBar(){
    bar = new QToolBar("Player bar", this);
    //    bar->setGeometry(bar->geometry().x(),bar->geometry().y(),bar->geometry().width(),bar->geometry().height()-10);
    //    bar->addAction(playAction);
    //    bar->addAction(pauseAction);
    //    bar->addAction(stopAction);
    //    bar->addAction(nextStepAction);
    //    bar->addSeparator();

    addToolBar(Qt::TopToolBarArea, bar);
    bar->addAction(compositionAction);
    bar->addAction(shapeAction);
    bar->addAction(textAction);

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
            this, SLOT(colorChanged()));

    textToolBar = addToolBar(tr("Font"));

    popupBox = new QCheckBox(tr("dialog"));
    connect(popupBox, SIGNAL(clicked()),
            this, SLOT(popupClick()));

    interactiveBox = new QCheckBox(tr("interactive"));
    connect(interactiveBox, SIGNAL(clicked()),
            this, SLOT(interactiveClick()));

    resolution = new QComboBox();
    foreach(QString res, CGLayoutManager::getInstance()->getManagedResolutions()){
        resolution->addItem(res);
    }
    connect(resolution, SIGNAL(currentIndexChanged(QString)),
            this, SLOT(changeResolution(QString)));
    //    resolution->addItem("800 x 600");
    //    resolution->addItem("1024 x 768");
    //    resolution->addItem("1280 x 1024");
    //    resolution->addItem("1920 x 1080");

    QPushButton* initscreen = new QPushButton("focus");

    connect(initscreen, SIGNAL(clicked()),
            this, SLOT(initview()));

    textToolBar->addAction(toFrontAction);
    textToolBar->addAction(sendBackAction);
    textToolBar->addSeparator();
    textToolBar->addWidget(fontCombo);
    textToolBar->addWidget(fontSizeCombo);
    textToolBar->addAction(boldAction);
    textToolBar->addAction(italicAction);
    textToolBar->addAction(underlineAction);
    textToolBar->addWidget(colorToolButton);
    textToolBar->addSeparator();
//    textToolBar->addSeparator();
    textToolBar->addWidget(initscreen);
//    textToolBar->addSeparator();
    //    textToolBar->addWidget(popupBox);
    //    textToolBar->addWidget(interactiveBox);
    addToolBar(Qt::TopToolBarArea, textToolBar);
}

void CGController::popupClick(){
    CGLayoutManager::getInstance()->getCurrentLayout()->setIsPopup(popupBox->isChecked());
}

void CGController::interactiveClick(){

}


void CGController::setIsPopup(bool pop){
    this->popupBox->setChecked(pop);
}

void CGController::setResolution(QString s){
    int index = resolution->findText(s);
    resolution->setCurrentIndex(index);
}

void CGController::changeResolution(QString s){
    CGLayoutManager::getInstance()->getCurrentLayout()->setResolution(s);
}


void CGController::attachFactory(){

    dock = CGGlossary::getInstance()->getWidgetCopy();
    addDockWidget(Qt::LeftDockWidgetArea, dock);
    dockProject = CGProject::getInstance()->getWidgetCopy();

    addDockWidget(Qt::RightDockWidgetArea, dockProject);
    dockProject->hide();

    //    this->tabifyDockWidget(dockProject,dock);

}

void CGController::attachProject(){
}

QMenu* CGController::getConnectorMenu(){
    return connectorMenu;
}

QMenu* CGController::getControlMenu(){
    return controlMenu;
}

QMenu* CGController::getControllerMenu(){
    return controllerMenu;
}


void CGController::attachLayout(){
    CGLayoutManager::getInstance()->init(this);
    setWindowTitle(tr("Cameleon Controller (beta)"));
}

void CGController::closeEvent(QCloseEvent *)
{
    if(!alreadyClosed){
        //        CGCompositionManager::getInstance()->getComposer()->close();
        QMainWindow::close();
    }
    alreadyClosed = true;
}

void CGController::attachAction(){
    disconnectAllAction= new QAction(QIcon(":/icons/disconnect.png"),
                                     tr("disconnect all"), this);
    disconnectAllAction->setStatusTip(tr("disconnect all"));
    connect(disconnectAllAction, SIGNAL(triggered()),
            this, SLOT(disconnectAll()));

    deleteAction= new QAction(QIcon(":/icons/delete.png"),
                              tr("delete"), this);
    deleteAction->setStatusTip(tr("delete"));
    connect(deleteAction, SIGNAL(triggered()),
            this, SLOT(deleteItem()));

    //connector menu
    disconnectAction= new QAction(QIcon(":/icons/disconnect.png"),
                                  tr("disconnect"), this);
    disconnectAction->setStatusTip(tr("disconnect"));
    connect(disconnectAction, SIGNAL(triggered()),
            this, SLOT(disconnect()));

    showAction= new QAction(QIcon(":/icons/eye.png"),
                            tr("show connected operator"), this);
    showAction->setStatusTip(tr("show operator"));
    connect(showAction, SIGNAL(triggered()),
            this, SLOT(showConnector()));

    //controller menu
    newLayoutAction= new QAction(QIcon(":/icons/page_save.png"),
                                 tr("new layout"), this);
    newLayoutAction->setStatusTip(tr("new layout"));
    connect(newLayoutAction, SIGNAL(triggered()),
            this, SLOT(newLayout()));

    clearAction= new QAction(QIcon(":/icons/delete.png"),
                             tr("clear"), this);
    clearAction->setStatusTip(tr("clear"));
    connect(clearAction, SIGNAL(triggered()),
            this, SLOT(clear()));

    //player bar actions
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

    compositionAction= new QAction(QIcon(":/icons/cursor.png"),
                                   tr("bring to front"), this);
    compositionAction->setCheckable(true);
    compositionAction->setStatusTip(tr("bring item to front"));
    connect(compositionAction, SIGNAL(triggered()),
            this, SLOT(compositionMode()));

    moveToLayoutAction = new QAction(QIcon(":/icons/application_go.png"),
                                     tr("move to layout"), this);
    moveToLayoutAction->setStatusTip(tr("move to layout"));
    connect(moveToLayoutAction, SIGNAL(triggered()),
            this, SLOT(moveToLayout()));

    makePersistentAction = new QAction(QIcon(":/icons/application_cascade.png"),
                                       tr("persist"), this);
    makePersistentAction->setStatusTip(tr("persist"));
    connect(makePersistentAction, SIGNAL(triggered()),
            this, SLOT(makePersistent()));

    makeInteractiveAction = new QAction(QIcon(":/icons/hand_point.png"),
                                        tr("interactive"), this);
    makeInteractiveAction->setCheckable(true);
    makeInteractiveAction->setChecked(true);
    makeInteractiveAction->setStatusTip(tr("interactive"));
    connect(makeInteractiveAction, SIGNAL(triggered()),
            this, SLOT(makePersistent()));

    groupAction = new QAction(QIcon(":/icons/application_cascade.png"),
                              tr("group"), this);
    groupAction->setStatusTip(tr("group"));
    connect(groupAction, SIGNAL(triggered()),
            this, SLOT(group()));

    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(false);
    nextStepAction->setEnabled(true);
    compositionAction->setChecked(true);
}

void CGController::attachMenu(){
    controlMenu = new QMenu();
    //    controlMenu->addAction(moveToLayoutAction);
    //    controlMenu->addAction(groupAction);
    controlMenu->addAction(makePersistentAction);
    //    controlMenu->addAction(makeInteractiveAction);
    controlMenu->addAction(deleteAction);
    controlMenu->addAction(disconnectAllAction);

    connectorMenu = new QMenu();
    connectorMenu->addAction(disconnectAction);
    connectorMenu->addAction(showAction);
    controllerMenu = new QMenu();
    controllerMenu->addAction(newLayoutAction);
    controllerMenu->addAction(clearAction);
}

void CGController::disconnectAll(){
    CGCommandManager::getInstance()->startAction("DICSONNECT ALL");
    if(CClientSingleton::getInstance()->applyStopForEachAction()==true)
        CGCompositionManager::getInstance()->getComposer()->stop();
    QGraphicsItem* item = CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems().first();
    CGLayoutItem* layout = qgraphicsitem_cast<CGLayoutItem *>(item);
    foreach(CGLayoutItemConnector* connector, layout->getConnectorsL()){
        if(connector->getConnectedConnector() != 0){
            CGDisconnectConnector* disconnect = new CGDisconnectConnector();
            disconnect->setControl(layout->getControl());
            disconnect->setControlConnector(connector);
            disconnect->setConnector(connector->getConnectedConnector());
            disconnect->doCommand();
        }
    }
    CGCommandManager::getInstance()->endAction("DICSONNECT ALL");
}

void CGController::disconnect(){
    QList<QGraphicsItem*> items = CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems();
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


QDockWidget* CGController::getGlossary(){
    return dock;
}

QDockWidget* CGController::getProject(){
    return dockProject;
}
void CGController::deleteItem(){
    CGCommandManager::getInstance()->startAction("DELETE");
    foreach (QGraphicsItem *item, CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems()) {
        if(item->type() == CGLayoutItem::Type){
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
        }
        if(item->type()!=CGConnector::Type
                &&item->type()!=CGMultiConnector::Type
                &&item->type()!=CGLayoutItemConnector::Type){
            CGDeleteControl* del= new CGDeleteControl();
            del->setScene(item->scene());
            del->setItem(item);
            del->doCommand();
        }
    }
    CGCommandManager::getInstance()->endAction("DELETE");
}

void CGController::showConnector(){
    QGraphicsItem* item = CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems().first();
    CGLayoutItemConnector* connector = qgraphicsitem_cast<CGLayoutItemConnector *>(item);
    CGCompositionManager::getInstance()->showConnected(connector->getConnectedConnector());
}

void CGController::clear(){

}

void CGController::moveToLayout(){
    //popup
}

void CGController::makePersistent(){
    QList<QGraphicsItem*> items = CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems();
    foreach(QGraphicsItem* item, items){
        if(item->type() == CGLayoutItem::Type){
            CGLayoutItem* layout = qgraphicsitem_cast<CGLayoutItem *>(item);
            layout->setIsPersistant(!layout->getIsPersistant());
            if(!layout->getIsPersistant()) layout->setLayout(CGLayoutManager::getInstance()->getCurrentLayout());
        }
        if(item->type() == CGShape::Type){
            CGShape* layout = qgraphicsitem_cast<CGShape *>(item);
            layout->setPersistent(!layout->getPersistent());
        }
        if(item->type() == CGText::Type){
            CGText* layout = qgraphicsitem_cast<CGText *>(item);
            layout->setPersistent(!layout->getPersistent());
        }
    }
}

void CGController::newLayout(){
    //    bool ok;
    //    QString text = QInputDialog::getText(0, QObject::tr("QInputDialog::getText()"),
    //                                         QObject::tr("interface name:"), QLineEdit::Normal,
    //                                         QObject::tr(""), &ok);

    //    QStringList items;
    //    items << tr("interface") << tr("configuration");

    //    QString item = QInputDialog::getItem(this, tr("QInputDialog::getItem()"),
    //                                         tr(""), items, 0, false, &ok);
    ////    if (ok && !item.isEmpty())
    ////        itemLabel->setText(item);


    layoutdialog dialog;
    dialog.exec();
    QString text = dialog.getName();
    bool ok = dialog.ret();
    QString item = dialog.getType();

    if (ok && !text.isEmpty()){
        CGCommandManager::getInstance()->startAction("CREATE LAYOUT");
        CGCreateLayout* create = new CGCreateLayout();
        create->setName(text);
        create->setType(item);
        create->doCommand();
        CGCommandManager::getInstance()->endAction("CREATE LAYOUT");
    }
}

void CGController::group(){
    QList<QGraphicsItem*> items = CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems();
    QGraphicsItemGroup* group = new QGraphicsItemGroup(0,CGLayoutManager::getInstance()->getCurrentLayout());
    foreach(QGraphicsItem* item, items){
        group->addToGroup(item);
    }
    QGraphicsRectItem* r = new QGraphicsRectItem();
    r->setRect(group->boundingRect());
    r->setPen(QPen(Qt::white));
    CGLayoutManager::getInstance()->getCurrentLayout()->destroyItemGroup(group);
    foreach(QGraphicsItem* item, items){
        item->setParentItem(r);
    }

    r->setFlag(QGraphicsItem::ItemIsMovable, true);
    r->setFlag(QGraphicsItem::ItemIsSelectable, true);
    r->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    CGLayoutManager::getInstance()->getCurrentLayout()->addItem(r);

}


void CGController::play(){
    playAction->setEnabled(false);
    pauseAction->setEnabled(true);
    stopAction->setEnabled(true);
    nextStepAction->setEnabled(true);

}

void CGController::pause(){
    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(true);
    nextStepAction->setEnabled(true);

}

void CGController::stop(){
    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(false);
    nextStepAction->setEnabled(true);

}

void CGController::nextStep(){
    playAction->setEnabled(true);
    pauseAction->setEnabled(false);
    stopAction->setEnabled(true);
    nextStepAction->setEnabled(true);

}

//commentBar's
void CGController::toFront(){
    if (CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems().isEmpty())
        return;

    QList<QGraphicsItem *> selectedItems = CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems();

    foreach(QGraphicsItem* selectedItem, selectedItems){
        QList<QGraphicsItem *> overlapItems = CGLayoutManager::getInstance()->getCurrentLayout()->items();
        qreal zValue = 0;
        foreach (QGraphicsItem *item, overlapItems) {
            if (selectedItem->zValue() >= zValue)
                zValue = item->zValue() + 1;
        }
        selectedItem->setZValue(zValue);
    }
}

void CGController::sendBack(){
    if (CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems().isEmpty())
        return;

    QGraphicsItem *selectedItem = CGLayoutManager::getInstance()->getCurrentLayout()->selectedItems().first();
    QList<QGraphicsItem *> overlapItems = selectedItem->collidingItems();

    qreal zValue = 0;
    foreach (QGraphicsItem *item, overlapItems) {
        if (item->zValue() <= zValue && item->zValue()!=-2000)
            zValue = item->zValue() - 1;
    }
    selectedItem->setZValue(zValue);
}

void CGController::currentFontChanged(const QFont &)
{
    handleFontChange();
}

void CGController::fontSizeChanged(const QString &)
{
    handleFontChange();
}

void CGController::colorChanged(){
    colorAction = qobject_cast<QAction *>(sender());
    if(colorAction!=0)colorToolButton->setIcon(createColorToolButtonIcon(qVariantValue<QColor>(colorAction->data())));
    //    currentColor = qVariantValue<QColor>(colorAction->data());
    buttonTriggered();
}

void CGController::buttonTriggered()
{
    if(colorAction!=0){
        CGLayoutManager::getInstance()->getCurrentLayout()->setItemColor(qVariantValue<QColor>(colorAction->data()));
        CGLayoutManager::getInstance()->getCurrentLayout()->setTextColor(qVariantValue<QColor>(colorAction->data()));
    }else{
        CGLayoutManager::getInstance()->getCurrentLayout()->setItemColor(Qt::white);
        CGLayoutManager::getInstance()->getCurrentLayout()->setTextColor(Qt::white);
    }
}

void CGController::handleFontChange(){
    QFont font = fontCombo->currentFont();
    font.setPointSize(fontSizeCombo->currentText().toInt());
    font.setWeight(boldAction->isChecked() ? QFont::Bold : QFont::Normal);
    font.setItalic(italicAction->isChecked());
    font.setUnderline(underlineAction->isChecked());
    CGLayoutManager::getInstance()->getCurrentLayout()->setFont(font);
}

//compositionBar's
void CGController::text(){
    if(textAction->isChecked()){
        CGLayoutManager::getInstance()->getCurrentLayout()->setMode(CGLayout::InsertText);
        shapeAction->setChecked(false);
        compositionAction->setChecked(false);
    }else{
        CGLayoutManager::getInstance()->getCurrentLayout()->setMode(CGLayout::Composition);
        compositionAction->setChecked(true);
    }
}

void CGController::shape(){
    if(shapeAction->isChecked()){
        CGLayoutManager::getInstance()->getCurrentLayout()->setMode(CGLayout::InsertShape);
        textAction->setChecked(false);
        compositionAction->setChecked(false);
    }else{
        CGLayoutManager::getInstance()->getCurrentLayout()->setMode(CGLayout::Composition);
        compositionAction->setChecked(true);
    }
}

void CGController::compositionMode(){
    if(compositionAction->isChecked()){
        CGLayoutManager::getInstance()->getCurrentLayout()->setMode(CGLayout::Composition);
        textAction->setChecked(false);
        shapeAction->setChecked(false);
    }else{
        CGLayoutManager::getInstance()->getCurrentLayout()->setMode(CGLayout::Composition);
        compositionAction->setChecked(true);
    }
}

layoutdialog::layoutdialog(QWidget *parent)
    : QDialog(parent)
{
    label = new QLabel(tr("Layout name:"));
    lineEdit = new QLineEdit;
    label->setBuddy(lineEdit);

    typeBox = new QComboBox();
    typeBox->addItem("interface");
    typeBox->addItem("configuration");

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
//    leftLayout->addWidget(typeBox);

    QGridLayout *mainLayout = new QGridLayout;

    mainLayout->addLayout(leftLayout, 0, 0);
    mainLayout->addWidget(buttonBox, 1, 0);
    mainLayout->setRowStretch(2, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("Layout creation"));
}

QString layoutdialog::getType(){
    return typeBox->currentText();
}

QString layoutdialog::getName(){
    return lineEdit->text();
}

bool layoutdialog::ret(){
    return okButton->isChecked();
}
