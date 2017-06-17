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
//    void setDataByFile(QImage * type);
       QImage * cloneData(QImage * type);
//    QImage * getDataByFile();
};


#endif // DATAIMAGEQT_H
