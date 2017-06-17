/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGThreadInProgressDialog.h"

#include "CGCompositionManager.h"
#include "CGLayoutManager.h"
#include "CGCompositionItem.h"
#include "CLogger.h"
CGThreadInProgressDialog::CGThreadInProgressDialog()
{
    QVBoxLayout* layout = new QVBoxLayout();

    QHBoxLayout* buttonLayout= new QHBoxLayout();
    QPushButton* prevbutton = new QPushButton("save and restart");
    QPushButton* nextbutton = new QPushButton("kill threads");

    buttonLayout->addWidget(prevbutton);
//    buttonLayout->addWidget(nextbutton);

    QLabel * label = new QLabel("<b>Waiting current operator to end ...</b>");
    QLabel * label2 = new QLabel("Info: we can't kill current operator's run because it could goes to instability.");
        connect(prevbutton, SIGNAL(clicked()),
                this, SLOT(restart()));
        connect(nextbutton, SIGNAL(clicked()),
                this, SLOT(killThread()));
    this->setLayout(layout);

    QProgressBar* b = new QProgressBar();
    b->setMinimum(0);
    b->setMaximum(100);
    b->setValue(100);
    b->setTextVisible(false);
    layout->addWidget(label);
    layout->addWidget(label2);
    layout->addWidget(b);
    layout->addLayout(buttonLayout);
//    this->resize(750,150);
    this->setWindowIcon(QIcon(":/icons/clogo.png"));
    this->setWindowTitle("Waiting ...");
    current = 0;
}

void CGThreadInProgressDialog::restart(){
    CGCompositionManager::getInstance()->getComposer()->saveComposition();
    qApp->quit();
    QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
}

void CGThreadInProgressDialog::killThread(){

}

