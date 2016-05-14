#ifndef LOADIMAGEQT_H
#define LOADIMAGEQT_H
#include<COperator.h>

class LoadImageQT: public COperator
{
public:
    LoadImageQT();
    void exec();
    COperator * clone();
    //void initState();
};
#endif // LOADIMAGEQT_H
