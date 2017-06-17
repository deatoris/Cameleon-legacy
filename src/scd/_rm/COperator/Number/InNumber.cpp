#include "InNumber.h"
#include "DataString.h"
#include<DataNumber.h>
innumber::innumber(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataNumber::KEY,"out.bool");
    this->path().push_back("Number");
    this->setKey("OperatorLoadNumber");
    this->setName("LoadNumber");
    this->setInformation("Load the number value in the given file");
}

void innumber::exec(){
    string  file = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();
    double v;
    ifstream  out(file.c_str());
    if(out.is_open()){
        out>>v;
    }
    else{
        this->error("Cannot open file:"+file);
    }
    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue(v);
}

COperator * innumber::clone(){
    return new innumber();
}
