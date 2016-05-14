#include "SCDOperatorGetDataKeysVector.h"

#include<CData.h>
#include<DataVector.h>
#include<DataString.h>
#include "DataDictionnary.h"
#include "CGlossary.h"
SCDOperatorGetDataKeysVector::SCDOperatorGetDataKeysVector(){

    this->structurePlug().addPlugIn(DataDictionnary::KEY,"dictionnary.dic");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Introspection");
    this->path().push_back("Glossary");
    this->setKey("vectorSCDOperatorGetDataKeysVectorvector");
    this->setName("GetDataKeys");
    this->setInformation("Get the vector of data keys from the input dictionnary\n For no input dictionnary, the input dictionnary will be all dictionnaries");
}

void SCDOperatorGetDataKeysVector::exec(){
    vector<string> datakeys;
    if(this->plugIn()[0]->isConnected()==true){
        shared_ptr<pair<vector<string>,pair<string,string> > >v = dynamic_cast<DataDictionnary *>(this->plugIn()[0]->getData())->getData();
        datakeys = v->first;
    }else
    {
        datakeys =CGlossarySingletonServer::getInstance()->getDataKeys();
    }
    vector<shared_ptr<CData> > * vec = new vector<shared_ptr<CData> >;
    vector<string>::iterator it;
    for(it = datakeys.begin();it!=datakeys.end();it++){
        DataString * c = new DataString;
        c->setValue(*it);
        vec->push_back(shared_ptr<CData>(c));
    }
    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(vec);

}
bool SCDOperatorGetDataKeysVector::executionCondition(){

    if(this->plugIn()[0]->getState()!=CPlug::NEW && this->plugIn()[0]->isConnected()==true)
        return false;
    else if(this->plugOut()[0]->getState()==CPlug::NEW&&this->plugOut()[0]->isConnected()==true)
        return false;
    else if( this->plugIn()[0]->getState()!=CPlug::EMPTY && this->plugIn()[0]->isConnected()==false)
        return false;
    else
        return true;
}
void SCDOperatorGetDataKeysVector::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugOut()[0]->setState(CPlug::EMPTY);
}

COperator * SCDOperatorGetDataKeysVector::clone(){
    return new SCDOperatorGetDataKeysVector();
}
