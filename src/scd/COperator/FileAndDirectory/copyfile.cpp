#include "copyfile.h"
#include "CGProjectSerialization.h"
#include<DataBoolean.h>
copyfile::copyfile(){
    this->structurePlug().addPlugIn(DataString::KEY,"file1.str");
    this->structurePlug().addPlugIn(DataString::KEY,"file2.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->structurePlug().addPlugOut(DataString::KEY,"file2.str");
    this->path().push_back("FileSystem");
    this->setKey("direcopyfile");
    this->setName("CopyFile");
    this->setInformation("copy file1 to file2");
}

void copyfile::exec(){
    string filepath1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string filepath2 = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    QFileInfo info(filepath2.c_str());
    if(info.isDir()){
        QFileInfo info1(filepath1.c_str());
        QString s = info.absoluteFilePath()+"/"+info1.fileName();
        if(!CGProjectSerialization::copyFile(filepath1.c_str(),s)){
            string message = "Can't copy file! Check inputs paths file1="+filepath1+" and file 2="+filepath2;
            this->error(message);
            CLogger::getInstance()->log("SCD",CLogger::ERROR,message.c_str());
        }else{
            dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
            this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
        }
    }else{

        if(!CGProjectSerialization::copyFile(filepath1.c_str(),filepath2.c_str())){
            string message = "Can't copy file! Check inputs paths file1="+filepath1+" and file 2="+filepath2;
            this->error(message);
            CLogger::getInstance()->log("SCD",CLogger::ERROR,message.c_str());
        }else{
            dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
            this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
        }
    }
}

COperator * copyfile::clone(){
    return new copyfile();
}
