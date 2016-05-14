#include "OperatorBaseNameString.h"
#include<QFileInfo>
OperatorBaseNameString::OperatorBaseNameString(){
    this->structurePlug().addPlugIn(DataString::KEY,"path.str");
    this->structurePlug().addPlugOut(DataString::KEY,"basename.str");
    this->path().push_back("FileSystem");
    this->setKey("OperatorBaseNameString");
    this->setName("BaseName");
    this->setInformation("Get the base name of the file without the path (for instance path=/home/vincent/lena.pgm -> basename=lena");
}

void OperatorBaseNameString::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    QFileInfo info(filepath.c_str());
    QString str;
    str = info.baseName();
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(str.toStdString());
}

COperator * OperatorBaseNameString::clone(){
    return new OperatorBaseNameString();
}
