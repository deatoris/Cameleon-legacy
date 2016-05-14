#include "OperatorErrorString.h"
#include<DataString.h>
#include<DataNumber.h>
#include<CMachine.h>
OperatorErrorString::OperatorErrorString(){
    this->structurePlug().addPlugOut(DataString::KEY,"message.str");
    this->structurePlug().addPlugOut(DataNumber::KEY,"opid.num");
    this->path().push_back("String");
    this->setKey("OperatorErrorString");
    this->setName("ErrorString");
    this->setInformation("When an error occurs, this operator is executed!!!");

    connect(CMachineSingleton::getInstance()->getProcessor(),SIGNAL(errorOperatorSignal(COperator::Id,string)),this,SLOT(errorOperatorSlot(COperator::Id,string)));
}
void OperatorErrorString::errorOperatorSlot(COperator::Id opid,string msg){
    if(this->plugOut().size()==2){
        dynamic_cast<DataString *>(this->plugOut()[0]->getData())->setValue(msg);
        dynamic_cast<DataNumber *>(this->plugOut()[1]->getData())->setValue(opid);
        this->updateMarkingAfterExecution();
    }
}
bool OperatorErrorString::executionCondition(){
    return false;
}

void OperatorErrorString::exec(){
    //    string str =
    //    dynamic_cast<DataNumber *>(this->plugOut()[0]->getData())->setValue((int)str.Error());
}

COperator * OperatorErrorString::clone(){
    return new OperatorErrorString;
}
