#include "split.h"


split::split(){
    this->structurePlug().addPlugIn(DataString::KEY,"in1.str");
    this->structurePlug().addPlugIn(DataString::KEY,"separtor.str");
    this->structurePlug().addPlugOut(DataVector::KEY,"string.vec");
    this->path().push_back("String");
    this->setKey("stringsplitstring");
    this->setName("SplitString");
    this->setInformation("Splits the string into substrings with the separtor string\nThe substrings are included in a vector of DataString");

}

void split::exec(){
    string v1 = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    string v2 = *dynamic_cast<DataString *>(this->plugIn()[1]->getData())->getData();

    QString str1(v1.c_str());
    QString str2(v2.c_str());
    QStringList spL = str1.split(str2);
    vector<shared_ptr<CData> > * v = new vector<shared_ptr<CData> > ;
    foreach(QString s, spL){
        DataString* st = new DataString();
        st->setValue(s.toStdString());
        v->push_back(shared_ptr<CData>(st));
    }

    dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(v);
}

COperator * split::clone(){
    return new split();
}
