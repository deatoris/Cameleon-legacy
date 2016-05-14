#include "OperatorSaveVector.h"

#include<DataVector.h>
#include<DataString.h>
#include<DataBoolean.h>
OperatorSaveVector::OperatorSaveVector(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.bool");
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Vector");
    this->setKey("OperatorSaveVector");
    this->setName("SaveVector");
    this->setInformation("Save the Vector value in the given file, out= false for bad writing, true otherwise");
}

void OperatorSaveVector::exec(){
    DataVector::DATAVECTOR inVector = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    string  file = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();

    ofstream  out(file.c_str());
    if(out.is_open()){
        out<<*(inVector);
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);


}

COperator * OperatorSaveVector::clone(){
    return new OperatorSaveVector;
}
