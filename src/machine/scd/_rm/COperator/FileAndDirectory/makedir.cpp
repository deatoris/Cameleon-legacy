#include "makedir.h"
#include "DataBoolean.h"
#include "CLogger.h"
MakeDir::MakeDir(){
    this->structurePlug().addPlugIn(DataString::KEY,"target.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("FileSystem");
    this->setKey("direMakeDir");
    this->setName("MakeDir");
    this->setInformation("Create a directory");
}

void MakeDir::exec(){
    string dirpath1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    QFileInfo info(dirpath1.c_str());
    QDir dir = info.absoluteDir();

    if(!dir.mkdir(info.fileName())){
        this->error("Can't create directory!");
        return;
    }

    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
}

COperator * MakeDir::clone(){
    return new MakeDir();
}
