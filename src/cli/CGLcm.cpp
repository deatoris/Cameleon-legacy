#include <CUtilitySTL.h>
#include "CGLicence.h"
#include "CLauncher.h"
#include "CApplication.h"
#include "CLogger.h"
int main(int argc, char *argv[]){
    CLoggerInstance::getInstance()->log("=========================");
    CApplication a(argc, argv);
    //start instance logger
    CLoggerInstance::getInstance()->log("Application started ...");
        CLauncher * lau = new CLauncher();
        //lau->setDesktopSize(a.desktop()->screenGeometry().size());

        if(!QObject::connect(&a, SIGNAL(applicationStarted()),
                             lau, SLOT(startCli()))){
            //qDebug<<"[ERROR] Launch fatal Error";
        }
        return a.exec();

    return 0;
}
