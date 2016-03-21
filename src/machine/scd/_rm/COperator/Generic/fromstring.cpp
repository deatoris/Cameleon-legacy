#include "fromstring.h"
#include "DataString.h"
#include "CGlossary.h"

fromstring::fromstring(){
    this->structurePlug().addPlugIn(DataString::KEY,"from.str");
    this->structurePlug().addPlugIn(DataString::KEY,"type.str");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"output.gen");
    this->path().push_back("Generic");
    this->setKey("fromstring");
    this->setName("FromString");
    this->setInformation("Create a data of the given type and load data information with from");
}

void fromstring::exec(){
    DataString* from = dynamic_cast<DataString *>(this->plugIn()[0]->getData());
    DataString* type = dynamic_cast<DataString *>(this->plugIn()[1]->getData());
    CData* data = CGlossarySingletonClient::getInstance()->createData(type->getValue());
    data->fromString(from->getValue());

    DataGeneric* gen = dynamic_cast<DataGeneric *>(this->plugOut()[0]->getData());
    gen->dumpReception(data);
}

COperator * fromstring::clone(){
    return new fromstring();
}
