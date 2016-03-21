#ifndef CommandLine_H
#define CommandLine_H
#include<COperator.h>
#include<DataString.h>
#include<DataVector.h>
#include<DataNumber.h>
#include <QProcess>
class CommandLine: public COperator
{
        Q_OBJECT
public slots:
    void processStoped(int exitCode, QProcess::ExitStatus exitStatus);
    void processError();
    void readOut();
    void readError();

public:
    CommandLine();
    virtual void exec();
    virtual COperator * clone();
    void updateMarkingAfterExecution();
private:
    QStringList processArgs(DataVector* table);
    QProcess* p;
};

#endif // CommandLine_H
