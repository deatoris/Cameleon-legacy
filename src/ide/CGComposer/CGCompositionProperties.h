/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CGCOMPOSITIONPROPERTIES_H
#define CGCOMPOSITIONPROPERTIES_H
#include <QtGui>
#include "CGCompositionModel.h"
class CGCompositionProperties : public QWidget
{
    Q_OBJECT

public slots:
    void save();
    void close();

public:
    CGCompositionProperties();
    void load(CGCompositionModel* model);

private:
    CGConnector* getConnectorById(int id);
    void create();
    QStandardItemModel* model;
    QTableView* table;
    CGCompositionModel* compositionModel;
    QTextEdit* edit;
    QLineEdit * nameEdit;
};

#endif // CGCOMPOSITIONPROPERTIES_H
