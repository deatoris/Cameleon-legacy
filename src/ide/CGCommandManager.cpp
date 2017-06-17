/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CGCommandManager.h"
#include "CGCompositionManager.h"
CGCommandManager* CGCommandManager::commandManagerInstance = NULL;
#include "CLogger.h"
#include "CPerf.h"
#include<CClient.h>
//#include "CLauncher.h"
CGCommandManager::CGCommandManager()
{
    CLogger::getInstance()->registerComponent("FUNCTIONAL");
    count = 0;
    step = 5;
}

CGCommandManager* CGCommandManager::getInstance(){
    if(NULL != commandManagerInstance){
        return commandManagerInstance;
    }else{
        commandManagerInstance = new CGCommandManager();
        return commandManagerInstance;
    }
}

void CGCommandManager::add(CGCommand* command){
    CLogger::getInstance()->log("FUNCTIONAL",CLogger::DEBUG,"COMMAND - "+command->getType());
    commandList.append(command);
}

void CGCommandManager::undo(){
    if(!commandList.isEmpty()){
        CGCommand* c = commandList.last();
        CLogger::getInstance()->log("FUNCTIONAL",CLogger::DEBUG,"UNDO COMMAND - "+c->getType());
        while(!commandList.isEmpty() && c->getType().compare("SEPARATOR") != 0){
            commandList.removeOne(c);
            undocommandList.append(c);
            c->undoCommand();
            if(!commandList.isEmpty()) c = commandList.last();
        }
        if(!commandList.isEmpty() && c->getType().compare("SEPARATOR") == 0){
            commandList.removeOne(c);
            undocommandList.append(c);
            c->undoCommand();
        }
    }
}

void CGCommandManager::redo(){
    if(!undocommandList.isEmpty() ){
        CGCommand* c = undocommandList.last();
        if(c->getType().compare("SEPARATOR") == 0){
            undocommandList.removeOne(c);
            this->add(c);
            c->exec();
            if(!undocommandList.isEmpty()){
                c = undocommandList.last();
                while(!undocommandList.isEmpty() && c->getType().compare("SEPARATOR") != 0){
                    c = undocommandList.last();
                    undocommandList.removeOne(c);
                    this->add(c);
                    c->exec();
                    if(!undocommandList.isEmpty()) c = undocommandList.last();
                }
            }
        }
    }
}

void CGCommandManager::clearDoUndo(){
    commandList.clear();
    undocommandList.clear();
}


void CGCommandManager::clearUndo(){
    undocommandList.clear();
}

void CGCommandManager::endAction(QString name){
    CPerf::getInstance()->probeEnd(name);
}

void CGCommandManager::startAction(QString name){
    CLogger::getInstance()->log("FUNCTIONAL",CLogger::INFO,"ACTION - "+name);
    if(count == step){
        CGCompositionManager::getInstance()->getComposer()->saveTmp();
        count=0;
    }
    CPerf::getInstance()->probeStart(name);
    CGCommand* separator = new CGCommand();
    this->add(separator);
    count++;
}

void CGCommandManager::setStep(int s){
    this->step = s;
}

int CGCommandManager::getStep(){
    return step;
}

CGCommand::CGCommand()
{
    isReversable=true;
    type = "SEPARATOR";
}

void CGCommand::doCommand(){
    if(this->type.compare("CHANGEVIEW")!=0){
        if(CClientSingleton::getInstance()->applyStopForEachAction()==true)
            CGCompositionManager::getInstance()->getComposer()->stop();
    }
    CGCommandManager::getInstance()->clearUndo();
    CGCommandManager::getInstance()->add(this);
    this->exec();
}

void CGCommand::undoCommand(){
    this->unexec();
}

QString CGCommand::getType(){
    return this->type;
}

bool CGCommand::getIsReversable(){
    return isReversable;
}

void CGCommand::setIsReversable(bool reversable){
    this->isReversable = reversable;
}
