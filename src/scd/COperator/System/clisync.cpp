#include "clisync.h"
#include <CLogger.h>
#include "CClient.h"

CommandLineSync::CommandLineSync(){
    this->structurePlug().addPlugIn(DataString::KEY,"exePath.str");
    this->structurePlug().addPlugIn(DataVector::KEY,"parameters.vec");
    this->structurePlug().addPlugOut(DataNumber::KEY,"exitCode.num");
    this->structurePlug().addPlugOut(DataString::KEY,"stdout.str");
    this->structurePlug().addPlugOut(DataString::KEY,"stderr.str");
    this->setExecutedThread(COperator::CLIENT);
    this->path().push_back("System");
    this->setKey("CommandLineSync");
    this->setName("CLI-SYNC");
    this->setInformation("to write");

    p = new QProcess();

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

}

void CommandLineSync::exec(){
    string exec_path = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    DataVector* v = dynamic_cast<DataVector *>(this->plugIn()[1]->getData());


    QStringList args = processArgs(v);



    p->start(exec_path.c_str(),args);
    if (!p->waitForStarted(-1))
        this->error("Can't launch command, you must fill the exact path");
}

QStringList CommandLineSync::processArgs(DataVector* vv){
    QStringList list;
    DataVector::DATAVECTOR v = vv->getData();
    for ( int i =0 ; i <(int) v->size(); i++ ){
        list.append((*v)[i]->toString().c_str());
    }
    return list;
}

COperator * CommandLineSync::clone(){
    return new CommandLineSync();
}

void CommandLineSync::updateMarkingAfterExecution(){
  //  qDebug("void CommandLineSync::updateMarkingAfterExecution()");

}

void CommandLineSync::processError(){
   // qDebug("void CommandLineSync::processError()");
    int exitCod = p->exitCode();
    QString p_stdout = p->readAllStandardOutput();
    QString p_stderr = p->readAllStandardError();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(exitCod);
    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(p_stdout.toStdString());
    dynamic_cast<DataString *>(this->plugOut()[2]->getData())->setValue(p_stderr.toStdString());
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
    p_stderr = "";
    p_stdout = "";
    //CGCompositionManager::getInstance()->getComposer()->play();
}

void CommandLineSync::processStoped(int exitCode, QProcess::ExitStatus exitStatus){
    //qDebug("void CommandLineSync::processStoped()");
    int exitCod = p->exitCode();

        dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(exitCod);
    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(p_stdout.toStdString());
    dynamic_cast<DataString *>(this->plugOut()[2]->getData())->setValue(p_stderr.toStdString());

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
    p_stderr = "";
    p_stdout = "";
    //CGCompositionManager::getInstance()->getComposer()->play();
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->startSend();

}

void CommandLineSync::readError(){
   //qDebug("void CommandLineSync::readError()");
    QString tmp = p->readAllStandardError();
    p_stderr = p_stderr+tmp;
    //qDebug(tmp.toStdString().c_str());
   // CLogger::getInstance()->log("SCD",CLogger::INFO,tmp.toStdString().c_str());

}

void CommandLineSync::readOut(){
    //qDebug("void CommandLineSync::readOut()");
    QString tmp = p->readAllStandardOutput();
    p_stdout = p_stdout+tmp;
    //qDebug(tmp.toStdString().c_str());
    //CLogger::getInstance()->log("SCD",CLogger::INFO,tmp.toStdString().c_str());

}

CommandLineSync2::CommandLineSync2(){
    this->structurePlug().addPlugIn(DataString::KEY,"exePath.str");
    this->structurePlug().addPlugIn(DataVector::KEY,"parameters.vec");
    this->structurePlug().addPlugIn(DataString::KEY,"runPath.str");
    this->structurePlug().addPlugOut(DataNumber::KEY,"exitCode.num");
    this->structurePlug().addPlugOut(DataString::KEY,"stdout.str");
    this->structurePlug().addPlugOut(DataString::KEY,"stderr.str");
    this->setExecutedThread(COperator::CLIENT);
    this->path().push_back("System");
    this->setKey("CommandLineSync2");
    this->setName("CLI-SYNC2");
    this->setInformation("to write");
}

