#include "relativedir.h"

relativedir::relativedir(){
    this->structurePlug().addPlugIn(DataString::KEY,"path.str");
    this->structurePlug().addPlugIn(DataString::KEY,"to.str");
    this->structurePlug().addPlugOut(DataString::KEY,"relativepath.str");
    this->path().push_back("FileSystem");
    this->setKey("direrelativedir");
    this->setName("MakeRelative");
    this->setInformation("Get the absolute directory (by removing the file name in case of file)");
}

void relativedir::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string filepathto = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    QString relativepath;
    QFileInfo info(filepathto.c_str());
    QFileInfo itInfo(filepath.c_str());
    if(itInfo.isDir()){
        QDir dto = info.absoluteDir();
        QDir d = itInfo.absoluteDir();
        QString s = d.absolutePath()+"/"+itInfo.baseName()+"/s.pgm";
        QString toReturn  = dto.relativeFilePath(s);
        QFileInfo i(toReturn);
        relativepath = i.path();
    }else{
        QDir d = info.absoluteDir();
        relativepath  = d.relativeFilePath(filepath.c_str());
    }
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(relativepath.toStdString());
}

COperator * relativedir::clone(){
    return new relativedir();
}

