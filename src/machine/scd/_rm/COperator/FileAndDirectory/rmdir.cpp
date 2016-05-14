#include "rmdir.h"
#include<DataBoolean.h>
#include "CLogger.h"
rmDir::rmDir(){
    this->structurePlug().addPlugIn(DataString::KEY,"dir.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");

    this->path().push_back("FileSystem");
    this->setKey("dirermDir");
    this->setName("RmDir");
    this->setInformation("remove dir");
}

void rmDir::exec(){
    string dirpath1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    if(this->eraseDir(dirpath1.c_str())){
        this->error("Can't delete dir");
        CLogger::getInstance()->log("SCD",CLogger::ERROR,"Can't delete dir");
    }else{
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
    }
}

bool rmDir::eraseDir(QString dirPath)
{
    QDir dir(dirPath);
    if (!dir.exists())
        return true;
    foreach(const QFileInfo &info, dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        if (info.isDir()) {
            if (!eraseDir(info.filePath()))
                return false;
        } else {
            if (!dir.remove(info.fileName()))
                return false;
        }
    }
    QDir parentDir(QFileInfo(dirPath).path());
    return parentDir.rmdir(QFileInfo(dirPath).fileName());
}

COperator * rmDir::clone(){
    return new rmDir();
}
