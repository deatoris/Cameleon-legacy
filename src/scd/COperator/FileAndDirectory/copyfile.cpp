#include "copyfile.h"
#include<DataBoolean.h>
#include "CLogger.h"
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
        if(!this->copyFile(filepath1.c_str(),s)){
            string message = "Can't copy file! Check inputs paths file1="+filepath1+" and file 2="+filepath2;
            this->error(message);
            CLogger::getInstance()->log("SCD",CLogger::ERROR,message.c_str());
        }else{
            dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
            this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
        }
    }else{
        if(!this->copyFile(filepath1.c_str(),filepath2.c_str())){
            string message = "Can't copy file! Check inputs paths file1="+filepath1+" and file 2="+filepath2;
            this->error(message);
            CLogger::getInstance()->log("SCD",CLogger::ERROR,message.c_str());
        }else{
            dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
            this->plugOut()[1]->getData()->dumpReception(this->plugIn()[1]->getData());
        }
    }
}

bool copyfile::copyFile(QString oldFilePath, QString newFilePath)
{
    //same file, no need to copy
    if(oldFilePath.compare(newFilePath) == 0)
        return true;

    //load both files
    QFile oldFile(oldFilePath);
    QFile newFile(newFilePath);
    bool openOld = oldFile.open( QIODevice::ReadOnly );
    bool openNew = newFile.open( QIODevice::WriteOnly );

    //if either file fails to open bail
    if(!openOld || !openNew) { return false; }

    //copy contents
    uint BUFFER_SIZE = 16000;
    char* buffer = new char[BUFFER_SIZE];
    while(!oldFile.atEnd())
    {
        qint64 len = oldFile.read( buffer, BUFFER_SIZE );
        newFile.write( buffer, len );
    }

    //deallocate buffer
    delete[] buffer;
    buffer = NULL;
    return true;
}

COperator * copyfile::clone(){
    return new copyfile();
}
