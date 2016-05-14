#include "DataImageQt.h"

DataImageQt::DataImageQt()
    :CDataByFile<QImage>()
{
    _data =shared_ptr<QImage>(new QImage);
    this->_key = DataImageQt::KEY;
    this->setExtension(".bmp");
}


string DataImageQt::KEY ="DataImageQt";

DataImageQt * DataImageQt::clone()
{
    return new DataImageQt;
}

void DataImageQt::setDataByFile(shared_ptr<QImage > type){
    string str = this->getFile().c_str();
    type->save( this->getFile().c_str());
}
void DataImageQt::setDataByCopy(shared_ptr<QImage > type){
    _data = shared_ptr<QImage >(new QImage(*(type.get())));
}

shared_ptr<QImage> DataImageQt::getDataByFile(){
    shared_ptr<QImage > t(new QImage);

    t->load(this->getFile().c_str());

    return t;
}
