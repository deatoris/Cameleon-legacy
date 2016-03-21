#include "erase.h"
#include<DataVector.h>
#include<DataNumber.h>
erase::erase(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataNumber::KEY,"index1.num");
    this->structurePlug().addPlugIn(DataNumber::KEY,"index2.num");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("vectorerasevector");
    this->setName("EraseVector");
    this->setInformation("removes from the vector container either a single element (index1) or a range of elements ([index1,index2)).");
}
void erase::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    this->plugIn()[1]->setState(CPlug::EMPTY);
    this->plugIn()[2]->setState(CPlug::OLD);
    this->plugOut()[0]->setState(CPlug::EMPTY);
}
void erase::exec(){
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    int index1 = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();
    if(index1>=0&&index1<(int)vec->size()){
        if(this->plugIn()[2]->isConnected()==false){

            vec->erase(vec->begin()+index1);
        }
        else{
            int index2 = dynamic_cast<DataNumber *>(this->plugIn()[2]->getData())->getValue();
            if(index2>=0&&index2<(int)vec->size()&&index1<index2){
                vec->erase(vec->begin()+index1,vec->begin()+index2);
            }
        }
    }
    else{
        this->error("index1>=0 && index1<vec.size() and if index2 index1<index2 && index2>=0 && index2<vec.size() ");
    }
    this->plugOut()[0]->getData()->setMode(this->plugIn()[0]->getData()->getMode());
     dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(vec);
}

COperator * erase::clone(){
    return new erase();
}
