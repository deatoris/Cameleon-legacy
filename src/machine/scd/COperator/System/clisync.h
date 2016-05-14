#ifndef CommandLineSync_H
#define CommandLineSync_H
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
#include<DataNumber.h>
#include <QProcess>
class CommandLineSync: public COperator
{
        Q_OBJECT
public slots:
    void processStoped(int exitCode, QProcess::ExitStatus exitStatus);
    void processError();
    void readOut();
    void readError();

public:
    CommandLineSync();
    virtual void exec();
    virtual COperator * clone();
    void updateMarkingAfterExecution();
private:
    QString p_stderr;
    QString p_stdout;
    QStringList processArgs(DataVector* table);
    QProcess* p;
};

class CommandLineSync2: public COperator
{
        Q_OBJECT
public slots:
    void processStoped(int exitCode, QProcess::ExitStatus exitStatus);
    void processError();
    void readOut();
    void readError();

public:
    CommandLineSync2();
    virtual void exec();
    virtual COperator * clone();
    void updateMarkingAfterExecution();
private:
    QString p_stderr;
    QString p_stdout;
    QStringList processArgs(DataVector* table);
    QProcess* p;
};


#endif // CommandLineSync_H
