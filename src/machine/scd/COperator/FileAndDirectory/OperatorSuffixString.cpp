

#include "OperatorSuffixString.h"
#include<QFileInfo>
OperatorSuffixString::OperatorSuffixString(){
    this->structurePlug().addPlugIn(DataString::KEY,"path.str");
    this->structurePlug().addPlugOut(DataString::KEY,"Suffix.str");
    this->path().push_back("FileSystem");
    this->setKey("OperatorSuffixString");
    this->setName("Suffix");
    this->setInformation("Get the suffix of the file without the path without including the dot (for instance path=/home/vincent/lena.pgm -> Suffix=pgm");
}

void OperatorSuffixString::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    QFileInfo info(filepath.c_str());
    QString str;
    str = info.completeSuffix();
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(str.toStdString());
}

COperator * OperatorSuffixString::clone(){
    return new OperatorSuffixString();
}
