#include "isdir.h"


isDir::isDir(){
    this->structurePlug().addPlugIn(DataString::KEY,"path.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"isDir.bool");
    this->path().push_back("FileSystem");
    this->setKey("direisDir");
    this->setName("IsDir");
    this->setInformation("isDir=true if the path is a directory, false otherwise");
}

void isDir::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    bool b = false;
    QFileInfo info(filepath.c_str());
    b = info.isDir();
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(b);
}

COperator * isDir::clone(){
    return new isDir();
}
