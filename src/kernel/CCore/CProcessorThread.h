/*
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#ifndef CPROCESSORTHREAD_H
#define CPROCESSORTHREAD_H
#include<CProcessor.h>
#include<COperatorThread.h>
#include<vector>
class COperatorThread;
using namespace std;






class CProcessorThread :  public QObject, public CProcessor
{
        Q_OBJECT
signals:
    void tryExecutedOperatorSignal();
    void killAllThread();
public slots:
//    void finishedExecutedOperator(int idop);
    void tryExecutedOperatorSlot();
signals:
    void startExec(int opid, int threadid);
public slots:
    void endExec(int opid,int threadid);
    void endExec(int opid);

private:
    vector<bool> _v_thread_running;
//    vector<COperatorThread*> _v_object;
    vector<QThread*> _v_thread;
    int _nbr_thread_running;
    int _nbrthread;
public:

    CProcessorThread();
    void setNbrThread(int nbrthread);
    virtual bool endExecutingLoop();
    virtual bool executeOperator(int opid);
    virtual void executingLoop();
    virtual void start();
//    virtual void next();
    virtual void stop();
    virtual void pause();
};

#endif // CPROCESSORTHREAD_H
