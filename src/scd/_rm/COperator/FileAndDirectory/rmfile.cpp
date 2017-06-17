#include "rmfile.h"
#include<DataBoolean.h>
#include "CLogger.h"

rmfile::rmfile(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("FileSystem");
    this->setKey("dirermfile");
    this->setName("RmFile");
    this->setInformation("remove file");
}

void rmfile::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();

    QFileInfo info(filepath.c_str());
    QString d = info.absolutePath();
    QDir dir(d);
    if(dir.remove(info.fileName())){
        this->error("Can't delete file");
        CLogger::getInstance()->log("SCD",CLogger::ERROR,"Can't delete file");
    }else{
        dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
    }
}

COperator * rmfile::clone(){
    return new rmfile();
}
