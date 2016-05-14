#include "exists.h"


exists::exists(){
    this->structurePlug().addPlugIn(DataString::KEY,"path.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"exists.bool");
    this->path().push_back("FileSystem");
    this->setKey("direexists");
    this->setName("IsExist");
    this->setInformation("exists=true if the path exist, false otherwise");
}

void exists::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    bool b = false;
    QFileInfo info(filepath.c_str());
    b = info.exists();
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(b);
}

COperator * exists::clone(){
    return new exists();
}
