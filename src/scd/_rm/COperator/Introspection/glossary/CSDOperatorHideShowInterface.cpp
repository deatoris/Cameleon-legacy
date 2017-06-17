#include "CSDOperatorHideShowInterface.h"
#include "DataNumber.h"
#include "CGLayoutManager.h"

CSDOperatorHideShowInterface::CSDOperatorHideShowInterface(){
    this->structurePlug().addPlugIn(DataNumber::KEY,"layoutid.num");
    this->path().push_back("Introspection");
    this->path().push_back("Glossary");
    this->setKey("stringCSDOperatorHideShowInterfacestring");
    this->setName("ShowInterface");
    this->setExecutedThread(COperator::CLIENT);
}

void CSDOperatorHideShowInterface::exec(){
    int id = dynamic_cast<DataNumber *>(this->plugIn()[0]->getData())->getValue();
    CGLayoutManager::getInstance()->showLayout(id);
}

COperator * CSDOperatorHideShowInterface::clone(){
    return new CSDOperatorHideShowInterface();
}

