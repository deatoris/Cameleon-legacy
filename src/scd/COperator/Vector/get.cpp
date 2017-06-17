#include "get.h"


get::get(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataNumber::KEY,"index.num");
    this->structurePlug().addPlugOut(DataGeneric::KEY,"element.gen");
    //this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("vectorgetvector");
    this->setName("GetElementVector");
    this->setInformation("Get the data given by the index");
}

void get::exec(){
    DataVector::DATAVECTOR vec = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();

    int ind = dynamic_cast<DataNumber *>(this->plugIn()[1]->getData())->getValue();

    if(ind<(int)vec->size() && ind != -1)
    {
        DataGeneric* gen = dynamic_cast<DataGeneric *>(this->plugOut()[0]->getData());
        gen->dumpReception(vec->operator [](ind).get());
    }
    else
    {
        this->error("Out of range: size is: "+QString::number((int)vec->size()).toStdString()+" required element is: "+QString::number(ind).toStdString());
        return;
    }
}

COperator * get::clone(){
    return new get();
}

compareVector::compareVector(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugOut(DataBoolean::KEY,"element.bool");
    //this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("compareVector");
    this->setName("compareVector");
    this->setInformation("camapre two vectors, return true or false");
}

void compareVector::exec(){
    DataVector::DATAVECTOR vec1 = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    DataVector::DATAVECTOR vec2 = dynamic_cast<DataVector *>(this->plugIn()[1]->getData())->getData();
    DataBoolean * dret = dynamic_cast<DataBoolean *>(this->plugOut()[0]->getData());
    dret->setValue(true);

    if((int)vec2->size() != (int)vec1->size()) dret->setValue(false);

    for(int i=0;i<(int)vec1->size() && i<(int)vec2->size();i++){
        DataGeneric* gen1 = vec1->operator [](i).get();
        DataGeneric* gen2 = vec2->operator [](i).get();
        if (gen1->toString().compare(gen2->toString()) != 0) dret->setValue(false);
    }
}

COperator * compareVector::clone(){
    return new compareVector();
}

concatVector::concatVector(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in1.vec");
    this->structurePlug().addPlugIn(DataVector::KEY,"in2.vec");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    //this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("concatVector");
    this->setName("concatVector");
    this->setInformation("out=in1+in2");
}

void concatVector::exec(){
    DataVector::DATAVECTOR vec1 = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    DataVector::DATAVECTOR vec2 = dynamic_cast<DataVector *>(this->plugIn()[1]->getData())->getData();
    DataVector::DATAVECTOR out = dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->getData();

    out->clear();

    for(int i=0;i<(int)vec1->size();i++){
        DataGeneric* gen1 = vec1->operator [](i).get();
        DataGeneric* gen2 = gen1->clone();
        gen2->dumpReception(gen1);
        out->push_back(shared_ptr<CData>(gen2));
    }

    for(int i=0;i<(int)vec2->size();i++){
        DataGeneric* gen1 = vec2->operator [](i).get();
        DataGeneric* gen2 = gen1->clone();
        gen2->dumpReception(gen1);
        out->push_back(shared_ptr<CData>(gen2));
    }

}

COperator * concatVector::clone(){
    return new concatVector();
}

findVector::findVector(){
    this->structurePlug().addPlugIn(DataVector::KEY,"in.vec");
    this->structurePlug().addPlugIn(DataGeneric::KEY,"data.gen");
    this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    //this->structurePlug().addPlugOut(DataVector::KEY,"out.vec");
    this->path().push_back("Vector");
    this->setKey("findVector");
    this->setName("findVector");
    this->setInformation("findVector");
}

void findVector::exec(){
    DataVector::DATAVECTOR vec1 = dynamic_cast<DataVector *>(this->plugIn()[0]->getData())->getData();
    DataVector::DATAVECTOR out = dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->getData();

    out->clear();

    for(int i=0;i<(int)vec1->size();i++){
        DataGeneric* gen1 = vec1->operator [](i).get();
        string s1 = this->plugIn()[1]->getData()->toString();
        string s2 = gen1->toString();
        if (s1.compare(s2) == 0){
            DataNumber* gen2 = new DataNumber();
            gen2->setValue(i);
            out->push_back(shared_ptr<CData>(gen2));
        }


    }
}

COperator * findVector::clone(){
    return new findVector();
}
