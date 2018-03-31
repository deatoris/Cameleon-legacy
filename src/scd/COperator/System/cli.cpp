#include "cli.h"
//#include "CGCompositionManager.h"
#include <QtCore>
#include "CClient.h"
CommandLine::CommandLine(){
    this->structurePlug().addPlugIn(DataString::KEY,"exePath.str");
    this->structurePlug().addPlugIn(DataVector::KEY,"parameters.vec");
    this->structurePlug().addPlugOut(DataString::KEY,"standard.str");
    this->structurePlug().addPlugOut(DataString::KEY,"error.str");
    this->structurePlug().addPlugOut(DataNumber::KEY,"exitCode.num");
    this->setExecutedThread(COperator::CLIENT);
    this->path().push_back("System");
    this->setKey("CommandLine");
    this->setName("CLI");
    this->setInformation("to write");
}

void CommandLine::exec(){
    string exec_path = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    DataVector* v = dynamic_cast<DataVector *>(this->plugIn()[1]->getData());

    p = new QProcess();

    QStringList args = processArgs(v);

    if(!connect(p, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processStoped(int, QProcess::ExitStatus)),Qt::DirectConnection)){
        this->error("connect finished error. code 1");
    }

    if(!connect(p, SIGNAL(error(QProcess::ProcessError)), this, SLOT(processError()))){
        this->error("connect error error. code 2");
    }

    if(!connect(p, SIGNAL(readyReadStandardOutput ()), this, SLOT(readOut()))){
        this->error("connect error error. code 3");
    }

    if(!connect(p, SIGNAL(readyReadStandardError ()), this, SLOT(readError()))){
        this->error("connect error error. code 4");
    }

    p->start(exec_path.c_str(),args);
    if (!p->waitForStarted(-1))
        this->error("Can't launch command, you must fill the exact path");
}

QStringList CommandLine::processArgs(DataVector* vv){
    QStringList list;
    DataVector::DATAVECTOR v = vv->getData();
    for ( int i =0 ; i <(int) v->size(); i++ ){
        list.append((*v)[i]->toString().c_str());
    }
    return list;
}

COperator * CommandLine::clone(){
    return new CommandLine();
}

void CommandLine::updateMarkingAfterExecution(){
    qDebug("void CommandLine::updateMarkingAfterExecution()");

}

void CommandLine::processError(){
    qDebug("void CommandLine::processError()");
    QString p_stdout = p->readAllStandardOutput();
    QString p_stderr = p->readAllStandardError();
    int exitCod = p->exitCode();

    dynamic_cast<DataNumber *>(this->plugOut()[2]->getData())->setValue(exitCod);
    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(p_stderr.toStdString());
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(p_stdout.toStdString());
    this->plugIn()[0]->setState(CPlug::OLD);
    this->plugIn()[0]->send();
    this->plugIn()[1]->setState(CPlug::OLD);
    this->plugIn()[1]->send();
    this->plugOut()[0]->setState(CPlug::NEW);
    this->plugOut()[0]->send();
    this->plugOut()[1]->setState(CPlug::NEW);
    this->plugOut()[1]->send();
    this->plugOut()[2]->setState(CPlug::NEW);
    this->plugOut()[2]->send();
    this->updateState();
    //CGCompositionManager::getInstance()->getComposer()->play();
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->startSend();

}

void CommandLine::processStoped(int exitCode, QProcess::ExitStatus exitStatus){
    qDebug("void CommandLine::processStoped()");
    QString p_stdout = p->readAllStandardOutput();
    QString p_stderr = p->readAllStandardError();
    int exitCod = p->exitCode();

    dynamic_cast<DataNumber *>(this->plugOut()[2]->getData())->setValue(exitCod);
    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(p_stderr.toStdString());
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(p_stdout.toStdString());
    this->plugIn()[0]->setState(CPlug::OLD);
    this->plugIn()[0]->send();
    this->plugIn()[1]->setState(CPlug::OLD);
    this->plugIn()[1]->send();
    this->plugOut()[0]->setState(CPlug::NEW);
    this->plugOut()[0]->send();
    this->plugOut()[1]->setState(CPlug::NEW);
    this->plugOut()[1]->send();
    this->plugOut()[2]->setState(CPlug::NEW);
    this->plugOut()[2]->send();
    this->updateState();
//    CGCompositionManager::getInstance()->getComposer()->play();
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->startSend();

}

void CommandLine::readError(){
    qDebug("void CommandLine::readError()");
    QString p_stderr = p->readAllStandardError();
    qDebug(p_stderr.toStdString().c_str());

    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(p_stderr.toStdString());
    this->plugOut()[1]->setState(CPlug::NEW);
    this->plugOut()[1]->send();
}

void CommandLine::readOut(){
    qDebug("void CommandLine::readOut()");
    QString p_stdout = p->readAllStandardOutput();
    qDebug(p_stdout.toStdString().c_str());

    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(p_stdout.toStdString());
    this->plugOut()[0]->setState(CPlug::NEW);
    this->plugOut()[0]->send();
}

StdOuptutOperator::StdOuptutOperator(){
    this->structurePlug().addPlugIn(DataString::KEY,"message.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("System");
    this->setKey("StdOuptutOperator");
    this->setName("StdOutput");
    this->setInformation("Write message string to standard output");
}
void StdOuptutOperator::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    qDebug() << v1.c_str();
    DataBoolean * dret = dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData());
    dret->setValue(true);
}

COperator * StdOuptutOperator::clone(){
    return new StdOuptutOperator();
}
