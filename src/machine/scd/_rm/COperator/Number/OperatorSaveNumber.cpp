#include "OperatorSaveNumber.h"

#include<DataNumber.h>
#include<DataString.h>
#include<DataBoolean.h>
OperatorSaveNumber::OperatorSaveNumber(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"in.bool");
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"out.bool");
    this->path().push_back("Number");
    this->setKey("OperatorSaveNumber");
    this->setName("SaveNumber");
    this->setInformation("Save the Number value in the given file, out= false for bad writing, true otherwise");
}

void OperatorSaveNumber::exec(){
    double b = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    string  file = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();

    ofstream  out(file.c_str());
    if(out.is_open()){
        out<<b;
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData())->setValue(true);


}

COperator * OperatorSaveNumber::clone(){
    return new OperatorSaveNumber;
}
