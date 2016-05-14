#include "readdir.h"
#include "DataBoolean.h"
readdir::readdir(){
    this->structurePlug().addPlugIn(DataString::KEY,"dirpath.str");
    this->structurePlug().addPlugIn(DataBoolean::KEY,"recurcive.bool (by default false)");
    this->structurePlug().addPlugOut(DataVector::KEY,"string.vec");
    this->path().push_back("FileSystem");
    this->setKey("direreaddir");
    this->setName("ReadDir");
    this->setInformation("List every entries (file and directory) from an absolute dir path.");
}

void readdir::exec(){
    string dirpath = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();

    bool recurcive = false;
    if(this->plugIn()[1]->isDataAvailable()==true){
        recurcive = dynamic_cast<DataBoolean *>(this->plugIn()[1]->getData())->getValue();
    }

    if(!recurcive){
        QStringList listFilter;
        listFilter << "*";

        QDirIterator fileIterator(dirpath.c_str(), listFilter ,QDir::AllEntries | QDir::NoDotAndDotDot);
        vector<shared_ptr<CData> > * v = new vector<shared_ptr<CData> > ;
        while(fileIterator.hasNext())
        {
            DataString* st = new DataString();
            st->setValue(fileIterator.next().toStdString());
            v->push_back(shared_ptr<CData>(st));
        }
        dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(DataVector::DATAVECTOR(v));
    }else{
        QStringList listFilter;
        listFilter << "*";

        QDirIterator fileIterator(dirpath.c_str(), listFilter ,QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
        vector<shared_ptr<CData> > * v = new vector<shared_ptr<CData> > ;
        while(fileIterator.hasNext())
        {
            DataString* st = new DataString();
            st->setValue(fileIterator.next().toStdString());
            v->push_back(shared_ptr<CData>(st));
        }
        dynamic_cast<DataVector *>(this->plugOut()[0]->getData())->setData(DataVector::DATAVECTOR(v));
    }
}

void readdir::initState(){
    this->plugIn()[0]->setState(CPlug::EMPTY);
    if(this->plugIn()[1]->isConnected()==false)
        this->plugIn()[1]->setState(CPlug::OLD);
    else
        this->plugIn()[1]->setState(CPlug::EMPTY);

    this->plugOut()[0]->setState(CPlug::EMPTY);
}

COperator * readdir::clone(){
    return new readdir();
}
