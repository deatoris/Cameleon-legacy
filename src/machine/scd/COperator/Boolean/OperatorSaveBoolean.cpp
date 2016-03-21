#include "OperatorSaveBoolean.h"

#include<DataBoolean.h>
#include<DataString.h>

OperatorSaveBoolean::OperatorSaveBoolean(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"in.bool");
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Boolean");
    this->setKey("OperatorSaveBoolean");
    this->setName("SaveBoolean");
    this->setInformation("Save the boolean value in the given file, out= false for bad writing, true otherwise");
}

void OperatorSaveBoolean::exec(){
    bool b = dynamic_cast<DataBoolean *>(this->plugIn()[0]->getData())->getValue();
    string  file = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    ofstream  out(file.c_str());
    if(out.is_open()){
        out<<b;
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);


}

COperator * OperatorSaveBoolean::clone(){
    return new OperatorSaveBoolean;
}
