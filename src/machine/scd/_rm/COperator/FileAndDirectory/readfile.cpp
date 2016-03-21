#include "readfile.h"

readfile::readfile(){
    this->structurePlug().addPlugIn(DataString::KEY,"file path.str");
    this->structurePlug().addPlugOut(DataString::KEY,"content.str");
    this->path().push_back("FileSystem");
    this->setKey("direreadfile");
    this->setName("Readfile");
}

void readfile::exec(){
    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();

    QFile file(filepath.c_str());

    if(!file.exists()){
        this->error("File doesn't exist!");
        return;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        this->error("Can't open file!");
        return;
    }

    QTextStream in(&file);
    QString line = in.readLine();
    QString outstr = "";
    while (!line.isNull()) {
        outstr = outstr+line;
        line = in.readLine();
    }
    dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(outstr.toStdString());
}

COperator * readfile::clone(){
    return new readfile();
}
