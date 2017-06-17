#include "instring.h"
#include "qfileinfo.h"
#include<CLogger.h>
instring::instring(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataString::KEY,"in.str");
    this->path().push_back("String");
    this->setKey("inputstring");
    this->setName("LoadString");
}

void instring::exec(){

    string filepath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();

    CLogger::getInstance()->log("MACHINE",CLogger::INFO,filepath.c_str());
    QFile file(filepath.c_str());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        this->error("Error opening file"+filepath) ;
        return;

    }
    else{
        QString linesum;
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine()+"\n";
            linesum+=line;
        }
        dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(linesum.toStdString());
        file.close();

    }
}

COperator * instring::clone(){
    return new instring();
}
