/*======================================
Caméléon Creator & Population 

Copyright 2012 O. Cugnon de Sévricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include "CMachine.h"
#include<QWaitCondition>
#include <QMetaType>
#include<CLogger.h>

CProcessorObject * CMachine::getProcessor(){
    return _processor;
}

void CMachine::setProcessor(CProcessorObject * processor){
    _processor = processor;
}

CMachine::CMachine()
{
    CLoggerInstance::getInstance()->log("INNER Start machine ...");
    CProcessorObject * processor = new CProcessorObject;
    processor->moveToThread(this);
    this->setProcessor(processor);

    CLogger::getInstance()->registerComponent("MACHINE");
    CLoggerInstance::getInstance()->log("INNER Machine started");
}
string CMachine::getTmpPath(){
    return _dir;
}

void CMachine::setTmpPath(string tmpdir){
    _dir = tmpdir;
}

void CMachine::run(){
    this->exec();
}
Mode CMachine::getMode(){
    return _mode;
}

void CMachine::setMode(Mode mode){
    _mode = mode;
    _processor->setMode(mode);
}
