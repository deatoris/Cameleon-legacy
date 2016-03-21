/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef COPERATORTHREAD_H
#define COPERATORTHREAD_H
#include<QThread>
#include<COperator.h>
#include<QMutex>

class COperatorThread:  public QObject
{
    Q_OBJECT
private:
    COperator *_op;
public:
    COperatorThread();
    void setOperator(COperator *op);
signals:
    void endExec(int opid,int threadid);
public slots:
    void startExec(int opid, int threadid);

};

//class COperatorThread : public QThread
//{
//    Q_OBJECT
//private:
//   COperator * _op;

//signals:
//    void finished(int opid);
//public slots:
//    void myexec();
//public:
//    COperatorThread(COperator *op,QObject *parent = 0);
//    void run();
//    QMutex _mutex;
//};

#endif // COPERATORTHREAD_H
