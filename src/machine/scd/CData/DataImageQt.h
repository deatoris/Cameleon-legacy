#ifndef DATAIMAGEQT_H
#define DATAIMAGEQT_H

#include<CDataByFile.h>
using namespace std;
#include<QImage>
class DataImageQt : public CDataByFile<QImage  >
{
public:
    DataImageQt();
    static string KEY;
    virtual DataImageQt * clone();
    void setDataByFile(shared_ptr<QImage > type);
    void setDataByCopy(shared_ptr<QImage > type);
    shared_ptr<QImage > getDataByFile();
};


#endif // DATAIMAGEQT_H
