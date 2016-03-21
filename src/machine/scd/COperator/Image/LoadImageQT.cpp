#include "LoadImageQT.h"

#include<DataImageQt.h>
#include<DataString.h>
LoadImageQT::LoadImageQT()
    :COperator()
{

    this->path().push_back("ImageQT");
    this->setKey("SCDLoadImageQT");
    this->setName("Load");
    this->setInformation("Load image by file");
    this->structurePlug().addPlugIn(DataString::KEY,"file.str");
    this->structurePlug().addPlugOut(DataImageQt::KEY,"h.bmp");
}

void LoadImageQT::exec(){
    string file = dynamic_cast<DataString *>(this->plugIn()[0]->getData())->getValue();
    try{
        QImage* img = new QImage(file.c_str());
        dynamic_cast<DataImageQt *>(this->plugOut()[0]->getData())->setData(shared_ptr<QImage>(img));
    }
    catch(string msg){
        this->error(msg);
        return;
    }

}

COperator * LoadImageQT::clone(){
    return new LoadImageQT();
}
