#include "OperatorSaveString.h"


#include<DataString.h>
#include<DataString.h>
#include<DataBoolean.h>
OperatorSaveString::OperatorSaveString(){
    this->structurePlug().addPlugIn(DataString::KEY,"in.str");
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("String");
    this->setKey("OperatorSaveString");
    this->setName("SaveString");
    this->setInformation("Save the String value in the given file, out= false for bad writing, true otherwise");
}

void OperatorSaveString::exec(){
    string str = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string  file = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    ofstream  out(file.c_str());
    if(out.is_open())
        out<<str;
    else
        this->error("Cannot open file: "+file);
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);
    out.close();
}

COperator * OperatorSaveString::clone(){
    return new OperatorSaveString;
}
