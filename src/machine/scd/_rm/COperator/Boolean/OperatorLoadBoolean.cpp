#include "OperatorLoadBoolean.h"
#include<DataBoolean.h>
#include<DataString.h>

OperatorLoadBoolean::OperatorLoadBoolean(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Boolean");
    this->setKey("OperatorLoadBoolean");
    this->setName("LoadBoolean");
    this->setInformation("Load the boolean value in the given file");
}

void OperatorLoadBoolean::exec(){
    string  file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    bool b;
    ifstream  out(file.c_str());
    if(out.is_open()){
        out>>b;
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setData(&b);


}

COperator * OperatorLoadBoolean::clone(){
    return new OperatorLoadBoolean;
}
