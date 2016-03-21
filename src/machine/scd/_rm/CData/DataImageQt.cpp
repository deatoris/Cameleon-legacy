#include "DataImageQt.h"

DataImageQt::DataImageQt()
    :CDataByFile<QImage>()
{
//    _data =tr1::shared_ptr<QImage>(new QImage);
    this->_key = DataImageQt::KEY;
    this->setExtension(".bmp");
}


string DataImageQt::KEY ="DataImageQt";

DataImageQt * DataImageQt::clone()
{
    return new DataImageQt;
}

//void DataImageQt::setDataByFile(QImage * type){
//    type->save( this->getFile().c_str());
//}
QImage * DataImageQt::cloneData(QImage * type){
    return new QImage(*type);
}

//QImage * DataImageQt::getDataByFile(){
//     QImage  *t= new QImage;
//    t->load(this->getFile().c_str());
//    return t;
//}
