#include "filedir.h"

filedir::filedir(){
    this->structurePlug().addPlugIn(DataString::KEY,"path.str");
    this->structurePlug().addPlugOut(DataString::KEY,"abspath.str");
    this->path().push_back("FileSystem");
    this->setKey("direfiledir");
    this->setName("AbsoluteDir");
    this->setInformation("Get the absolute directory (by removing the file name in case of file)");
}

void filedir::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    QFileInfo info(filepath.c_str());
    QString str;
    if(info.isDir())
        str = info.absoluteFilePath();
    else if(info.isFile())
        str = info.absolutePath();
    else
        str = info.absolutePath();
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(str.toStdString());
}

COperator * filedir::clone(){
    return new filedir();
}
