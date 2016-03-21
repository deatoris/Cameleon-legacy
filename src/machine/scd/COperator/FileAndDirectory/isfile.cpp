#include "isfile.h"


isFile::isFile(){
    this->structurePlug().addPlugIn(DataString::KEY,"path.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"isFile.bool");
    this->path().push_back("FileSystem");
    this->setKey("direisFile");
    this->setName("IsFile");
        this->setInformation("isFile=true if the path is a file, false otherwise");
}

void isFile::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    bool b = false;
    QFileInfo info(filepath.c_str());
    b = info.isFile();
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(b);
}

COperator * isFile::clone(){
    return new isFile();
}
