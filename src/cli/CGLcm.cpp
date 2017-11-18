/*======================================
Camlon Creator & Population

Copyright 2012 O. Cugnon de Svricourt & V. Tariel

This software is under the MIT license :
Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the Software), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED AS IS, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

More informations can be found here: http://www.shinoe.org/cameleon
======================================*/
#include <QtGui/QApplication>

#include <CUtilitySTL.h>
#include "CGLicence/CGLicence.h"
#include "CLauncher.h"
#include "CGApplication.h"
#include "CLogger.h"
//#include <math.h>

//#include<CInterfaceClient.h>
//#include<CClient.h>
int main(int argc, char *argv[]){
    CLoggerInstance::getInstance()->log("=========================");
    CGApplication a(argc, argv);
    //start instance logger
    CLoggerInstance::getInstance()->log("Application started ...");
        CLauncher * lau = new CLauncher();
        qDebug()<<"blabla0";
        //lau->setDesktopSize(a.desktop()->screenGeometry().size());

        if(!QObject::connect(&a, SIGNAL(applicationStarted()),
                             lau, SLOT(startCli()))){
            //qDebug<<"[ERROR] Launch fatal Error";
        }
            qDebug()<<"blabla6";
        return a.exec();

    return 0;
}
