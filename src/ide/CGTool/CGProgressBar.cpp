/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGProgressBar.h"

CGProgressBar::CGProgressBar()
{
    /*
      DEFAULT CONFIG
      */
    maxTime = 1;

    //widgets
    bar = new QProgressBar();
    bar->setStyleSheet("QProgressBar:horizontal {"
                       "border: 1px solid gray;"
                       "border-radius: 3px;"
                       "background: black;"
                        "padding: 1px;"
                       "}"
                       "QProgressBar::chunk:horizontal {"
                       "background: qlineargradient(x1: 0, y1: 0.5, x2: 1, y2: 0.5, stop: 0 gray, stop: 1 gray);"
                       "}");
    bar->setTextVisible(false);
    messageLabel = new QLabel();
    componentLabel = new QLabel();
    pourcentLabel = new QLabel();

    widget = new QWidget(0,Qt::Window);
    widget->setWindowFlags(Qt::FramelessWindowHint);
    QImage* image = new QImage("ressource/logo_front.png");
    QLabel* imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Shadow);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);
    imageLabel->setPixmap(QPixmap::fromImage(*image));
    imageLabel->setScaledContents(true);

    imageLabel->setPixmap(QPixmap::fromImage(*image));

    QVBoxLayout* vlayout = new QVBoxLayout();
    vlayout->setMargin(0);
    vlayout->setSpacing(0);

    vlayout->addWidget(imageLabel);
    vlayout->addWidget(bar);
    widget->setLayout(vlayout);
    widget->hide();
    connect(this, SIGNAL(updateNumber(int)),
            bar, SLOT(setValue(int)),Qt::QueuedConnection);

    connect(this, SIGNAL(updatePercent(QString)),
            pourcentLabel, SLOT(setText(QString)),Qt::QueuedConnection);

    connect(this, SIGNAL(updateComponent(QString)),
            componentLabel, SLOT(setText(QString)),Qt::QueuedConnection);

    connect(this, SIGNAL(updateMessage(QString)),
            messageLabel, SLOT(setText(QString)),Qt::QueuedConnection);

}

QWidget* CGProgressBar::getWidget(){
    return widget;
}

void CGProgressBar::start(int numberToProcess){
    timer.start();
    max = numberToProcess;
    bar->setMaximum(100);
    bar->setMinimum(0);
    if(widget->isHidden()) widget->show();
}

void CGProgressBar::update(int number, QString component, QString message){
    QCoreApplication::processEvents();
    int time = timer.elapsed();
    if(time>maxTime){
        double n = 1.0*number/max;
        emit updateMessage(message);
        emit updateComponent(component);
        int per = n*100;
        emit updateNumber(per);
        emit updatePercent(QString::number(per)+"%");
    }
}

void CGProgressBar::end(){
    widget->close();
    delete widget;
}
