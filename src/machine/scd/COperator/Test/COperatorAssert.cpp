#include "COperatorAssert.h"
#include "CLogger.h"
#include<CUtilitySTL.h>
#include<CMachine.h>
#include<DataBoolean.h>
#include<DataString.h>
COperatorAssert::COperatorAssert(){
    this->structurePlug().addPlugIn(DataBoolean::KEY,"test.bool");
    this->structurePlug().addPlugIn(DataString::KEY,"name.str");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"test.bool");
    this->path().push_back("Test");
    this->setKey("COperatorAssert");
    this->setName("Assert");
}

void COperatorAssert::exec(){
    DataBoolean * d1 = dynamic_cast<DataBoolean *>(this->plugIn()[0]->getData());
    DataString * d2 = dynamic_cast<DataString *>(this->plugIn()[1]->getData());

    bool b1 = d1->getValue();
    string b2 = d2->getValue();
    QString s =b2.c_str();
    if(b1){
        CLogger::getInstance()->log("SCD",CLogger::INFO,"[ASSERT][SUCCESS] "+s);

    }else{
        CLogger::getInstance()->log("SCD",CLogger::INFO,"[ASSERT][ERROR] "+s);

    }

    this->plugOut()[0]->getData()->dumpReception(this->plugIn()[0]->getData());

}

COperator * COperatorAssert::clone(){
    return new COperatorAssert();
}
