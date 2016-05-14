#include "copydir.h"
#include "DataBoolean.h"
#include "CLogger.h"
copydir::copydir(){
    this->structurePlug().addPlugIn(DataString::KEY,"source.str");
    this->structurePlug().addPlugIn(DataString::KEY,"target.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("FileSystem");
    this->setKey("direcopydir");
    this->setName("CopyDir");
    this->setInformation("Copy source path dir content to target path dir");
}

void copydir::exec(){
    string dirpath1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string dirpath2 = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    if(!this->cpDir(dirpath1.c_str(),dirpath2.c_str())){
        this->error("Can't copy directory. Check your paths.");
    }else{
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
    }
}

bool copydir::cpDir(QString srcPath, QString dstPath)
{
    if(QFileInfo(dstPath).exists()){
        CLogger::getInstance()->log("SCD",CLogger::ERROR,"dir already exist "+dstPath);
        return false;
    }
    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName())){
        CLogger::getInstance()->log("SCD",CLogger::ERROR,"can't mkdir "+QFileInfo(dstPath).fileName());
        return false;
    }

    QDir srcDir(srcPath);
    if(!QFileInfo(srcPath).exists()){
        CLogger::getInstance()->log("SCD",CLogger::ERROR,"dir does not exist "+srcPath);
        return false;
    }

    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                CLogger::getInstance()->log("SCD",CLogger::ERROR,"can't copy file"+srcItemPath+"to "+dstItemPath);
                return false;
            }
        } else {
            CLogger::getInstance()->log("SCD",CLogger::INFO,"Unhandled item" + info.filePath() + "in cpDir");
        }
    }
    return true;
}

COperator * copydir::clone(){
    return new copydir();
}