void CommandLineSync2::exec(){
    string exec_path = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    DataVector* v = dynamic_cast<DataVector *>(this->plugIn()[1]->getData());
    string run_path = dynamic_cast<DataString *>(this->plugIn()[2]->getData())->getValue();

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

    QDir::setCurrent(QString::fromStdString(run_path));

    p->start(exec_path.c_str(),args);
    if (!p->waitForStarted(-1))
        this->error("Can't launch command, you must fill the exact path");
}

QStringList CommandLineSync2::processArgs(DataVector* vv){
    QStringList list;
    DataVector::DATAVECTOR v = vv->getData();
    for ( int i =0 ; i <(int) v->size(); i++ ){
        list.append((*v)[i]->toString().c_str());
    }
    return list;
}

COperator * CommandLineSync2::clone(){
    return new CommandLineSync2();
}

void CommandLineSync2::updateMarkingAfterExecution(){
   // qDebug("void CommandLineSync2::updateMarkingAfterExecution()");

}

void CommandLineSync2::processError(){
    //qDebug("void CommandLineSync2::processError()");
    int exitCod = p->exitCode();
    QString p_stdout = p->readAllStandardOutput();
    QString p_stderr = p->readAllStandardError();
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(exitCod);
    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(p_stdout.toStdString());
    dynamic_cast<DataString *>(this->plugOut()[2]->getData())->setValue(p_stderr.toStdString());
    this->plugIn()[0]->setState(CPlug::OLD);
    this->plugIn()[0]->send();
    this->plugIn()[1]->setState(CPlug::OLD);
    this->plugIn()[1]->send();
    this->plugIn()[2]->setState(CPlug::OLD);
    this->plugIn()[2]->send();
    this->plugOut()[0]->setState(CPlug::NEW);
    this->plugOut()[0]->send();
    this->plugOut()[1]->setState(CPlug::NEW);
    this->plugOut()[1]->send();
    this->plugOut()[2]->setState(CPlug::NEW);
    this->plugOut()[2]->send();
    p_stderr = "";
    p_stdout = "";
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->startSend();
}

void CommandLineSync2::processStoped(int exitCode, QProcess::ExitStatus exitStatus){
    //qDebug("void CommandLineSync2::processStoped()");
    int exitCod = p->exitCode();

        dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(exitCod);
    dynamic_cast<DataString *>(this->plugOut()[1]->getData())->setValue(p_stdout.toStdString());
    dynamic_cast<DataString *>(this->plugOut()[2]->getData())->setValue(p_stderr.toStdString());

    this->plugIn()[0]->setState(CPlug::OLD);
    this->plugIn()[0]->send();
    this->plugIn()[1]->setState(CPlug::OLD);
    this->plugIn()[1]->send();
    this->plugIn()[2]->setState(CPlug::OLD);
    this->plugIn()[2]->send();
    this->plugOut()[0]->setState(CPlug::NEW);
    this->plugOut()[0]->send();
    this->plugOut()[1]->setState(CPlug::NEW);
    this->plugOut()[1]->send();
    this->plugOut()[2]->setState(CPlug::NEW);
    this->plugOut()[2]->send();
    this->updateState();
    p_stderr = "";
    p_stdout = "";
    CClientSingleton::getInstance()->getCInterfaceClient2Server()->startSend();
}

void CommandLineSync2::readError(){
   // qDebug("void CommandLineSync2::readError()");
    QString tmp = p->readAllStandardError();
    p_stderr = p_stderr+tmp;
  //  qDebug(tmp.toStdString().c_str());
  //  CLogger::getInstance()->log("SCD",CLogger::INFO,tmp.toStdString().c_str());

}

void CommandLineSync2::readOut(){
  //  qDebug("void CommandLineSync2::readOut()");
    QString tmp = p->readAllStandardOutput();
    p_stdout = p_stdout+tmp;
  //  qDebug(tmp.toStdString().c_str());
  // CLogger::getInstance()->log("SCD",CLogger::INFO,tmp.toStdString().c_str());

}
