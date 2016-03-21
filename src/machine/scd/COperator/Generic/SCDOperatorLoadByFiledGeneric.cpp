#include "SCDOperatorLoadByFiledGeneric.h"


#include<CGlossary.h>
#include<CData.h>
#include<DataString.h>
SCDOperatorLoadByFiledGeneric::SCDOperatorLoadByFiledGeneric(){
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugIn(DataString::KEY,"datatype.str");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"blank.gen");
    this->path().push_back("Generic");
    this->setKey("SCDOperatorLoadByFiledGeneric");
    this->setName("LoadByFile");
    this->setInformation("Create a data by file following the data type and load the data information contained in the given file");
}

void SCDOperatorLoadByFiledGeneric::exec(){
    string file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string datatype = dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getValue();

    CData * d = CGlossarySingletonServer::getInstance()->createData(datatype);
    if(d!=NULL){
        d->fromString(file);
        DataGeneric* gen = dynamic_cast<DataGeneric *>(this->plugOut()[0]->getData());
        gen->dumpReception(d);
    }else{
        this->error("data type doesn't exist"+datatype);
        return;
    }
}

COperator * SCDOperatorLoadByFiledGeneric::clone(){
    return new SCDOperatorLoadByFiledGeneric();
}
